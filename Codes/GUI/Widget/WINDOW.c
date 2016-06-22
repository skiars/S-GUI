#include "WINDOW.h"
#include "GUI.h"

#define WINDOW_DEF_CAPHEIGHT   20
/* 默认的窗体caption背景色caption的颜色. */
/* 以及窗体背景色. */
#define WINDOW_CAPTION_COLOR1       0x0042789B  /* 窗口CAPTION填充色 */
#define WINDOW_CAPTION_COLOR2       0x00FFFFFF  /* 窗口非活动颜色 */
#define WINDOW_TITLE_COLOR1         0x00FFFFFF  /* 窗口标题颜色 */
#define WINDOW_TITLE_COLOR2         0x00909090  /* 窗口标题颜色 */
#define WINDOW_BODY_BKC             0x00FFFFFF  /* 窗口底色 */

/* 标准窗体边框颜色定义 */
#define WINDOW_EDGE_COLOR           0x002A3033  /* 边线颜色 */

static WM_HWIN __GetClient(WINDOW_Obj *pObj)
{
    return pObj->hClient;
}

/* Window自绘函数 */
static void __Paint(WM_HWIN hWin)
{
    u_16 xSize, ySize;
    GUI_RECT Rect;
    WINDOW_Obj *pObj = hWin;

    GUI_GetClientRect(&Rect);
    xSize = Rect.x1 + 1;
    ySize = Rect.y1 + 1;
    
    /* 绘制标题栏 */
    if (pObj != WM_GetActiveWindow()) {
        GUI_SetFGColor(pObj->Widget.Skin.CaptionColor[1]);
        GUI_SetFontColor(pObj->Widget.Skin.FontColor[1]);
    } else {
        GUI_SetFGColor(pObj->Widget.Skin.CaptionColor[0]);
        GUI_SetFontColor(pObj->Widget.Skin.FontColor[0]);
    }
    GUI_FillRect(1, 1, xSize - 2, pObj->CaptionHeight - 1);
    /* 绘制标题 */
    GUI_SetFont(WIDGET_GetFont(pObj));
    GUI_Val2Rect(&Rect, 2, 1, xSize - 4, pObj->CaptionHeight);
    GUI_DispStringInRect(&Rect, pObj->Title, GUI_ALIGN_VCENTER); /* 垂直居中 */
    /* 绘制边框 */
    if (pObj != WM_GetActiveWindow()) {
        GUI_SetFGColor(pObj->Widget.Skin.FontColor[1]);
    } else {
        GUI_SetFGColor(pObj->Widget.Skin.CaptionColor[0]);
    }
    GUI_DrawRect(0, 0, xSize, ySize);
}

/* WINDOW设置焦点函数 */
static void WINDOW_SetFocus(GUI_MESSAGE *pMsg)
{
    WINDOW_Obj *pObj = pMsg->hWin;

    if (pMsg->hWinSrc != pObj && pMsg->hWinSrc != pObj->hClient) {
        pObj->hFocus = pMsg->hWinSrc;
    }
}

