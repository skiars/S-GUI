#include "GUI_Queue.h"
#include "GUI.h"

/*
 * 消息队列初始化
 * Capacity:队列容量
 * 返回值：一个已经初始化的队列指针
 */
GUI_QUEUE * GUI_QueueInit(u_16 Capacity)
{
    GUI_QUEUE *pQue = GUI_fastmalloc(sizeof(GUI_QUEUE));

    pQue->pArray = GUI_fastmalloc((u_32)Capacity * sizeof(GUI_MESSAGE));
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
        GUI_fastfree(pQue->pArray);
        GUI_fastfree(pQue);
    }
}

/*
 * 从消息队列里读取一条消息
 **/
GUI_RESULT GUI_GetMessageQueue(GUI_QUEUE *pQue, GUI_MESSAGE *pMsg)
{
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
 * 向消息队列发送一个消息
 * pQue:事件队列指针
 * pMsg:需要发送的消息
 **/
GUI_RESULT GUI_PostMessageQueue(GUI_QUEUE *pQue, GUI_MESSAGE *pMsg)
{
    if (pQue->size == pQue->Capacity - 1) { /* 队列已满 */
        return GUI_ERR;
    }
    ++pQue->size;
    if (++pQue->rear == pQue->Capacity) {  /* 队尾绕回到开头 */
        pQue->rear = 0;
    }
    pQue->pArray[pQue->rear] = *pMsg;
    return GUI_OK;
}
