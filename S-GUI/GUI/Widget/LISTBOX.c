#include "LISTBOX.h"
#include "GUI.h"
#include "SCROLLBAR.h"
#include <string.h>

#define LBOX_DFT_LBKCOLOR       0x00111F1F        /* 内部背景色 */
#define LBOX_DFT_LSELCOLOR      0x000A0A0A        /* 选中list后的字体颜色 */
#define LBOX_DFT_LSELBKCOLOR    0x004876FF        /* 选中list后的背景色 */
#define LBOX_DFT_LNCOLOR        0X00E3E3E3        /* 未选中的list字体颜色 */
#define LBOX_DFT_RIMCOLOR       0x00000000        /* 边框颜色 */
#define LBOX_DFT_LINECOLOR      0x002A3033        /* 分隔线颜色 */

#define LBOX_DFT_HEIGHT         20      /* 条目高度 */
#define LBOX_DFT_SCROENDWID     48      /* 滚动显示末尾空白宽度 */
#define LBOX_DFT_UNSCRO_TIME    600     /* 选中项开始不滚动的ms数 */
#define LBOX_DFT_TIMER          50      /* 选中项显示滚动一个像素间隔时间(ms) */
#define LBOX_DFT_SCRO_PIX       2       /* 每次滚动的像素数 */
#define LBOX_SCB_WIDTH          5       /* 滚动条宽度为5个像素 */

static void _DrawPage(LISTBOX_Obj *pObj);
static void _ItemMove(LISTBOX_Obj *pObj, GUI_POINT *p);
static void _CheckSetItem(LISTBOX_Obj *pObj, GUI_POINT *p);
static void _TimerHandle(LISTBOX_Obj *pObj);
static void _SetSpeed(LISTBOX_Obj *pObj, GUI_POINT *p);
static void LISTBOX__SetSelInfo(LISTBOX_Obj *pObj);

/* LISTBOX控件自动回调函数 */
static void __Callback(WM_MESSAGE *pMsg)
{
    switch (pMsg->MsgId) {
        case WM_PAINT :
            _DrawPage(pMsg->hWin);
            break;
        case WM_DELETE :
            /* 删除链表 */
            List_Delete(((LISTBOX_Obj *)pMsg->hWin)->pList);
            break;
        case WM_TP_CHECKED :
            WM_SetForegroundWindow(pMsg->hWin);
            ((LISTBOX_Obj *)pMsg->hWin)->lTime = GUI_GetTime();
            break;
        case WM_TP_PRESS :
            _ItemMove(pMsg->hWin, (GUI_POINT *)pMsg->Param);
            _SetSpeed(pMsg->hWin, (GUI_POINT *)pMsg->Param);
            break;
        case WM_TP_REMOVED:
            _CheckSetItem(pMsg->hWin, (GUI_POINT *)pMsg->Param);
            break;
        case WM_TIME_UPDATA:
            _TimerHandle(pMsg->hWin);
            break;
        default :
            WM_DefaultProc(pMsg);
    }
}

/*
 * 创建LISTBOX
 * x0:LISTBOX控件的最左像素(相对于父窗口)
 * y0:LISTBOX控件的最右像素(相对于父窗口)
 * xSize:LISTBOX控件的水平宽度
 * ySize:LISTBOX控件的竖直高度
 * hParent:父窗口句柄
 * Id:窗口ID
 * Flag:窗口状态
 **/
