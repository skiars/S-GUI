#include "RootWindow.h"
#include "GUI.h"

#define ROOTWINDOW_BACK_COLOR 0x00FFFFFF     /* 根窗口默认背景色 */

void (*RootWinPaint_Cb)(WM_hWin hWin) = NULL;
void (*RootWinTimer_Cb)(WM_hWin hWin) = NULL;

static void __Paint(WM_hWin hWin)
{
    GUI_RECT Rect = WM_GetWindowRect(hWin);

    if (RootWinPaint_Cb) {
        RootWinPaint_Cb(hWin);
    } else {
        /* 绘制背景 */
        GUI_FillRect(Rect.x0, Rect.y0, Rect.x1 - Rect.x0 + 1,
            Rect.y1 - Rect.y0 +1, ROOTWINDOW_BACK_COLOR);
    }
}

static void _RootWin_Callback(WM_MESSAGE *pMsg)
{
    switch (pMsg->MsgId) {
        case WM_PAINT : {
            __Paint(pMsg->hWin);
            break;
        }
        case WM_DELETE : {  /* 根窗口不可删除 */
            break;
        }
        case WM_TIME_UPDATA :
            /* 用户函数 */
            if (RootWinTimer_Cb) {
                RootWinTimer_Cb(pMsg->hWin);
            }
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
    pObj->UserRect = pObj->Rect;
    pObj->WinCb = _RootWin_Callback;
    pObj->hNext = NULL;
    pObj->hNextLine = NULL;
    pObj->hParent = NULL;
    pObj->hFirstChild = NULL;
    pObj->Style = 0x0000;
    pObj->Id = WM_ROOTWIN_ID;
    pObj->Sign = WIDGET_ROOTWIN;
    WM_Invalidate(pObj);  /* 根窗口无效化 */
    WM_SetWindowTimer(pObj, 1000); /* 需要窗口计时器 */
}