/* WINDOW控件自动回调函数 */
static void __Callback(WM_MESSAGE *pMsg)
{
    WM_CALLBACK *UserCb = ((WINDOW_Obj*)pMsg->hWin)->UserCb;

    switch (pMsg->MsgId) {
    case WM_PAINT:
        WIDGET_Paint(pMsg->hWin);
        break;
    case WM_GET_CLIENT:
        pMsg->Param = (GUI_PARAM)__GetClient(pMsg->hWin);
        break;
    case WM_SET_FOCUS:
        WINDOW_SetFocus(pMsg); /* 设置焦点 */
        WM_Invalidate(((WINDOW_Obj*)pMsg->hWin)->hBtn);
        break;
    case WM_GET_FOCUS:
        pMsg->hWin = ((WINDOW_Obj*)pMsg->hWin)->hFocus;
        break;
    case WM_KILL_FOCUS:
        WM_SendMessage(((WINDOW_Obj*)pMsg->hWin)->hFocus, WM_KILL_FOCUS, 0);
        WM_SendMessage(((WINDOW_Obj*)pMsg->hWin)->hBtn, WM_KILL_FOCUS, 0);
        WM_Invalidate(pMsg->hWin);
        if (UserCb) {
            UserCb(pMsg);
        }
        break;
    case WM_KEYDOWN:
        WM_SendMessage(((WINDOW_Obj*)pMsg->hWin)->hFocus,
            WM_KEYDOWN, pMsg->Param);
        if (UserCb) {
            UserCb(pMsg);
        }
        break;
    case WM_KEYUP:
        WM_SendMessage(((WINDOW_Obj*)pMsg->hWin)->hFocus,
            WM_KEYUP, pMsg->Param);
        if (UserCb) {
            UserCb(pMsg);
        }
        break;
    case WM_BUTTON_CLICKED:
        if (pMsg->hWinSrc != ((WINDOW_Obj*)pMsg->hWin)->hBtn && UserCb) {
            UserCb(pMsg);
        }
        break;
    case WM_BUTTON_RELEASED:
        if (pMsg->hWinSrc == ((WINDOW_Obj*)pMsg->hWin)->hBtn) {
            WM_DeleteWindow(pMsg->hWin);
        } else if (UserCb) {
            UserCb(pMsg);
        }
        break;
    default:
        UserCb = ((WINDOW_Obj*)pMsg->hWin)->UserCb;
        if (UserCb) {
            UserCb(pMsg);
        }
        WM_DefaultProc(pMsg);
    }
}

/* 客户区自绘函数 */
static void __PaintClient(WM_HWIN hWin)
{
    GUI_RECT Rect;
    WINDOW_Obj *pObj = WM_GetParentHandle(hWin);

    GUI_GetClientRect(&Rect);
    /* 绘制背景 */
    GUI_SetFGColor(pObj->Widget.Skin.BackColor[0]);
    GUI_FillRect(0, 0, Rect.x1 + 1, Rect.y1 + 1);
}

static void __ClientCallback(WM_MESSAGE *pMsg)
{
    WM_HWIN hParent;
    WM_CALLBACK *UserCb;

    hParent = WM_GetParentHandle(pMsg->hWin);
    switch (pMsg->MsgId) {
    case WM_PAINT:
        __PaintClient(pMsg->hWin);
        break;
    case WM_DELETE:
        break;
    case WM_SET_FOCUS:
        pMsg->hWin = hParent;
        WINDOW_SetFocus(pMsg); /* 设置焦点 */
        break;
    case WM_TP_PRESS: /* 不移动窗口 */
        break;
    case WM_TP_LEAVE: /* 不移动窗口 */
        break;
    default:
        UserCb = ((WINDOW_Obj*)hParent)->UserCb;
        if (UserCb) {
            pMsg->hWin = hParent;
            UserCb(pMsg);
        }
        WM_DefaultProc(pMsg);
    }
}

/* 关闭按钮自绘函数 */
static void __BtnPaint(GUI_HWIN hWin)
{
    GUI_RECT Rect;

    GUI_SetFontColor(((WIDGET *)hWin)->Skin.FontColor[0]);
    if (WM_GetActiveWindow() != WM_GetDsektopWindow(hWin)) {
        GUI_SetFGColor(((WIDGET *)hWin)->Skin.BackColor[1]);
        GUI_SetFontColor(((WIDGET *)hWin)->Skin.FontColor[1]);
    } else if (BUTTON_GetStatus(hWin)) { /* 按下 */
        GUI_SetFGColor(((WIDGET *)hWin)->Skin.BackColor[2]);
    } else {
        GUI_SetFGColor(((WIDGET *)hWin)->Skin.BackColor[0]);
    }
    GUI_GetClientRect(&Rect);
    /* 绘制背景 */
    GUI_FillRect(0, 0, Rect.x1 + 1, Rect.y1 + 1);
    GUI_DispStringInRect(&Rect, "X", GUI_ALIGN_VCENTER | GUI_ALIGN_HCENTER);
}

static void __CreateClient(WINDOW_Obj *pObj)
{
    u_16 xSize, ySize;
    GUI_RECT *pr = &pObj->Widget.Win.Rect;

    xSize = pr->x1 - pr->x0 - 1;
    ySize = pr->y1 - pr->y0 - pObj->CaptionHeight;
    if (xSize < 0) {
        xSize = 0;
    }
    if (ySize < 0) {
        ySize = 0;
    }
    pObj->hClient = WM_CreateWindowAsChild(1, pObj->CaptionHeight,
        xSize, ySize, pObj, 0, WM_NULL_ID, __ClientCallback, 0);
}

