#include "LISTBOX.h"
#include "GUI.h"
#include "SCROLLBAR.h"

static void LISTBOX__DrawPage(LISTBOX_Obj *pObj);
static void LISTBOX__TextScroll(LISTBOX_Obj *pObj);
static void LISTBOX__SetSelInfo(LISTBOX_Obj *pObj);

/* LISTBOX控件自动回调函数 */
static void __Callback(WM_MESSAGE *pMsg)
{
    static i_16 dY;
    /* 检测是否为LISTBOX控件 */
    if (WM_CheckWindowSign(pMsg->hWin, WIDGET_LISTBOX)) {
        return;
    }
    switch (pMsg->MsgId) {
        case WM_PAINT :
            LISTBOX__DrawPage(pMsg->hWin);
            break;
        case WM_DELETE :
            GUI_fastfree(pMsg->hWin);
            break;
        case WM_TP_CHECKED :
            WM_SetActiveMainWindow(pMsg->hWin);
            break;
        case WM_TP_PRESS :
            dY += GUI_GetTouchPad_MoveY();
            if (dY > 20) {
                dY = 0;
                LISTBOX_ItemDown(pMsg->hWin);
            } else if (dY < -20) {
                dY = 0;
                LISTBOX_ItemUp(pMsg->hWin);
            }
            break;
        case WM_TP_LEAVE :
            dY = 0;
            break;
        case WM_TIME_UPDATA :
            LISTBOX__TextScroll(pMsg->hWin);
            break;
    }
}

/*
 *创建LISTBOX
 *x0:LISTBOX控件的最左像素(相对于父窗口)
 *y0:LISTBOX控件的最右像素(相对于父窗口)
 *xSize:LISTBOX控件的水平宽度
 *ySize:LISTBOX控件的竖直高度
 *hParent:父窗口句柄
 *Id:窗口ID
 *Flag:窗口状态
 *cb:用户回调历程指针
 */
WM_hWin LISTBOX_Create(i_16 x0,
                      i_16 y0,
                      u_16 xSize,
                      u_16 ySize,
                      WM_hWin hParent,
                      u_16 Id,
                      u_8 Flag,
                      u_16 namepos)
{
    LISTBOX_Obj *pObj;
    
    pObj = WM_CreateWindowAsChild(x0, y0, xSize, ySize, hParent, Flag,
                                  WIDGET_LISTBOX, Id, __Callback,
                                  sizeof(LISTBOX_Obj) - sizeof(WM_Obj));
    if (pObj == NULL) {
        return NULL;
    }
    /* 设置用户区 */
    pObj->Widget.Win.UserRect.x0 = pObj->Widget.Win.Rect.x0;
    pObj->Widget.Win.UserRect.y0 = pObj->Widget.Win.Rect.y0;
    pObj->Widget.Win.UserRect.x1 = pObj->Widget.Win.Rect.x1;
    pObj->Widget.Win.UserRect.y1 = pObj->Widget.Win.Rect.y1;
    /* 配色 */
    pObj->Widget.Skin.BackColor[0] = LBOX_DFT_LBKCOLOR;   //未选中背景
    pObj->Widget.Skin.BackColor[1] = LBOX_DFT_LSELBKCOLOR;//选中背景
    pObj->Widget.Skin.FontColor[0] = LBOX_DFT_LNCOLOR;    //未选中字体
    pObj->Widget.Skin.FontColor[1] = LBOX_DFT_LSELCOLOR;  //选中字体
    pObj->Widget.Skin.EdgeColor[0] = LBOX_DFT_RIMCOLOR;   //边框颜色
    pObj->Widget.Skin.EdgeColor[0] = LBOX_DFT_LINECOLOR;  //分隔线颜色
    WIDGET_SetFont(pObj, GUI_DEF_FONT);
    pObj->ItemHei=LBOX_DFT_HEIGHT;             //条目高度+分隔线高度
    pObj->TopIndex = 0;
    pObj->SelIndex = 0;                           //选中的索引
    pObj->SelPixs = 0;
    pObj->PageItems = ySize / (pObj->ItemHei + 1);//每页可显示的条目数
    pObj->ItemNum=0;                //总条目数清零
     pObj->pList=ListInit(namepos);    //空链表,链表数据长度为namepos
    pObj->LastNode=pObj->pList;
    pObj->StrTab = NULL;
    pObj->hScro  = NULL;
    pObj->ScbWidth = LBOX_SCB_WIDTH;
    pObj->DispPosPix = 0;
    WM_SetWindowTimer(pObj, LBOX_DFT_SCRO_TIME);//需要窗口计时器
    return pObj;
}

