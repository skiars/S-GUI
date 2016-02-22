#include "GUI_Core.h"
#include "GUI.h"

GUI_WORK_SPACE *GUI_Data;
static u_32 __LockTaskId;
static u_16 __TaskLockCnt;

/* GUI初始化 */
GUI_RESULT GUI_Init(void)
{    
    GUI_Data = GUI_fastmalloc(sizeof(GUI_WORK_SPACE));
    if (GUI_Data == NULL) {
        return GUI_ERR;
    }
    GUI_Phy_Init(&GUI_Data->phy_info);
    GUI_InitOS();
    GUI_RectListInit(GUI_RECT_HEAP_SIZE); /* 注意内存是否足够 */
    GUI_Data->MsgQueue = GUI_QueueInit(GUI_MSG_QUEUE_SIZE);
    if (GUI_Data->MsgQueue == NULL) {
        return GUI_ERR;
    }
    if (WM_Init() == GUI_ERR) {
        return GUI_ERR;
    }
#if GUI_USE_MEMORY
    GUI_Data->lcdbuf = GUI_malloc(sizeof(GUI_COLOR)
                                  * GUI_Data->phy_info.xSize
                                  * GUI_Data->phy_info.ySize);
#endif
    return GUI_OK;
}

/* 从内存中卸载GUI */
void GUI_Unload(void)
{
    WM_DeleteWindow(_hRootWin); /* 删除所有窗口 */
    GUI_QueueDelete(GUI_Data->MsgQueue); /* 删除消息队列 */
    GUI_fastfree(GUI_Data); /* 删除GUI工作空间 */
    GUI_Data = NULL;
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
    GUI_TIME t = GUI_GetTime();

    WM_Exec();
    t = GUI_GetTime() - t; /* 计算执行WM_Exec()的时间 */
    if (tms > t) {
        _GUI_Delay_ms(tms - t); /* 延时 */
    }
}

/* -------------------- GUI消息处理 -------------------- */
/* 从GUI消息队列中读取一个消息 */
GUI_RESULT GUI_GetMessage(GUI_MESSAGE *pMsg)
{
    GUI_RESULT res;

    GUI_LOCK();
    res = GUI_GetMessageQueue(GUI_Data->MsgQueue, pMsg);
    GUI_UNLOCK();
    return res;
}

/* 向GUI消息队列发送一条消息 */
GUI_RESULT GUI_PostMessage(GUI_MESSAGE *pMsg)
{
    GUI_RESULT res;

    GUI_LOCK();
    res = GUI_PostMessageQueue(GUI_Data->MsgQueue, pMsg);
    GUI_UNLOCK();
    return res;
}

/* -------------------- GUI任务锁 -------------------- */

/* GUI上锁 */
void GUI_LOCK(void)
{
    /* 还没有上锁或不是已经上锁的任务上锁 */
    if (!__TaskLockCnt || __LockTaskId != GUI_GetTaskId()) {
        GUI_TaskLock();
        __LockTaskId = GUI_GetTaskId();
    }
    ++__TaskLockCnt;
}

/* GUI解锁 */
void GUI_UNLOCK(void)
{
    if (--__TaskLockCnt == 0) {
        GUI_TaskUnlock();
    }
}

/* -------------------- GUI矩形绘制 -------------------- */
/* 设置现在绘制区域的裁剪矩形链表 */
void GUI_SetNowRectList(RECT_LIST l, GUI_RECT *p)
{
    GUI_Data->NowRectList = l;
    GUI_Data->PaintArea = p;
}

/* 返回现在绘制区域的裁剪矩形链表 */
RECT_LIST GUI_GetNowRectList(void)
{
    return GUI_Data->NowRectList;
}

static RECT_NODE *__NowRectNode = NULL;

/* 初始化绘制区域 */
void GUI_DrawAreaInit(GUI_RECT *p)
{
    if (GUI_CheckRectIntersect(GUI_Data->PaintArea, p)) {
        __NowRectNode = GUI_Data->NowRectList;
    } else {
        __NowRectNode = NULL; /* 绘图区域与当前的有效绘制区域不相交 */
    }
}

/* 返回当前的裁剪矩形 */
GUI_RECT *GUI_GetNowArea(void)
{
    if (__NowRectNode) {
        return &__NowRectNode->Rect;
    }
    return NULL;
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

/* GUI调试输出 */
#if GUI_DEBUG_MODE
void GUI_DebugOut(const char *s)
{
    _GUI_DebugOut(s);
}
#endif
