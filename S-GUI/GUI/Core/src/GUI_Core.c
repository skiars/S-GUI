#include "GUI_Core.h"
#include "GUI.h"

GUI_WORK_SPACE *GUI_Data;
GUI_RECT __Now_DrawRect;  //GUI当前可以绘制的区域

void GUI_Init(void)
{    
    GUI_Data = GUI_fastmalloc(sizeof(GUI_WORK_SPACE));
    if (GUI_Data == NULL) {
        return;
    }
    GUI_Phy_Init(&GUI_Data->phy_info);
    GUI_InitOS();
    GUI_RectListInit(GUI_RECT_HEAP_SIZE); /* 注意内存是否足够 */
    GUI_KeyBufferInit(); /* 按键处理函数初始化 */
    WM_Init();
#if GUI_USE_MEMORY
    GUI_Data->lcdbuf = GUI_malloc(sizeof(GUI_COLOR)
                                 * GUI_Data->phy_info.xSize
                                 * GUI_Data->phy_info.ySize);
#endif
    GUI_Data->guitime=0;
}

/* 从内存中卸载GUI */
void GUI_Unload(void)
{
    WM_DeleteWindow(_hRootWin); /* 删除所有窗口 */
    GUI_KeyBufferDelete();      /* 删除按键队列 */
    GUI_fastfree(GUI_Data);     /* 删除GUI工作空间 */
}

/* 获取屏幕尺寸 */
void GUI_ScreenSize(u_16 *xSize, u_16 *ySize)
{
    *xSize = GUI_Data->phy_info.xSize;
    *ySize = GUI_Data->phy_info.ySize;
}

/* 获取屏幕宽度 */
u_16 GUI_GetScreenWidth(void)
{
    return GUI_Data->phy_info.xSize;
}

/* 获取屏幕高度 */
u_16 GUI_GetScreenHeight(void)
{
    return GUI_Data->phy_info.ySize;
}

/* GUI延时并更新 */
void GUI_Delay(GUI_TIME tms)
{
    GUI_KeyProcess();
    WM_Exec();  //窗口管理器
    _GUI_Delay_ms(tms); //延时
}

/*
 * 事件队列初始化
 * capacity:队列容量
 * ItemSize:每个结构单元的字节数
 * 返回值：一个已经初始化的队列指针
 */
GUI_QUEUE* GUI_EventQueueInit(u_16 capacity, u_16 ItemSize)
{
    GUI_QUEUE *pQue = GUI_fastmalloc(sizeof(GUI_QUEUE));

    pQue->pArray = GUI_fastmalloc((u_32)capacity * ItemSize);
    pQue->capacity = capacity;
    pQue->ItemSize = ItemSize;
    pQue->size = 0;
    pQue->front = 1;
    pQue->rear = 0;
    return pQue;
}

/* 删除队列 */
void GUI_EventQueueDelete(GUI_QUEUE *pQue)
{
    if (pQue) {
        GUI_fastfree(pQue->pArray);
        GUI_fastfree(pQue);
    }
}

/*
*从事件队列里读取一个事件
*/
u_8 GUI_GetEvent(GUI_QUEUE *pQue, GUI_EVENT event)
{
    u_16 i = pQue->ItemSize;
    u_8 *p1, *p2;
    
    if (!pQue->size) {   //队列为空
        return GUI_ERR;
    }
    pQue->size--;
    p1 = (u_8*)event;
    p2 = (u_8*)pQue->pArray + (u_32)pQue->front * i;
    while(i--) {
        *p1++ = *p2++;
    }
    if (++pQue->front == pQue->capacity) {  //队头绕回到开头
        pQue->front = 0;
    }
    return GUI_OK;
}

/*
*向事件队列发送一个事件
*pQue:事件队列指针
*event:需要发送的事件
*/
u_8 GUI_SendEvent(GUI_QUEUE *pQue, GUI_EVENT event)
{
    u_16 i = pQue->ItemSize;
    u_8 *p1, *p2;
    
    if (pQue->size == pQue->capacity - 1) { //队列已满
        return GUI_ERR;
    }
    pQue->size++;
    if (++pQue->rear == pQue->capacity) {  //队尾绕回到开头
        pQue->rear = 0;
    }
    p1 = (u_8*)pQue->pArray + (u_32)pQue->rear * i;
    p2 = (u_8*)event;
    while(i--) {
        *p1++ = *p2++;
    }
    return GUI_OK;
}

/*
*清空事件队列
*/
void GUI_CleanQueue(GUI_QUEUE *pQue)
{
    pQue->size = 0;
    pQue->front = 1;
    pQue->rear = 0;
}

/*
*检测事件队列是否为空
*/
u_8 GUI_QueueIsEmpty(GUI_QUEUE *pQue)
{
    if (!pQue->size) {   //队列为空
        return 1;
    }
    return 0;
}

/* 设置现在绘制区域的裁剪矩形链表 */
void GUI_SetNowRectList(RECT_LIST l)
{
    GUI_Data->NowRectList = l;
}

/* 返回现在绘制区域的裁剪矩形链表 */
RECT_LIST GUI_GetNowRectList(void)
{
    return GUI_Data->NowRectList;
}

static RECT_NODE *__NowRectNode = NULL;

/* 初始化绘制区域 */
void GUI_DrawAreaInit(void)
{
    __NowRectNode = GUI_Data->NowRectList;
}

/* 返回当前的裁剪矩形 */
GUI_RECT *GUI_GetNowArea(void)
{
    return &__NowRectNode->Rect;
}

/* 获取下一个裁剪矩形
 * pRect:用于保存返回的裁剪矩形
 * 返回值:0：当前绘制区域的裁剪矩形已经获取完，主调函数在GUI_GetNextArea
 *          返回0时应该结束当前图形在裁剪矩形链表里的绘制循环。
 *       1：当前绘制区域的还有没有绘制的裁剪矩形，主调函数在GUI_GetNextArea
 *          返回0时应该继续当前图形在裁剪矩形链表里的绘制循环。
 **/
u_8 GUI_GetNextArea(GUI_RECT *pRect)
{    
    if (__NowRectNode == NULL) {
        return 0;
    }
    *pRect = __NowRectNode->Rect;
    __NowRectNode = __NowRectNode->pNext;
    return 1;
}
