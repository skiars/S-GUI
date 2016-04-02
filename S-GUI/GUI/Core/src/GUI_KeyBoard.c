#include "GUI_KeyBoard.h"
#include "GUI.h"

/*
*把键值发送到消息队列
*/
void GUI_SendKey(u_8 Key, u_8 Status)
{
    if (Status == GUI_KEYDOWN) { /* 按键按下 */
        WM_PostMessage(GUI_Context.hFocus, WM_KEYDOWN, Key);
    } else if (Status == GUI_KEYUP){ /* 按键松开 */
        WM_PostMessage(GUI_Context.hFocus, WM_KEYUP, Key);
    }
    
}

/* 按键处理 */
GUI_RESULT GUI_KeyMessageProc(GUI_MESSAGE *pMsg)
{
    if (pMsg->MsgId == WM_KEYUP) {
        GUI_LOCK();
        if (pMsg->hWin == GUI_Context.hFocus) {
            WM__SendMessage(pMsg->hWin, pMsg);
        }
        GUI_UNLOCK();
        return GUI_OK;
    }
    return GUI_ERR;
}
