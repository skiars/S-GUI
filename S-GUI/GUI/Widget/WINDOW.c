#include "WINDOW.h"
#include "GUI.h"

#define WINDOW_DEF_CAPHEIGHT   20
/* 默认的窗体caption背景色caption的颜色. */
/* 以及窗体背景色. */
#define WINDOW_CAPTION_UPC          0x003C4C52  /* 窗口CAPTION上部分填充色 */
#define WINDOW_CAPTION_DOWNC        0x00333E42  /* 窗口CAPTION下部分填充色 */
#define WINDOW_TITLE_COLOR          0x0096CDCD  /* 窗口标题颜色 */
#define WINDOW_BODY_BKC             0x00292323  /* 窗口底色 */
#define WINDOW_FONT_COLOR           0x00FFFFFF  /* 窗口字体颜色 */

/* 标准窗体边框颜色定义 */
#define STD_WIN_RIM_OUTC            0x002A3033  /* 外线颜色 */
#define STD_WIN_RIM_MIDC            0x0054666D  /* 中线颜色 */
#define STD_WIN_RIM_INC             0x00303D42  /* 内线颜色 */

static WM_HWIN __GetClient(WINDOW_Obj *pObj)
{
    return pObj->hClient;
}

/* Window自绘函数 */
static void __Paint(WM_HWIN hWin)
{
    i_16 x0, y0;
    u_16 xSize, ySize;
    GUI_COLOR Color;
    GUI_RECT Rect;
    WINDOW_Obj *pObj = hWin;

    Rect = ((WM_Obj*)hWin)->Rect;
    x0 = Rect.x0;
    y0 = Rect.y0;
    xSize = Rect.x1 - x0 + 1;
    ySize = Rect.y1 - y0 + 1;
    
    /* 绘制外框 */
    Color = pObj->Widget.Skin.EdgeColor[0];
    GUI_DrawRect(x0, y0, xSize, ySize, Color);
    /* 绘制中框 */
    Color = pObj->Widget.Skin.EdgeColor[1];
    GUI_VertLine(x0 + 1, y0  + pObj->CaptionHeight + 1,
                       ySize - pObj->CaptionHeight - 2, Color);
    GUI_VertLine(x0 + xSize - 2, y0  + pObj->CaptionHeight + 1,
                       ySize - pObj->CaptionHeight - 2, Color);
    GUI_HoriLine(x0 + 1, y0  + ySize - 2, xSize - 2, Color);
    /* 绘制内框 */
    Color = pObj->Widget.Skin.EdgeColor[2];
    GUI_DrawRect(x0 + 2, y0 + pObj->CaptionHeight + 1,
                 xSize - 4, ySize - pObj->CaptionHeight - 3, 
                 Color);
    /* 绘制标题栏 */
    Color = pObj->Widget.Skin.CaptionColor[0];
    GUI_FillRect(x0 + 1, y0 + 1, xSize - 2,
                 pObj->CaptionHeight / 2, Color);
    Color = pObj->Widget.Skin.CaptionColor[1];
    GUI_FillRect(x0 + 1, y0 + pObj->CaptionHeight / 2 + 1,
                 xSize - 2, pObj->CaptionHeight / 2, Color);

    /* 绘制标题 */
    Color = pObj->Widget.Skin.FontColor[0];
    GUI_DspStringCurRectMiddle(x0 + 1, y0 + 1, xSize - 2,
                               pObj->CaptionHeight,
                               pObj->Title, Color, 
                               WIDGET_GetFont(pObj));
}

/* WINDOW设置焦点函数 */
static void WINDOW_SetFocus(GUI_MESSAGE *pMsg)
{
    WINDOW_Obj *pObj = pMsg->hWin;

    if (!pMsg->Param) { /* 设置下一个输入焦点 */
        if (pObj->hFocus == NULL || !((WM_Obj *)pObj->hFocus)->hNext) {
            pMsg->hWinSrc = ((WM_Obj *)pObj->hClient)->hFirstChild;
        } else {
            pMsg->hWinSrc = ((WM_Obj *)pObj->hFocus)->hNext;
        }
    } else if (pMsg->hWinSrc == NULL) { /* 直接设置输入焦点 */
        if (pObj->hFocus) {
            pMsg->hWinSrc = pObj->hFocus;
        } else {
            pMsg->hWinSrc = ((WM_Obj *)pObj->hClient)->hFirstChild;
        }
    }
    pObj->hFocus = WIDGET_SetFocus(pMsg);
}

/* WINDOW控件自动回调函数 */
static void __Callback(WM_MESSAGE *pMsg)
{
    WM_CALLBACK *cb;

    switch (pMsg->MsgId) {
    case WM_PAINT:
        __Paint(pMsg->hWin);
        break;
    case WM_DELETE:
        return;
    case WM_GET_CLIENT:
        pMsg->Param = (GUI_PARAM)__GetClient(pMsg->hWin);
        break;
    case WM_SET_FOCUS:
        WINDOW_SetFocus(pMsg); /* 设置焦点 */
        break;
    default:
        WM_DefaultProc(pMsg);
        cb = ((WINDOW_Obj*)pMsg->hWin)->UserCb;
        if (cb) {
            pMsg->hWin = pMsg->hWin;
            cb(pMsg);
        }
    }
}

/* 客户区自绘函数 */
static void __PaintClient(WM_HWIN hWin)
{
    GUI_RECT *pr = &((WM_Obj*)hWin)->Rect;
    WINDOW_Obj *pObj = WM_GetParentHandle(hWin);

    /* 绘制背景 */
    GUI_FillRect(pr->x0, pr->y0, pr->x1 - pr->x0 + 1,
        pr->y1 - pr->y0 + 1, pObj->Widget.Skin.BackColor[0]);
}