/* 创建列表框滚动条 */
static u_8 __CreateScro(GUI_hWin hWin)
{
    LISTBOX_Obj *pObj = hWin;
    if (pObj->hScro == NULL) {
        i_16 x0;
        GUI_RECT Rect;
        
        Rect = WM_GetWindowUserRect(pObj);
        x0 = Rect.x1 - Rect.x0 - pObj->ScbWidth + 1;
        pObj->hScro = SCROLLBAR_Create(x0, 0, pObj->ScbWidth,
                                       Rect.y1 - Rect.y0 + 1,
                                       pObj, 0x0001, 0);
        return GUI_OK;
    }
    return GUI_ERR;
}

//增加一条pObj的条目
//0,增加成功;
//1,增加失败
u_8 LISTBOX_addlist(WM_hWin hWin,char *name)
{
    LISTBOX_Obj *pObj = hWin;
    
    if (WM_CheckWindowSign(hWin, WIDGET_LISTBOX)) {
        return GUI_ERR;
    }
    //if(pObj->StrTab) return GUI_ERR;  //已经锁定使用一次性添加模式
    Insert(name,pObj->LastNode); //插入到链表结尾
    pObj->LastNode=pObj->LastNode->next;   //仅有本函数使用
    pObj->ItemNum++;//总条目数增加1条
    if (pObj->ItemNum > pObj->PageItems) {
        __CreateScro(pObj);
        SCROLLBAR_SetTotality(pObj->hScro, pObj->ItemNum);
        SCROLLBAR_SetLoation(pObj->hScro, pObj->SelIndex);
    }
    LISTBOX__SetSelInfo(pObj);  //选中项
    return GUI_OK;
}

/************************************************************
* 一次性添加pObj的所有条目，这种方式添加条目后无法再做更改
* *name的长度必须等于pObj->list->datalen！
* 0,增加成功
* 1,增加失败
*************************************************************/
u_8 LISTBOX_addall(WM_hWin hWin,const char *pTab,u_16 num)
{
    
    return 0;
}

//获取指定条目的名字
//pObj目标列表框
//idxpos要获取名字的条目数
static PNode Get__ItemName(WM_hWin hWin,u_16 Item)
{
    LISTBOX_Obj *pObj = hWin;

    if(pObj->StrTab == NULL) {
        return GetNodePtr(pObj->pList,Item + 1);
    }
    return NULL;
}

static PNode Get__NextItemName(LISTBOX_Obj *pObj, PNode pNode)
{
    if(!pObj->StrTab) {
        return pNode->next;
    }
    return NULL;
}

//绘制一个条目
//pObj要绘制的列表框
//idxpos要绘制条目的偏移
static void LISTBOX__DrawList(LISTBOX_Obj *pObj, u_16 ItemPos, char *Str)
{
    GUI_FontType Font;
    i_16 x0, y0, xSize, ySize, xPixPos;
    GUI_COLOR FontColor, BkColor;
    GUI_RECT *pRect;
    
    /* 获取窗口用户区 */
    pRect = &pObj->Widget.Win.UserRect; 
    /* 计算条目位置 */
    x0 = pRect->x0;
    y0 = pRect->y0 + (pObj->ItemHei + 1) * ItemPos;
    xSize = pRect->x1 - x0 + 1;
    ySize = pObj->ItemHei + 1;
    if(ItemPos + pObj->TopIndex == pObj->SelIndex) /* 选中的条目 */
    {
        BkColor = WIDGET_GetBackColor(pObj, 1);
        FontColor = WIDGET_GetFontColor(pObj, 1);
        xPixPos = pObj->DispPosPix;
    } else {                    /* 未选中的条目 */
        BkColor = WIDGET_GetBackColor(pObj, 0);
        FontColor = WIDGET_GetFontColor(pObj, 0);
        xPixPos = 0;
    }
    GUI_FillTailorRect(x0, y0, xSize, ySize - 1, BkColor);
    /* 分隔线 */
    BkColor = WIDGET_GetEdgeColor(pObj, 0);
    GUI_HoriTailorLine(x0 + 1, y0 + ySize - 1, xSize - 2, BkColor);
    /* 显示条目内容,文字不能覆盖分隔线 */
    Font = WIDGET_GetFont(pObj);
    if (pObj->ItemHei > Font->CharHeight) { /* 字体高度居中显示 */
        y0 += (pObj->ItemHei - Font->CharHeight) / 2;
    }
    x0 -= xPixPos;   //显示偏移
    GUI_DispStringCurRect(x0, y0, Str, FontColor, Font);
}

