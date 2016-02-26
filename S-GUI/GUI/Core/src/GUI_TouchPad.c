#include "GUI_TouchPad.h"
#include "GUI.h"

/* 设置触摸板状态 */
void GUI_TouchPadSendValue(i_16 x, i_16 y, u_16 State)
{
    static u_16 LastState = GUI_TP_REMOVED;

    /*
     * 触摸板当前状态为按下或上一次的状态为按下。
     * 也就是说只会向GUI消息队列发送一次触摸松开消息。
     **/
    if (State == GUI_TP_CHECKED || LastState == GUI_TP_CHECKED) {
        u_32 Point;
        GUI_MESSAGE Msg;

        LastState = State;
        Point = ((u_32)x << 16) | y;
        /* 填充消息结构体 */
        Msg.hWin = NULL;
        Msg.hWinSrc = NULL;
        if (State == GUI_TP_CHECKED) {
            Msg.MsgId = WM_TP_CHECKED;
        } else {
            Msg.MsgId = WM_TP_REMOVED;
        }
        Msg.Param = (GUI_PARAM)Point;
        GUI_PostMessage(&Msg); /* 发送消息到GUI消息队列 */
    }
}

/* 触摸板消息处理 */
GUI_RESULT GUI_TouchPadMessageProc(GUI_MESSAGE *pMsg)
{
    static GUI_HWIN _CurWin = NULL;
    static GUI_POINT _CurPos;
    
    /* 检查是否是WM触摸板消息 */
    if (pMsg->MsgId == WM_TP_CHECKED || pMsg->MsgId == WM_TP_REMOVED) {
        GUI_HWIN hWin;
        GUI_POINT Point;

        GUI_LOCK();
        Point.x = (u_32)pMsg->Param >> 16;
        Point.y = (u_32)pMsg->Param & 0xffff;
        if (pMsg->MsgId == WM_TP_CHECKED) {
            hWin = WM_GetExposedWindow(Point.x, Point.y);
            /* 判断是否还是上一次的窗口 */
            if (hWin != NULL) {
                GUI_POINT Pos[2];

                /* 当前坐标 */
                Pos[0].x = Point.x;
                Pos[0].y = Point.x;
                /* 坐标偏移 */
                Pos[1].x = Point.x - _CurPos.x;
                Pos[1].y = Point.y - _CurPos.y;
                if (_CurWin == NULL) { /* 第一次触摸 */
                    _CurWin = hWin;
                    WM_SendMessage(hWin, WM_TP_CHECKED, (GUI_PARAM)&Point);
                } else if (hWin == _CurWin) { /* 一直在触摸 */
                    WM_SendMessage(hWin, WM_TP_PRESS, (GUI_PARAM)&Pos);
                } else if (WM_FindWindow(_CurWin) == GUI_OK) {
                    /* 离开了控件且窗口存在 */
                    WM_SendMessage(_CurWin, WM_TP_LEAVE, (GUI_PARAM)&Pos);
                }
                _CurPos = Point;
            }
        } else {  /* 触摸松开 */
            if (WM_FindWindow(_CurWin) == GUI_OK) {  /* 窗口还存在 */
                WM_SendMessage(_CurWin, WM_TP_REMOVED, (GUI_PARAM)NULL);
            }
            _CurWin = NULL;
        }
        GUI_UNLOCK();
        return GUI_OK;
    }
    return GUI_ERR;
}
