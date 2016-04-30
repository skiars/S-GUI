#include "GUI_Core.h"
#include "GUI.h"

void *GUI_Heap[2];           /* 内存堆指针 */
GUI_WORK_SPACE *GUI_Data;    /* GUI工作数据 */
GUI_CONTEXT GUI_Context;     /* GUI上下文 */
static u_32 __LockTaskId;
static u_16 __TaskLockCnt;

/* GUI初始化 */
GUI_RESULT GUI_Init(void)
{
    int res;
    u_32 HeapSize;

    GUI_Heap[GUI_HEAP_FAST] = _GUI_GetHeapBuffer(GUI_HEAP_FAST, &HeapSize);
    res = GUI_HeapInit(GUI_Heap[GUI_HEAP_FAST], HeapSize);
    GUI_Heap[GUI_HEAP_HCAP] = _GUI_GetHeapBuffer(GUI_HEAP_HCAP, &HeapSize);
    res |= GUI_HeapInit(GUI_Heap[GUI_HEAP_HCAP], HeapSize);
    if (res) {
        return GUI_ERR;
    }
    GUI_Data = GUI_fastmalloc(sizeof(GUI_WORK_SPACE));
    if (GUI_Data == NULL) {
        return GUI_ERR;
    }
    GUI_Phy_Init(&GUI_Data->phy_info);
    GUI_InitOS();
    GUI_RectListInit(GUI_RECT_HEAP_SIZE); /* 注意内存是否足够 */
    if (GUI_MessageQueueInit() == GUI_ERR) {
        return GUI_ERR;
    }
    if (WM_Init() == GUI_ERR) {
        return GUI_ERR;
    }
    return GUI_OK;
}

/* 从内存中卸载GUI */
void GUI_Unload(void)
{
    WM_DeleteWindow(_hRootWin); /* 删除所有窗口 */
    GUI_MessageQueueDelete();   /* 删除消息队列 */
    GUI_fastfree(GUI_Data);     /* 删除GUI工作空间 */
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
void GUI_SetNowRectList(GUI_AREA l, GUI_RECT *p)
{
    GUI_Context.Area = l;
    GUI_Context.ClipRect = p;
}

/* 返回现在绘制区域的裁剪矩形链表 */
GUI_AREA GUI_GetNowRectList(void)
{
    return GUI_Context.Area;
}

/* 初始化绘制区域 */
void GUI_DrawAreaInit(GUI_RECT *p)
{
    if (GUI_CheckRectIntersect(GUI_Context.ClipRect, p)) {
        GUI_Context.pAreaNode = GUI_Context.Area;
    } else {
        GUI_Context.pAreaNode = NULL; /* 绘图区域与当前的有效绘制区域不相交 */
    }
}

/* 返回当前的裁剪矩形 */
GUI_RECT *GUI_GetNowArea(void)
{
    if (GUI_Context.pAreaNode) {
        return &GUI_Context.pAreaNode->Rect;
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
    if (GUI_Context.pAreaNode == NULL) {
        return 0;
    }
    *pRect = GUI_Context.pAreaNode->Rect;
    GUI_Context.pAreaNode = GUI_Context.pAreaNode->pNext;
    return 1;
}

u_8 GUI_GetNextAreaP(GUI_RECT ** p)
{
    if (GUI_Context.pAreaNode == NULL) {
        return 0;
    }
    *p = &GUI_Context.pAreaNode->Rect;
    GUI_Context.pAreaNode = GUI_Context.pAreaNode->pNext;
    return 1;
}

/* GUI调试输出 */
#if GUI_DEBUG_MODE
void GUI_DebugOut(const char *s)
{
    _GUI_DebugOut(s);
}
#endif