//绘制一页pObj,从pObj->TopIndex开始绘制
static void LISTBOX__DrawPage(LISTBOX_Obj *pObj)
{
    PNode pNode;
    GUI_RECT Rect;
    u_16 i, PageItems;
    i_16 x0, y0, xSize, ySize;

    PageItems = pObj->PageItems; /* 一页可以显示的条数 */
    pNode = Get__ItemName(pObj, pObj->TopIndex);
    for(i = 0; i <= PageItems; ++i) /* 显示条目 */
    {
        LISTBOX__DrawList(pObj, i, pNode->data);
        pNode = Get__NextItemName(pObj, pNode);
        /* 已经到了最后一条,但i依然要自加1 */
        if(i + pObj->TopIndex + 1 == pObj->ItemNum) {
            ++i;
            break;
        }
    }
    Rect = WM_GetWindowUserRect(pObj);
    i *= (pObj->ItemHei + 1);  /* 要清空的y方向偏移 */
    x0 = Rect.x0;
    y0 = Rect.y0 + i;
    xSize = Rect.x1 - Rect.x0 + 1;
    ySize = Rect.y1 - Rect.y0 + 1;
    /* 清空为底色 */
    GUI_FillTailorRect(x0, y0, xSize, ySize, WIDGET_GetBackColor(pObj, 0));
}

/* LISTBOX滚动显示 */
static void LISTBOX__TextScroll(LISTBOX_Obj *pObj)
{
    u_16 ItemPos;
    u_16 ListWidth;
    GUI_RECT Rect;

    Rect = WM_GetWindowUserRect(pObj);
    ListWidth = Rect.x1 - Rect.x0 + 1;
    if (ListWidth >= pObj->SelPixs) {
        return;   //不需要滚动显示
    }
    if (pObj->DispPosPix == 0) {
        if (WM_GetWindowTimerCount(pObj) == LBOX_DFT_SCRO_TIME) {
            WM_SetWindowTimerCount(pObj, LBOX_DFT_UNSCRO_TIME);
            return;
        }
        WM_SetWindowTimerCount(pObj, LBOX_DFT_SCRO_TIME);
    }
    ItemPos = pObj->SelIndex - pObj->TopIndex;
    if (ListWidth + pObj->DispPosPix == pObj->SelPixs + LBOX_DFT_SCROENDWID) {
        pObj->DispPosPix = 0;
    } else {
        pObj->DispPosPix += 1;
    }
    Rect.y0 += ItemPos * (pObj->ItemHei + 1);
    Rect.y1 = Rect.y0 + pObj->ItemHei - 1;
    WM_InvalidateRect(pObj, &Rect);
}

/* 设置选中项目的信息 */
static void LISTBOX__SetSelInfo(LISTBOX_Obj *pObj)
{
    char *Str;

    pObj->DispPosPix = 0;
    Str = Get__ItemName(pObj, pObj->SelIndex)->data;
    pObj->SelItem = Str;
    pObj->SelPixs = GetStringPixel(Str, WIDGET_GetFont(pObj));
}