static void __ClientCallback(WM_MESSAGE *pMsg)
{
    WM_HWIN hParent;
    WM_CALLBACK *cb;

    switch (pMsg->MsgId) {
    case WM_PAINT:
        __PaintClient(pMsg->hWin);
        break;
    case WM_DELETE:
        return;
    case WM_SET_FOCUS:
        pMsg->hWin = WM_GetParentHandle(pMsg->hWin);
        WINDOW_SetFocus(pMsg); /* 设置焦点 */
        break;
    default:
        WM_DefaultProc(pMsg);
        hParent = WM_GetParentHandle(pMsg->hWin);
        cb = ((WINDOW_Obj*)hParent)->UserCb;
        if (cb) {
            pMsg->hWin = ((WM_Obj *)pMsg->hWin)->hParent;
            cb(pMsg);
        }
    }
}

static void __CreateClient(WINDOW_Obj *pObj)
{
    int xSize, ySize;
    GUI_RECT *pr = &pObj->Widget.Win.Rect;

    xSize = pr->x1 - pr->x0 - 5;
    ySize = pr->y1 - pr->y0 - pObj->CaptionHeight - 4;
    if (xSize < 0) {
        xSize = 0;
    }
    if (ySize < 0) {
        ySize = 0;
    }
    pObj->hClient = WM_CreateWindowAsChild(3, pObj->CaptionHeight + 2,
        (u_16)xSize, (u_16)ySize, pObj, 0, WIDGET_CLIENT, WM_NULL_ID,
        __ClientCallback, 0);
}

/*
 * 创建窗口控件
 * x0:WINDOW控件的最左像素(相对于父窗口)
 * y0:WINDOW控件的最右像素(相对于父窗口)
 * xSize:WINDOW控件的水平宽度
 * ySize:WINDOW控件的竖直高度
 * hParent:父窗口句柄
 * Id:窗口ID
 * Flag:窗口状态
 * cb:用户回调历程指针
 **/
WM_HWIN WINDOW_Create(i_16 x0,
                      i_16 y0,
                      u_16 xSize,
                      u_16 ySize,
                      WM_HWIN hParent,
                      u_16 Id,
                      u_8 Flag,
                      WM_CALLBACK *cb)
{
    WINDOW_Obj *pObj;
    
    GUI_LOCK();
    pObj = WM_CreateWindowAsChild(x0, y0, xSize, ySize, hParent, Flag,
        WIDGET_WINDOW, Id, __Callback, sizeof(WINDOW_Obj) - sizeof(WM_Obj));
    if (pObj == NULL) {
        GUI_UNLOCK();
        return NULL;
    }
    pObj->CaptionHeight = WINDOW_DEF_CAPHEIGHT;  /* 标题栏高度 */
    /* 配色 */
    pObj->Widget.Skin.CaptionColor[0] = WINDOW_CAPTION_UPC;  /* 标题栏上半部分 */
    pObj->Widget.Skin.CaptionColor[1] = WINDOW_CAPTION_DOWNC;/* 标题栏下半部分 */
    pObj->Widget.Skin.EdgeColor[0] = STD_WIN_RIM_OUTC;       /* 外线 */
    pObj->Widget.Skin.EdgeColor[1] = STD_WIN_RIM_MIDC;       /* 中线 */
    pObj->Widget.Skin.EdgeColor[2] = STD_WIN_RIM_INC;        /* 内线 */
    pObj->Widget.Skin.BackColor[0] = WINDOW_BODY_BKC;        /* 底色 */
    pObj->Widget.Skin.FontColor[0] = WINDOW_TITLE_COLOR;
    pObj->Widget.Skin.FontColor[1] = WINDOW_FONT_COLOR;
    pObj->UserCb = cb;
    pObj->hFocus = NULL;
    pObj->hClient = NULL;
    WINDOW_SetTitle(pObj, ""); /* 设置初始字符串 */
    WINDOW_SetFont(pObj, GUI_DEF_FONT);
    __CreateClient(pObj); /* 建立客户区 */
    WM_SendMessage(pObj->hClient, WM_CREATED, (GUI_PARAM)NULL);
    WM_SetFocusWindow(pObj);
    GUI_UNLOCK();
    return pObj;
}

/* WINDOW设置标题 */
GUI_RESULT WINDOW_SetTitle(WM_HWIN hWin, const char *str)
{
    /* 检测是否为WINDOW控件 */
    WIDGET_SignErrorReturn(hWin, WIDGET_WINDOW);
    ((WINDOW_Obj*)hWin)->Title = (char*)str;
    return GUI_OK;
}

/* WINDOW设置字体 */
GUI_RESULT WINDOW_SetFont(WM_HWIN hWin, GUI_FONT Font)
{
    /* 检测是否为WINDOW控件 */
    WIDGET_SignErrorReturn(hWin, WIDGET_WINDOW);
    WIDGET_SetFont(hWin, Font);
    return GUI_OK;
}

/* WINDOW设置为透明窗口 */
GUI_RESULT WINDOW_SetAllAlpha(WM_HWIN hWin, u_8 Alpha)
{
    WINDOW_Obj *pObj = hWin;
    
    /* 检测是否为WINDOW控件 */
    WIDGET_SignErrorReturn(hWin, WIDGET_WINDOW);
    /* 设置Alpha */
    WIDGET_Alpha(hWin, WIDGET_ALL, 0, Alpha);
    WIDGET_SetTransWindow(pObj->hClient);
    /* 窗口无效化 */
    WM_Invalidate(hWin);
    WM_Invalidate(pObj->hClient);
    return GUI_OK;
}
