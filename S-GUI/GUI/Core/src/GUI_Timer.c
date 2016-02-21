#include "GUI_Timer.h"
#include "GUI.h"

static GUI_TIMER *_ListPtr = NULL;

static GUI_TIMER * _FindWindowInList(GUI_HWIN hWin)
{
    GUI_TIMER *pNode = _ListPtr;

    if (pNode) {
        while (pNode && pNode->hWin != hWin) {
            pNode = pNode->pNext;
        }
    }
    return pNode;
}

static void _DeleteWindowInList(GUI_HWIN hWin)
{
    if (_ListPtr) {
        GUI_TIMER *pNode = _ListPtr, *pLast = NULL;

        while (pNode && pNode->hWin != hWin ) {
            pLast = pNode;
            pNode = pNode->pNext;
        }
        if (pNode) { /* pNode不为NULL说明存在窗口 */
            if (pLast) { /* 窗口不在第1个节点 */
                pLast->pNext = pNode->pNext;
                GUI_fastfree(pNode);
            } else {
                GUI_fastfree(_ListPtr);
                _ListPtr = NULL;
            }
        }
    }
}

/* 窗口计时器
 * 窗口定时器可以让开启了定时功能的窗口在定时时间到以后产生一个WM_TIME_UPDATA消息
 **/
void GUI_TimerHandle(void)
{
    GUI_LOCK();
    if (_ListPtr) {
        GUI_TIMER *pNode;

        for (pNode = _ListPtr; pNode; pNode = pNode->pNext) {
            if (GUI_GetTime() >= pNode->LastTime + pNode->Interval) {
                WM_SendMessage(pNode->hWin, WM_TIME_UPDATA, (GUI_PARAM)NULL);
                pNode->LastTime = GUI_GetTime(); /* 重新获取时间 */
            }
        }
    }
    GUI_UNLOCK();
}

/* 设置窗口计时器,时间间隔单位为ms,如果Interval为0，就会删除该窗口的定时器 */
void GUI_SetWindowTimer(GUI_HWIN hWin, GUI_TIME Interval)
{
    if (hWin == NULL) {
        return;
    }
    GUI_LOCK();
    if (Interval == 0) {
        _DeleteWindowInList(hWin);
    } else {
        GUI_TIMER *pNode;

        pNode = _FindWindowInList(hWin);
        if (pNode) { /* 存在节点 */
            pNode->Interval = Interval;
        } else {
            /* 插入新节点 */
            pNode = _ListPtr;
            _ListPtr = GUI_fastmalloc(sizeof(GUI_TIMER));
            _ListPtr->hWin = hWin;
            _ListPtr->Interval = Interval;
            _ListPtr->LastTime = GUI_GetTime();
            _ListPtr->pNext = pNode;
        }
    }
    GUI_UNLOCK();
}

/* 返回窗口计数器时间间隔 */
GUI_TIME GUI_GetWindowTimer(GUI_HWIN hWin)
{
    GUI_TIMER *pNode;
    GUI_TIME Interval;
    
    if (hWin == NULL) {
        return 0;
    }
    GUI_LOCK();
    pNode = _FindWindowInList(hWin);
    if (pNode) {
        Interval = pNode->Interval;
    } else {
        Interval = 0;
    }
    GUI_UNLOCK();
    return Interval;
}