WM_HWIN LISTBOX_Create(i_16 x0,
    i_16 y0,
    u_16 xSize,
    u_16 ySize,
    WM_HWIN hParent,
    u_16 Id,
    u_8 Style)
{
    LISTBOX_Obj *pObj;
    
    pObj = WM_CreateWindowAsChild(x0, y0, xSize, ySize, hParent, Style,
        Id, __Callback, sizeof(LISTBOX_Obj) - sizeof(WM_Obj));
    if (pObj == NULL) {
        return NULL;
    }
    /* 配色 */
    pObj->Widget.Skin.BackColor[0] = LBOX_DFT_LBKCOLOR;   /* 未选中背景 */
    pObj->Widget.Skin.BackColor[1] = LBOX_DFT_LSELBKCOLOR;/* 选中背景 */
    pObj->Widget.Skin.FontColor[0] = LBOX_DFT_LNCOLOR;    /* 未选中字体 */
    pObj->Widget.Skin.FontColor[1] = LBOX_DFT_LSELCOLOR;  /* 选中字体 */
    pObj->Widget.Skin.EdgeColor[0] = LBOX_DFT_RIMCOLOR;   /* 边框颜色 */
    pObj->Widget.Skin.EdgeColor[0] = LBOX_DFT_LINECOLOR;  /* 分隔线颜色 */
    WIDGET_SetFont(pObj, GUI_DEF_FONT);
    pObj->ItemHei = LBOX_DFT_HEIGHT;             /* 条目高度+分隔线高度 */
    pObj->TopIndex = 0;
    pObj->SelIndex = 0;                           /* 选中的索引 */
    pObj->SelPixs = 0;
    pObj->PageItems = ySize / (pObj->ItemHei + 1); /* 每页可显示的条目数 */
    pObj->ItemNum = 0;                /* 总条目数清零 */
    pObj->pList = List_Init();    /* 空链表,链表数据长度为namepos */
    pObj->LastNode = pObj->pList;
    pObj->hScro = NULL;
    pObj->DispPosPix = 0;
    pObj->yPos = 0;
    pObj->MoveFlag = 0;
    pObj->ScroSpeed = 0;
    return pObj;
}

/* 创建列表框滚动条 */
static u_8 __CreateScro(GUI_HWIN hWin)
{
    LISTBOX_Obj *pObj = hWin;
    if (pObj->hScro == NULL) {
        i_16 x0;
        GUI_RECT Rect;
        
        Rect = WM_GetWindowRect(pObj);
        x0 = Rect.x1 - Rect.x0 - LBOX_SCB_WIDTH + 1;
        pObj->hScro = SCROLLBAR_Create(x0, 0, LBOX_SCB_WIDTH,
                                       Rect.y1 - Rect.y0 + 1,
                                       pObj, WM_NULL_ID, 0);
        return GUI_OK;
    }
    return GUI_ERR;
}

//增加一条pObj的条目
//GUI_OK,增加成功;
//GUI_ERR,增加失败
GUI_RESULT LISTBOX_AddList(WM_HWIN hWin, char *name)
{
    LISTBOX_Obj *pObj = hWin;

    List_InsertNode(pObj->LastNode, name, strlen(name) + 1); /* 插入到链表结尾 */
    pObj->LastNode = pObj->LastNode->pNext;   /* 仅有本函数使用 */
    pObj->ItemNum++; /* 总条目数增加1条 */
    if (pObj->ItemNum > pObj->PageItems) {
        __CreateScro(pObj);
        SCROLLBAR_SetTotality(pObj->hScro, pObj->ItemNum * pObj->ItemHei);
        SCROLLBAR_SetLoation(pObj->hScro, pObj->SelIndex * pObj->ItemHei);
    }
    LISTBOX__SetSelInfo(pObj);  /* 选中项 */
    return GUI_OK;
}

//获取指定条目的名字
//pObj目标列表框
//idxpos要获取名字的条目数
static LIST Get__ItemName(WM_HWIN hWin,u_16 Item)
{
    LISTBOX_Obj *pObj = hWin;

    return List_GetNodePtr(pObj->pList, Item + 1);
}

static LIST Get__NextItemName(LISTBOX_Obj *pObj, LIST pNode)
{
    return pNode->pNext;
}

