#include "GUI_Timer.h"
#include "GUI.h"

static GUI_TIMER *__TimerList = NULL;

/* 在定时器链表中寻找当前节点
 @ 存在当前节点就返回结节点指针, 否则返回NULL.
 */
static GUI_TIMER * _FindInList(GUI_HTMR hTimer)
{
    GUI_TIMER *pNode = __TimerList;

    if (pNode) {
        while (pNode && pNode != hTimer) {
            pNode = pNode->pNext;
        }
    }
    return pNode;
}

/* 定时器更新事件 */
static void __TimerEvent(GUI_TIMER *p)
{
    int per = p->Period;
    GUI_TIME t = GUI_GetTime();

    if (per && t >= p->SetTime) {
        if (p->Mode == GUI_TMR_ONE) { /* 单次触发 */
            p->Period = 0;
        } else { /* 自动重载 */
            /* 计算下一次定时器更新的时间 */
            p->SetTime = t + (per - (t - p->SetTime) % per);
        }
        WM_SendMessage(p->hWin, WM_TIMER, (u_32)p);
    }
}

/* 窗口计时器
 * 窗口定时器可以让开启了定时功能的窗口在定时时间到以后产生一个WM_TIMER消息
 **/
void GUI_TimerHandler(void)
{
    GUI_TIMER *pNode, *pNext;

    GUI_LOCK();
    for (pNode = __TimerList; pNode; pNode = pNext) {
        /* 保存pNext的值以防止在回调函数中删除定时器后出现错误 */
        pNext = pNode->pNext;
        __TimerEvent(pNode);
    }
    GUI_UNLOCK();
}

/* 创建定时器 */
GUI_HWIN GUI_TimerCreate(GUI_HWIN hWin, int Id, int Period, u_8 Mode)
{
    GUI_TIMER *pNode;

    GUI_LOCK();
    /* 插入新节点到链表头 */
    pNode = __TimerList;
    __TimerList = GUI_Malloc(sizeof(GUI_TIMER));
    __TimerList->hWin = hWin;
    __TimerList->Id = Id;
    __TimerList->Period = Period;
    __TimerList->SetTime = GUI_GetTime() + Period;
    __TimerList->Mode = Mode;
    __TimerList->pNext = pNode;
    GUI_UNLOCK();
    return pNode;
}

/* 删除定时器 */
void GUI_TimerDetete(GUI_HTMR hTimer)
{
    GUI_TIMER *pNode = __TimerList, *pLast = NULL;

    GUI_LOCK();
    while (pNode && pNode != hTimer) {
        pLast = pNode;
        pNode = pNode->pNext;
    }
    if (pNode) { /* pNode不为NULL说明存在节点 */
        if (pLast) { /* 不在第1个节点 */
            pLast->pNext = pNode->pNext;
        } else { /* 删除第一个节点 */
            __TimerList = pNode->pNext;
        }
        GUI_Free(pNode);
    }
    GUI_UNLOCK();
}

/* 删除某个窗口所有的定时器 */
void GUI_TimerDeleteAtWindow(GUI_HWIN hWin)
{
    GUI_TIMER *pNode = __TimerList, *pLast = NULL, *pNext;

    GUI_LOCK();
    while (pNode) {
        pNext = pNode->pNext;
        if (pNode->hWin == hWin) {
            if (pLast) { /* 不在第1个节点 */
                pLast->pNext = pNext;
            } else { /* 删除第一个节点 */
                __TimerList = pNext;
            }
            GUI_Free(pNode);
        } else {
            pLast = pNode;
        }
        pNode = pNext;
    }
    GUI_UNLOCK();
}

/* 获取定时器ID */
u_16 GUI_GetTimerId(GUI_HTMR hTimer)
{
    u_16 Id = 0;

    GUI_LOCK();
    if (_FindInList(hTimer)) {
        Id = ((GUI_TIMER *)hTimer)->Id;
    }
    GUI_UNLOCK();
    return Id;
}

/* 重置定时器 */
void GUI_ResetTimer(GUI_HTMR hTimer, int Period)
{
    GUI_LOCK();
    if (_FindInList(hTimer)) {
        ((GUI_TIMER *)hTimer)->Period = Period;
        ((GUI_TIMER *)hTimer)->SetTime = GUI_GetTime() + Period;
    }
    GUI_UNLOCK();
}
