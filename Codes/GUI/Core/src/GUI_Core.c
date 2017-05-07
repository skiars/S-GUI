#include "GUI_Core.h"
#include "GUI.h"

static void * __LockPtr;
static u_32 __LockTaskId;
static int __TaskLockCnt;

/* GUI初始化 */
GUI_RESULT GUI_Init(void)
{
    /* 内存管理初始化 */
    if (GUI_MemoryManagementInit() == GUI_ERR) {
        return GUI_ERR;
    }
    /* 初始化操作系统相关代码 */
    __LockPtr = GUI_TaskCreateLock();
    /* 初始化图形硬件 */
    GUI_DeviceInit();
    /* 初始化窗口剪切域裁剪私有堆 */
    if (GUI_RectListInit() == GUI_ERR) {
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
    GUI_SetPenSize(1); /* 默认线宽为1 */
    return GUI_OK;
}

/* 从内存中卸载GUI */
void GUI_Unload(void)
{
    GUI_LOCK();
    WM_DeleteWindow(gui_rootwin); /* 删除所有窗口 */
    GUI_MessageQueueDelete();   /* 删除消息队列 */
    GUI_DeleteDeviceList();     /* 删除设备列表 */
    GUI_UNLOCK();
}

/* GUI延时并更新 */
void GUI_Delay(GUI_TIME tms)
{
    GUI_TIME t = GUI_GetTime();
    static GUI_TIME t_last;

    WM_Exec();
    if (t > t_last + GUI_AUTO_MANAGE_TIME) {
        t_last = t;
        GUI_FreeIdleRectList();
    }
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
    if (!__TaskLockCnt || __LockTaskId != GUI_TaskGetId()) {
        GUI_TaskLock(__LockPtr);
        __LockTaskId = GUI_TaskGetId();
    }
    ++__TaskLockCnt;
}

/* GUI解锁 */
void GUI_UNLOCK(void)
{
    if (--__TaskLockCnt == 0) {
        GUI_TaskUnlock(__LockPtr);
    }
}

/* GUI调试输出 */
#if GUI_DEBUG_MODE
void GUI_DebugOut(const char *s)
{
    _GUI_DebugOut(s);
}
#endif
