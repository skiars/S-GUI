#include "GUI_KeyBoard.h"
#include "GUI.h"

/*
*把键值发送到消息队列
*/
void GUI_SendKey(u_8 Key, u_8 Status)
{
    if (Status == GUI_KEYDOWN) { /* 按键按下 */
        WM_PostMessage(WM_GetActiveWindow(), WM_KEYDOWN, Key);
    } else if (Status == GUI_KEYUP){ /* 按键松开 */
        WM_PostMessage(WM_GetActiveWindow(), WM_KEYUP, Key);
    }
}

/* 按键处理 */
GUI_RESULT GUI_KeyMessageProc(GUI_MESSAGE *pMsg)
{
    if (pMsg->MsgId == WM_KEYUP) {
        GUI_LOCK();
        /* 当窗口还是活动窗口时传递键值 */
        if (pMsg->hWin == WM_GetActiveWindow()) {
            WM__SendMessage(pMsg->hWin, pMsg);
        }
        GUI_UNLOCK();
        return GUI_OK;
    }
    return GUI_ERR;
}