static void _DrawItem(LISTBOX_Obj *pObj, i_16 yPos, char *Str, char Sel)
{
    GUI_FONT Font;
    i_16 x0, y0, xSize, ySize, xPixPos;
    GUI_COLOR FontColor, BkColor;
    GUI_RECT *pRect;

    /* 获取窗口用户区 */
    pRect = &pObj->Widget.Win.Rect;
    /* 计算条目位置 */
    x0 = pRect->x0;
    y0 = pRect->y0 + yPos;
    xSize = pRect->x1 - x0 + 1;
    ySize = pObj->ItemHei;
    if (Sel) /* 选中的条目 */
    {
        BkColor = WIDGET_GetBackColor(pObj, 1);
        FontColor = WIDGET_GetFontColor(pObj, 1);
        xPixPos = pObj->DispPosPix;
    } else {                    /* 未选中的条目 */
        BkColor = WIDGET_GetBackColor(pObj, 0);
        FontColor = WIDGET_GetFontColor(pObj, 0);
        xPixPos = 0;
    }
    GUI_FillRect(x0, y0, xSize, ySize - 1, BkColor);
    /* 分隔线 */
    BkColor = WIDGET_GetEdgeColor(pObj, 0);
    GUI_HoriLine(x0, y0 + ySize - 1, xSize, BkColor);
    /* 显示条目内容,文字不能覆盖分隔线 */
    Font = WIDGET_GetFont(pObj);
    if (pObj->ItemHei - 1 > Font->CharHeight) { /* 字体高度居中显示 */
        y0 += (pObj->ItemHei - 1 - Font->CharHeight) / 2;
    }
    x0 -= xPixPos;   /* 显示偏移 */
    GUI_DispStringCurRect(x0, y0, Str, FontColor, Font);
}

/* 绘制一页pObj,从pObj->TopIndex开始绘制 */
static void _DrawPage(LISTBOX_Obj *pObj)
{
    LIST pNode;
    GUI_RECT Rect;
    u_16 i, PageItems;
    i_16 x0, y0, xSize, ySize;

    y0 = pObj->yPos;
    PageItems = pObj->PageItems; /* 一页可以显示的条数 */
    pNode = Get__ItemName(pObj, pObj->TopIndex);
    for (i = 0; i <= PageItems && pNode; ++i) /* 显示条目 */
    {
        _DrawItem(pObj, i * pObj->ItemHei + y0,
            pNode->pData, pObj->TopIndex + i == pObj->SelIndex);
        pNode = Get__NextItemName(pObj, pNode);
        /* 已经到了最后一条,但i依然要自加1 */
        if (i + pObj->TopIndex + 1 == pObj->ItemNum) {
            ++i;
            break;
        }
    }
    Rect = WM_GetWindowRect(pObj);
    i *= (pObj->ItemHei);  /* 要清空的y方向偏移 */
    x0 = Rect.x0;
    y0 += Rect.y0 + i;
    xSize = Rect.x1 - Rect.x0 + 1;
    ySize = Rect.y1 - Rect.y0 + 1;
    /* 清空为底色 */
    GUI_FillRect(x0, y0, xSize, ySize, WIDGET_GetBackColor(pObj, 0));
}

/* 滚动列表框 */
static void _ItemMove(LISTBOX_Obj *pObj, GUI_POINT *p)
{
    i_16 Top, y, yPos;

    /* 鼠标移动太少 */
    if (!pObj->MoveFlag && GUI_ABS(p[1].y) < 2) {
        return;
    }
    if (GUI_ABS(p[1].y) > 0) {
        y = pObj->yPos + p[1].y;
        Top = pObj->TopIndex - y / pObj->ItemHei;
        yPos = y % pObj->ItemHei;
        if (y > 0) {
            Top -= 1;
            yPos = -(pObj->ItemHei - yPos);
        }
        if (Top < 0) {
            Top = 0;
            yPos = 0;
            pObj->ScroSpeed = 0;
        } else if (Top >= pObj->ItemNum) {
            Top = pObj->ItemNum - 1;
            yPos = 0;
            pObj->ScroSpeed = 0;
        }
        pObj->yPos = yPos;
        pObj->TopIndex = Top;
        if (pObj->hScro) {  /* 设置滚动条 */
            SCROLLBAR_SetLoation(pObj->hScro,
                pObj->TopIndex * pObj->ItemHei - yPos);
        }
        WM_Invalidate(pObj);
        pObj->MoveFlag = 1;
    }
}

