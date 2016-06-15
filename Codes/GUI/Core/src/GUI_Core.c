#include "GUI_Core.h"
#include "GUI.h"

void *GUI_Heap[2];           /* 内存堆指针 */
GUI_HWIN GUI_RootWin;        /* 根窗口 */
GUI_AREA GUI_AreaHeap;       /* 裁剪区域堆 */
GUI_CONTEXT GUI_Context;     /* GUI上下文 */
static u_32 __LockTaskId;
static u_16 __TaskLockCnt;

/* GUI初始化 */
GUI_RESULT GUI_Init(void)
{
    u_32 HeapSize;

    /* 内存管理初始化 */
    GUI_Heap[GUI_HEAP_FAST] = _GUI_GetHeapBuffer(GUI_HEAP_FAST, &HeapSize);
    if (GUI_HeapInit(GUI_Heap[GUI_HEAP_FAST], HeapSize) == GUI_ERR) {
        return GUI_ERR;
    }
    GUI_Heap[GUI_HEAP_HCAP] = _GUI_GetHeapBuffer(GUI_HEAP_HCAP, &HeapSize);
    if (GUI_HeapInit(GUI_Heap[GUI_HEAP_HCAP], HeapSize) == GUI_ERR) {
        return GUI_ERR;
    }
    /* 初始化操作系统相关代码 */
    GUI_InitOS();
    /* 初始化图形硬件 */
    GUI_DeviceInit();
    /* 初始化窗口剪切域裁剪私有堆 */
    if (GUI_RectListInit(GUI_RECT_HEAP_SIZE) == GUI_ERR) {
        return GUI_ERR;
    }
    /* 初始化消息队列 */
    if (GUI_MessageQueueInit() == GUI_ERR) {
        return GUI_ERR;
    }
    /* 初始化窗口管理器 */
    if (WM_Init() == GUI_ERR) {
        return GUI_ERR;
    }
    GUI_SetFont(&GUI_DEF_FONT);
    return GUI_OK;
}

/* 从内存中卸载GUI */
void GUI_Unload(void)
{
    GUI_LOCK();
    WM_DeleteWindow(_hRootWin); /* 删除所有窗口 */
    GUI_MessageQueueDelete();   /* 删除消息队列 */
    GUI_UNLOCK();
}

/* 获取屏幕尺寸 */
void GUI_ScreenSize(u_16 *xSize, u_16 *ySize)
{
    *xSize = GUI_LCD.xSize;
    *ySize = GUI_LCD.ySize;
}

/* 获取屏幕宽度 */
u_16 GUI_GetScreenWidth(void)
{
    return GUI_LCD.xSize;
}

/* 获取屏幕高度 */
u_16 GUI_GetScreenHeight(void)
{
    return GUI_LCD.ySize;
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

/* -------------------- GUI矩形计算 -------------------- */
/* 设置现在绘制区域的裁剪矩形链表 */
void GUI_SetNowRectList(GUI_AREA l, GUI_RECT *p)
{
    GUI_Context.Area = l;
    GUI_Context.InvalidRect = p;
}

/* 返回现在绘制区域的裁剪矩形链表 */
GUI_AREA GUI_GetNowRectList(void)
{
    return GUI_Context.Area;
}

/* 初始化绘制区域 */
void GUI_DrawAreaInit(GUI_RECT *p)
{
    if (GUI_RectOverlay(&GUI_Context.DrawRect, GUI_Context.InvalidRect, p)) {
        GUI_Context.pAreaNode = GUI_Context.Area;
    } else {
        GUI_Context.pAreaNode = NULL; /* 绘图区域与当前的有效绘制区域不相交 */
    }
}

/* 获取下一个裁剪矩形 */
GUI_BOOL GUI_GetNextArea(void)
{
    GUI_BOOL res = FALSE;
    GUI_AREA Area;
    GUI_RECT *ClipRect = &GUI_Context.ClipRect,
             *DrawRect = &GUI_Context.DrawRect;

    while (GUI_Context.pAreaNode && res == FALSE) { /* 直到找到下一个相交的矩形 */
        Area = GUI_Context.pAreaNode;
        GUI_Context.pAreaNode = Area->pNext;
        res = GUI_RectOverlay(ClipRect, DrawRect, &Area->Rect);
    }
    return res;
}

/* -------------------- GUI上下文设置 -------------------- */
/* 设置当前字体 */
void GUI_SetFont(GUI_FONT *Font)
{
    GUI_Context.Font = Font;
}

/* 设置背景色 */
void GUI_SetBackgroundColor(GUI_COLOR Color)
{
    GUI_Context.BGColor = Color;
}

/* 设置前景色 */
void GUI_SetForegroundColor(GUI_COLOR Color)
{
    GUI_Context.FGColor = Color;
}

/* 设置字体颜色 */
void GUI_SetFontColor(GUI_COLOR Color)
{
    GUI_Context.FontColor = Color;
}

/* GUI调试输出 */
#if GUI_DEBUG_MODE
void GUI_DebugOut(const char *s)
{
    _GUI_DebugOut(s);
}
#endif
