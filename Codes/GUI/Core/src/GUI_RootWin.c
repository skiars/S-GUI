#include "GUI_RootWin.h"
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
        GUI_SetForeground(ROOTWINDOW_BACK_COLOR);
        GUI_FillRect(r->x0, r->y0, r->x1 - r->x0 + 1,
            r->y1 - r->y0 +1);
    }
}

static void _RootWin_Callback(WM_MESSAGE *pMsg)
{
    switch (pMsg->MsgId) {
    case WM_PAINT:
        __Paint(pMsg->hWin);
        break;
    case WM_TIMER:
        /* 用户函数 */
        if (RootWinTimer_Cb) {
            RootWinTimer_Cb(pMsg->hWin);
        }
        break;
    case WM_SET_FOCUS: /* 设置输入焦点 */
        break;
    case WM_GET_FOCUS:
        break;
    case WM_TP_CHECKED:
        WM_SetActiveWindow(pMsg->hWin); /* 设置为活动窗口 */
        break;
    }
}

void WM_RootWindowInit(GUI_HWIN hWin)
{
    int xSize, ySize;
    WM_Obj *pObj = hWin;
    
    GUI_ScreenSize(&xSize, &ySize);
    pObj->rect.x0 = 0;
    pObj->rect.y0 = 0;
    pObj->rect.x1 = xSize - 1;
    pObj->rect.y1 = ySize - 1;
    pObj->winCb = _RootWin_Callback;
    pObj->hNext = NULL;
    pObj->hNextLine = NULL;
    pObj->hParent = NULL;
    pObj->hFirstChild = NULL;
    pObj->status = 0x0000;
    pObj->id = WM_ROOTWIN_ID;
    WM_Invalidate(pObj);  /* 根窗口无效化 */
    GUI_ClipNewWindow(pObj); /* 更新剪切域 */
}

/* 设置根窗口的定时器 */
void GUI_SetRootWindowTimer(GUI_TIME timer)
{
    GUI_TimerCreate(gui_rootwin, 0, timer, GUI_TMR_AUTO);
}