/* 关闭按钮 */
void __CreateBtn(WINDOW_Obj *pObj)
{
    u_16 xSize, ySize;
    GUI_RECT *r = &pObj->Widget.Win.Rect;

    xSize = r->x1 - r->x0 + 1;
    ySize = pObj->CaptionHeight - 2;
    pObj->hBtn = BUTTON_Create(xSize - ySize * 4 / 3 - 1,
        1, ySize * 4 / 3, ySize, pObj, 0, 0);
    WIDGET_SetPaintFunction(pObj->hBtn, __BtnPaint);
    ((WIDGET *)pObj->hBtn)->Skin.BackColor[0] = WINDOW_CAPTION_COLOR1;
    ((WIDGET *)pObj->hBtn)->Skin.BackColor[1] = WINDOW_CAPTION_COLOR2;
    ((WIDGET *)pObj->hBtn)->Skin.BackColor[2] = 0x00D04040;
    ((WIDGET *)pObj->hBtn)->Skin.FontColor[0] = WINDOW_TITLE_COLOR1;
    ((WIDGET *)pObj->hBtn)->Skin.FontColor[1] = WINDOW_TITLE_COLOR2;
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
    u_8 Style,
    WM_CALLBACK *cb)
{
    WINDOW_Obj *pObj;
    
    GUI_LOCK();
    pObj = WM_CreateWindowAsChild(x0, y0, xSize, ySize, hParent,
        Style, Id, __Callback, sizeof(WINDOW_Obj) - sizeof(WM_Obj));
    if (pObj == NULL) {
        GUI_UNLOCK();
        return NULL;
    }
    pObj->CaptionHeight = WINDOW_DEF_CAPHEIGHT;  /* 标题栏高度 */
    /* 配色 */
    pObj->Widget.Skin.CaptionColor[0] = WINDOW_CAPTION_COLOR1;  /* 标题栏 */
    pObj->Widget.Skin.CaptionColor[1] = WINDOW_CAPTION_COLOR2;
    pObj->Widget.Skin.EdgeColor[0] = WINDOW_EDGE_COLOR;        /* 边线 */
    pObj->Widget.Skin.BackColor[0] = WINDOW_BODY_BKC;          /* 底色 */
    pObj->Widget.Skin.FontColor[0] = WINDOW_TITLE_COLOR1;
    pObj->Widget.Skin.FontColor[1] = WINDOW_TITLE_COLOR2;
    pObj->UserCb = cb;
    WIDGET_SetPaintFunction(pObj, __Paint);
    WINDOW_SetTitle(pObj, ""); /* 设置初始字符串 */
    WINDOW_SetFont(pObj, &GUI_DEF_FONT);
    __CreateClient(pObj); /* 建立客户区 */
    __CreateBtn(pObj);
    WM_SendMessage(pObj, WM_CREATED, (GUI_PARAM)NULL);
    /* 设置输入焦点 */
    pObj->hFocus = ((WM_Obj *)pObj->hClient)->hFirstChild;
    GUI_UNLOCK();
    return pObj;
}

/* WINDOW设置标题 */
GUI_RESULT WINDOW_SetTitle(WM_HWIN hWin, const char *str)
{
    if (hWin) {
        ((WINDOW_Obj*)hWin)->Title = (char*)str;
        return GUI_OK;
    }
    return GUI_ERR;
}

/* WINDOW设置字体 */
GUI_RESULT WINDOW_SetFont(WM_HWIN hWin, GUI_FONT *Font)
{
    WIDGET_SetFont(hWin, Font);
    return GUI_OK;
}

/* WINDOW设置为透明窗口 */
GUI_RESULT WINDOW_SetAllAlpha(WM_HWIN hWin, u_8 Alpha)
{
    WINDOW_Obj *pObj = hWin;
    
    /* 设置Alpha */
    WIDGET_Alpha(hWin, WIDGET_ALL, 0, Alpha);
    WM_SetTransWindow(pObj->hClient, 1);
    return GUI_OK;
}
