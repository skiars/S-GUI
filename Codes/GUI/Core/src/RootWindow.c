#include "RootWindow.h"
#include "GUI.h"

#define ROOTWINDOW_BACK_COLOR 0x00FFFFFF     /* 根窗口默认背景色 */

void (*RootWinPaint_Cb)(WM_HWIN hWin) = NULL;
void (*RootWinTimer_Cb)(WM_HWIN hWin) = NULL;

static void __Paint(WM_HWIN hWin)
{
    GUI_RECT *r = WM_GetWindowRect(hWin);

    if (RootWinPaint_Cb) {
        RootWinPaint_Cb(hWin);
    } else {
        /* 绘制背景 */
        GUI_FillRect(r->x0, r->y0, r->x1 - r->x0 + 1,
            r->y1 - r->y0 +1, ROOTWINDOW_BACK_COLOR);
    }
}

static void _RootWin_Callback(WM_MESSAGE *pMsg)
{
    switch (pMsg->MsgId) {
    case WM_PAINT:
        __Paint(pMsg->hWin);
        break;
    case WM_TIME_UPDATA:
        /* 用户函数 */
        if (RootWinTimer_Cb) {
            RootWinTimer_Cb(pMsg->hWin);
        }
        break;
    case WM_SET_FOCUS: /* 设置输入焦点 */
        pMsg->hWinSrc = pMsg->hWin;
        WIDGET_SetFocus(pMsg);
        break;
    case WM_TP_CHECKED: /* 直接设置输入焦点 */
        pMsg->hWinSrc = pMsg->hWin;
        WIDGET_SetFocus(pMsg);
        break;
    }
}

void WM_RootWindowInit(WM_Obj *pObj)
{
    u_16 xSize, ySize;
    
    GUI_ScreenSize(&xSize, &ySize);
    pObj->Rect.x0 = 0;
    pObj->Rect.y0 = 0;
    pObj->Rect.x1 = xSize - 1;
    pObj->Rect.y1 = ySize - 1;
    pObj->WinCb = _RootWin_Callback;
    pObj->hNext = NULL;
    pObj->hNextLine = NULL;
    pObj->hParent = NULL;
    pObj->hFirstChild = NULL;
    pObj->Status = 0x0000;
    pObj->Id = WM_ROOTWIN_ID;
    WM_Invalidate(pObj);  /* 根窗口无效化 */
    GUI_ClipNewWindow(pObj); /* 更新剪切域 */
}

/* 设置根窗口的定时器 */
void GUI_SetRootWindowTimer(GUI_TIME timer)
{
    GUI_SetWindowTimer(_hRootWin, timer);
}