//将列表框的选中项下移一栏
void LISTBOX_ItemDown(WM_hWin hWin)
{
    LISTBOX_Obj *pObj = hWin;
    GUI_RECT Rect;
    u_16 PageItems, ItemPos;

    Rect = WM_GetWindowUserRect(pObj);
    ItemPos = pObj->SelIndex - pObj->TopIndex;
    PageItems = pObj->PageItems;
    if(pObj->SelIndex + 1 < pObj->ItemNum) { /* 还没有显示到最后一条 */
        pObj->SelIndex++;
        if(ItemPos + 1 == PageItems) {        /* 到了本页的最后一条 */
            pObj->TopIndex += PageItems;
        } else {
            Rect.y0 += ItemPos * (pObj->ItemHei + 1);
            Rect.y1 =  Rect.y0 + 2 * pObj->ItemHei + 1;
        }
    } else {               /* 到了最后一条 */
        pObj->SelIndex = 0;
        pObj->TopIndex = 0;
    }
    /* 设置选中项目的信息 */
    LISTBOX__SetSelInfo(pObj);
    if (pObj->hScro) {  /* 设置滚动条 */
        SCROLLBAR_SetLoation(pObj->hScro, pObj->SelIndex);
    }
    WM_InvalidateRect(pObj, &Rect);  /* 无效化窗口 */
}

//将列表框的选中项上移移一栏
void LISTBOX_ItemUp(WM_hWin hWin)
{
    LISTBOX_Obj *pObj = hWin;
    GUI_RECT Rect;
    u_16 PageItems, ItemPos, Temp;

    Rect = WM_GetWindowUserRect(pObj);
    ItemPos = pObj->SelIndex - pObj->TopIndex;
    PageItems = pObj->PageItems;
    if(pObj->SelIndex) {        /* 还没有显示到第一条 */
        pObj->SelIndex--;
        if(ItemPos == 0) {      /* 到了本页的第一条 */
            pObj->TopIndex -= PageItems;
        } else {
            Rect.y0 += (ItemPos - 1) * (pObj->ItemHei + 1);
            Rect.y1 =  Rect.y0 + 2 * pObj->ItemHei + 1;
        }
    } else {                    /* 到了第一条 */
        pObj->SelIndex = pObj->ItemNum - 1;
        /*   计算最后一页的TopIndex值 */
        Temp = pObj->ItemNum % PageItems;
        Temp = Temp ? Temp : PageItems;
        pObj->TopIndex = pObj->ItemNum - Temp;
    }
    /* 设置选中项目的信息 */
    LISTBOX__SetSelInfo(pObj);
    if (pObj->hScro) {  /* 设置滚动条 */
        SCROLLBAR_SetLoation(pObj->hScro, pObj->SelIndex);
    }
    WM_InvalidateRect(pObj, &Rect);  /* 无效化窗口 */
}

/* 返回选中项目数 */
u_16 LISTBOX_GetSel(WM_hWin hWin)
{
    LISTBOX_Obj *pObj = hWin;
    
    /* 检测是否为LISTBOX控件 */
    if (WM_CheckWindowSign(hWin, WIDGET_LISTBOX)) {
        return 0;
    }
    return pObj->SelIndex;
}

/* 返回选中项目的文本 */
char *LISTBOX_GetSelText(WM_hWin hWin)
{
    LISTBOX_Obj *pObj = hWin;
    
    /* 检测是否为LISTBOX控件 */
    if (WM_CheckWindowSign(hWin, WIDGET_LISTBOX)) {
        return 0;
    }
    return pObj->SelItem;
}

/* 设置选中项 */
u_8 LISTBOX_SetSel(WM_hWin hWin, u_16 Index)
{
    u_16 Temp;
    LISTBOX_Obj *pObj = hWin;
    
    /* 检测是否为LISTBOX控件 */
    if (WM_CheckWindowSign(hWin, WIDGET_LISTBOX)) {
        return GUI_ERR;
    }
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
u_8 LISTBOX_SetSelFromStr(WM_hWin hWin, const char *Str)
{
    u_16 Index;
    LISTBOX_Obj *pObj = hWin;
    
    /* 检测是否为LISTBOX控件 */
    if (WM_CheckWindowSign(hWin, WIDGET_LISTBOX)) {
        return GUI_ERR;
    }
    Index = FindNodeStr(pObj->pList, (char*)Str);
    if (Index) {
        return LISTBOX_SetSel(pObj, Index - 1);
    }
    return GUI_ERR;
}