/* 设置被点击的选中项 */
static void _CheckSetItem(LISTBOX_Obj *pObj, GUI_POINT *p)
{
    int i;

    if (!pObj->MoveFlag) {
        if (GUI_CheckPointAtRect(p[0].x, p[0].y,
            &pObj->Widget.Win.Rect) == TRUE) {
            i = pObj->TopIndex + (-pObj->yPos
                + p[0].y - pObj->Widget.Win.Rect.y0) / pObj->ItemHei;
            if (i >= 0 && i < pObj->ItemNum) {
                WM_MESSAGE Msg;

                pObj->SelIndex = (u_16)i;
                /* 设置选中项目的信息 */
                LISTBOX__SetSelInfo(pObj);
                WM_Invalidate(pObj);  /* 无效化窗口 */
                Msg.MsgId = WM_LISTBOX_CHECK;
                WM_SendMessageToParent(pObj, &Msg);
            }
        }
    } else {
        if (GUI_ABS(pObj->ScroSpeed) > 20) {
            GUI_SetWindowTimer(pObj, LBOX_DFT_TIMER);
        } else {
            pObj->MoveFlag = 0;
        }
    }
}

/* 设置滚动速度 */
static void _SetSpeed(LISTBOX_Obj *pObj, GUI_POINT *p)
{
    GUI_TIME t = GUI_GetTime();

    pObj->ScroSpeed = (i_16)(p[1].y * LBOX_DFT_TIMER / (int)(t - pObj->lTime + 1));
    pObj->ScroSpeed *= 4;
    pObj->lTime = t;
}

/* 减速滚动 */
static void _TimerHandle(LISTBOX_Obj *pObj)
{
    if (pObj->MoveFlag) {
        if (GUI_ABS(pObj->ScroSpeed)) {
            i_16 d;
            GUI_POINT Point[2];

            Point[1].y = pObj->ScroSpeed;
            _ItemMove(pObj, Point);
            d = -pObj->ScroSpeed / 8;
            if (GUI_ABS(d) == 0 && pObj->ScroSpeed) {
                d = pObj->ScroSpeed > 0 ? -1 : 1;
            }
            pObj->ScroSpeed += d;
            if (!pObj->ScroSpeed) {
                pObj->ScroSpeed = 0;
                pObj->MoveFlag = 0;
                GUI_SetWindowTimer(pObj, 0);
            }
        } else {
            pObj->ScroSpeed = 0;
            GUI_SetWindowTimer(pObj, 0);
        }
    }
}

/* 设置选中项目的信息 */
static void LISTBOX__SetSelInfo(LISTBOX_Obj *pObj)
{
    char *Str;

    pObj->DispPosPix = 0;
    Str = Get__ItemName(pObj, pObj->SelIndex)->pData;
    pObj->SelItem = Str;
    pObj->SelPixs = GetStringPixel(Str, WIDGET_GetFont(pObj));
}

/* 返回选中项目数 */
u_16 LISTBOX_GetSel(WM_HWIN hWin)
{
    LISTBOX_Obj *pObj = hWin;

    return pObj->SelIndex;
}

/* 返回选中项目的文本 */
char *LISTBOX_GetSelText(WM_HWIN hWin)
{
    LISTBOX_Obj *pObj = hWin;

    return pObj->SelItem;
}

/* 设置选中项 */
GUI_RESULT LISTBOX_SetSel(WM_HWIN hWin, u_16 Index)
{
    u_16 Temp;
    LISTBOX_Obj *pObj = hWin;
    
    if (Index < pObj->ItemNum) {
        pObj->SelIndex = Index;
        if (pObj->hScro) {  /* 设置滚动条 */
            SCROLLBAR_SetLoation(pObj->hScro, Index);
        }
        Index += 1;
        Temp = Index % pObj->PageItems;
        if (Temp) {
            pObj->TopIndex = Index - Temp;
        } else {
            pObj->TopIndex = Index - pObj->PageItems;
        }
        /* 设置选中项目的信息 */
        LISTBOX__SetSelInfo(pObj);
        return GUI_OK;
    }
    return GUI_ERR;  /* 不存在这一项 */
}

/* 设置选中项,以输入的字符串来搜索 */
GUI_RESULT LISTBOX_SetSelFromStr(WM_HWIN hWin, const char *Str)
{
    u_16 Index;
    LISTBOX_Obj *pObj = hWin;
    
    Index = (u_16)List_FindStr(pObj->pList, (char*)Str);
    if (Index) {
        return LISTBOX_SetSel(pObj, Index - 1);
    }
    return GUI_ERR;
}

/* 列表框使用滚动显示功能 */
void LISTBOX_ScrollDisplay(GUI_HWIN hWin)
{
    GUI_SetWindowTimer(hWin, LBOX_DFT_TIMER);
}
