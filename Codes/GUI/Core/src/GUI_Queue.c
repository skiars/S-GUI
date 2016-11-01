#include "GUI_Queue.h"
#include "GUI.h"

GUI_QUEUE *__MsgQueue;
static void *__msgLockPtr;

/*
 * 消息队列初始化
 * Capacity:队列容量
 * 返回值：一个已经初始化的队列指针
 */
GUI_QUEUE * GUI_QueueInit(u_16 Capacity)
{
    GUI_QUEUE *pQue = GUI_Malloc(sizeof(GUI_QUEUE));

    pQue->pArray = GUI_Malloc((u_32)Capacity * sizeof(GUI_MESSAGE));
    pQue->Capacity = Capacity;
    pQue->size = 0;
    pQue->front = 1;
    pQue->rear = 0;
    return pQue;
}

/* 删除队列 */
void GUI_QueueDelete(GUI_QUEUE *pQue)
{
    if (pQue) {
        GUI_Free(pQue->pArray);
        GUI_Free(pQue);
    }
}

/*
 * 从消息队列里读取一条消息
 **/
GUI_RESULT GUI_GetMessageQueue(GUI_QUEUE *pQue, GUI_MESSAGE *pMsg)
{
    if (!pQue) {
        return GUI_ERR;
    }
    GUI_LOCK();
    if (!pQue->size) {   /* 队列为空 */
        GUI_UNLOCK();
        return GUI_ERR;
    }
    --pQue->size;
    *pMsg = pQue->pArray[pQue->front];
    if (++pQue->front == pQue->Capacity) {  /* 队头绕回到开头 */
        pQue->front = 0;
    }
    GUI_UNLOCK();
    return GUI_OK;
}

/*
 * 向消息队列发送一条消息
 * pQue:事件队列指针
 * pMsg:需要发送的消息
 **/
GUI_RESULT GUI_PostMessageQueue(GUI_QUEUE *pQue, GUI_MESSAGE *pMsg)
{
    if (!pQue) {
        return GUI_ERR;
    } 
    if (pQue->size == pQue->Capacity - 1) { /* 队列已满 */
#if GUI_DEBUG_MODE
        GUI_DEBUG_OUT("GUI message queue is full.");
#endif
        return GUI_ERR;
    }
    ++pQue->size;
    if (++pQue->rear == pQue->Capacity) {  /* 队尾绕回到开头 */
        pQue->rear = 0;
    }
    pQue->pArray[pQue->rear] = *pMsg;
    return GUI_OK;
}

/* -------------------- GUI消息处理 -------------------- */
/* GUI消息队列初始化 */
GUI_RESULT GUI_MessageQueueInit(void)
{
    __msgLockPtr = GUI_TaskCreateLock(); /* 创建锁 */
    GUI_TaskLock(__msgLockPtr); /* 上锁 */
    __MsgQueue = GUI_QueueInit(GUI_MSG_QUEUE_SIZE);
    GUI_TaskUnlock(__msgLockPtr); /* 解锁 */
    if (__MsgQueue == NULL) {
#if GUI_DEBUG_MODE
        GUI_DEBUG_OUT("Failure to create message queue.");
#endif
        return GUI_ERR;
    }
    return GUI_OK;
}

/* 删除GUI消息队列 */
void GUI_MessageQueueDelete(void)
{
    GUI_TaskLock(__msgLockPtr); /* 上锁 */
    GUI_QueueDelete(__MsgQueue);
    __MsgQueue = NULL;
    GUI_TaskUnlock(__msgLockPtr); /* 解锁 */
}

/* 从GUI消息队列中读取一个消息 */
GUI_RESULT GUI_GetMessage(GUI_MESSAGE *pMsg)
{
    GUI_RESULT res;

    GUI_TaskLock(__msgLockPtr); /* 上锁 */
    res = GUI_GetMessageQueue(__MsgQueue, pMsg);
    GUI_TaskUnlock(__msgLockPtr); /* 解锁 */
    return res;
}

/* 向GUI消息队列发送一条消息 */
GUI_RESULT GUI_PostMessage(GUI_MESSAGE *pMsg)
{
    GUI_RESULT res;

    GUI_TaskLock(__msgLockPtr); /* 上锁 */
    res = GUI_PostMessageQueue(__MsgQueue, pMsg);
    GUI_TaskUnlock(__msgLockPtr); /* 解锁 */
    return res;
}
