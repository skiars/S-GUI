#include "GUI_WM.h"
#include "GUI.h"
#include "RootWindow.h"

/* 找到窗口Z序最大的子窗口（包括它自己）的下一个窗口，注意窗口h一定不能是NULL */
#define WM__FindChildEnd(h) \
    (((WM_Obj *)WM_GetTopChildWindow(h))->hNextLine)

#if GUI_USE_MEMORY
static GUI_RECT __PaintArea;
#endif

static int __InvalidWindowNum = 0;

static void WM__SendMessage(WM_hWin hWin, WM_MESSAGE *pMsg);
static void WM__RefreshBackgnd(WM_Obj *pWin, RECT_LIST RectList);

/*
 * 注意,窗口管理器有多个全局变量,因此在使用操作系统时
 * 要注意线程安全性(可重入性).
 **/

/* 窗口管理器初始化 */
GUI_RESULT WM_Init(void)
{
    GUI_hWin *p = &_hRootWin;
    
    *p = GUI_fastmalloc(sizeof(WM_Obj));
    if (*p == NULL) {
        return GUI_ERR;
    }
    WM_RootWindowInit(*p);
    return GUI_OK;
}

/* 窗口管理器重绘 */
static u_8 WM__WindowRepaint(WM_Obj *pWin)
{
    RECT_LIST RectList;

    if (WM_CheckInvalid(pWin)) { /* 窗口需要重绘 */
        /* 计算窗口裁剪矩形,并设置为当前的绘制链表 */
        RectList = GUI_CalcWindowRectList(pWin);
        if (RectList != NULL) {
            GUI_RECT Rect;

            /* 透明窗口先刷新遮挡部分 */
            if (pWin->Style & WM_WINDOW_TRANS) {
                WM__RefreshBackgnd(pWin, RectList);
            }
            /* 重绘窗口 */
            Rect = WM_GetTaliorInvalidRect(pWin);
            GUI_SetNowRectList(RectList, &Rect);
            WM_SendMessage(pWin, WM_PAINT, (GUI_PARAM)NULL);
            GUI_FreeRectList(RectList);
#if GUI_USE_MEMORY
            /* 需要刷新的矩形 */
            __PaintArea = GUI_RectOrCalc(&pWin->Rect, &__PaintArea);
#endif
        }
        WM_CleanInvalid(pWin); /* 清除无效化 */
    }
    return 0;
}

/* 重绘所有窗口 */
static void _PaintAll(void)
{
    WM_Obj *pWin;

    if (__InvalidWindowNum) {
        GUI_LOCK();
        /* 遍历并重绘窗口 */
        for (pWin = _hRootWin; pWin; pWin = pWin->hNextLine) {
            WM__WindowRepaint(pWin);
            --__InvalidWindowNum;
        }
        GUI_UNLOCK();
    }
}

/* 派发消息 */
static void WM__DispatchMessage(WM_MESSAGE *pMsg)
{
    if (pMsg->hWin) { /* 有目标窗口的消息 */
        WM__SendMessage(pMsg->hWin, pMsg);
    } else {
        GUI_TouchPadMessageHandle(pMsg); /* 处理触摸事件 */
    }
}

/* 通过执行回调重绘无效窗口(所有工作) */
void WM_Exec(void)
{
    WM_MESSAGE Msg;

    /* WM消息环 */
    while (GUI_GetMessage(&Msg) == GUI_OK) {
        WM__DispatchMessage(&Msg); /* 派发消息 */
    }
    GUI_TimerHandle();
    _PaintAll(); /* 重绘所有窗口 */
#if GUI_USE_MEMORY
    GUI_LOCK();
    /* 将内存数据更新到LCD上 */
    if (__PaintArea.x1 || __PaintArea.y1) {
        __PaintArea = GUI_RectAndCalc(&_pRootWin->Rect, &__PaintArea);
        GUI_DrawFromMem(__PaintArea.x0, __PaintArea.y0,
            __PaintArea.x1 - __PaintArea.x0 + 1,
            __PaintArea.y1 - __PaintArea.y0 + 1);
        __PaintArea.x0 = 0;
        __PaintArea.y0 = 0;
        __PaintArea.x1 = 0;
        __PaintArea.y1 = 0;
    }
    GUI_UNLOCK();
#endif
}

/*
 * 向窗口管理器的指定窗口发送消息。
 * hWin:要指定窗口的句柄,为NULL时将直接返回。
 **/
static void WM__SendMessage(WM_hWin hWin, WM_MESSAGE *pMsg)
{
    static u_8 __MseeageCount;
    
    if (hWin == NULL) { /* hWin不能为NULL */
        return;
    }
    GUI_LOCK();
    if (__MseeageCount < GUI_MAX_MSG_NEST) {
        ++__MseeageCount;
        pMsg->hWin = hWin;
        if (((WM_Obj *)hWin)->WinCb) {
            ((WM_Obj *)hWin)->WinCb(pMsg);
        }
        --__MseeageCount;
    }
    GUI_UNLOCK();
}

/* 向窗口管理器的指定窗口发送消息 */
void WM_SendMessage(WM_hWin hWin, u_16 MsgId, GUI_PARAM Param)
{
    WM_MESSAGE Msg;

    Msg.MsgId = MsgId;
    Msg.Param = Param;
    WM__SendMessage(hWin, &Msg);
}

/* 向窗口管理器的指定窗口的父窗口发送消息 */
void WM_SendMessageToParent(WM_hWin hWin, u_16 MsgId, GUI_PARAM Param)
{
    WM_MESSAGE Msg;

    Msg.hWinSrc = hWin;
    Msg.MsgId = MsgId;
    Msg.Param = Param;
    WM__SendMessage(((WM_Obj*)hWin)->hParent, &Msg);
}

/* 向消息队列发送消息 */
GUI_RESULT WM_PostMessage(WM_hWin hWin, u_16 MsgId, GUI_PARAM Param)
{
    WM_MESSAGE Msg;

    Msg.hWin = hWin;
    Msg.MsgId = MsgId;
    Msg.Param = Param;
    return GUI_PostMessage(&Msg);
}

/*
 * 获取窗口有效的区域大小
 * -该函数通过将目标窗口的与它所有的祖先窗口的用户区取并集得到有效区域
 * -hWin不能是NULL
 **/
GUI_RECT WM_GetWindowAreaRect(WM_hWin hWin)
{
    GUI_RECT Rect;

    GUI_LOCK();
    Rect = ((WM_Obj*)hWin)->Rect;
    while (((WM_Obj*)hWin) != _pRootWin) {
        hWin = ((WM_Obj*)hWin)->hParent;
        Rect = GUI_RectAndCalc(&((WM_Obj*)hWin)->UserRect,&Rect);
    }
    GUI_UNLOCK();
    return Rect;
}

/* 获得裁剪后的窗口无效区域大小，hWin不能是NULL */
GUI_RECT WM_GetTaliorInvalidRect(WM_hWin hWin)
{
    GUI_RECT Rect;

    GUI_LOCK();
    Rect = WM_GetWindowAreaRect(hWin);
    Rect = GUI_RectAndCalc(&Rect, &((WM_Obj*)hWin)->InvalidRect);
    GUI_UNLOCK();
    return Rect;
}

/* 获得比某个窗口Z序小1的窗口 */
WM_hWin WM_GetFrontWindow(WM_hWin hWin)
{
    WM_Obj *pWin = _pRootWin;

    if (hWin == _hRootWin) { /* 根窗口Z序最小 */
        return NULL;
    }
    GUI_LOCK();
    while (pWin && pWin->hNextLine != hWin) {
        pWin = pWin->hNextLine;
    }
    GUI_UNLOCK();
    return pWin;
}

/* 获得某个窗口Z序最高的子窗口 */
WM_hWin WM_GetTopChildWindow(WM_hWin hWin)
{
    WM_Obj *pWin = hWin;

    GUI_LOCK();
    if (pWin && pWin->hFirstChild) {
        pWin = pWin->hFirstChild;
        while (pWin->hFirstChild || pWin->hNext) {
            if (pWin->hNext) {
                pWin = pWin->hNext;
            } else {
                pWin = pWin->hFirstChild;
            }
        }
    }
    GUI_UNLOCK();
    return pWin;
}

/* 将一个窗口添加到父窗口下的最顶层 */
void WM_AttachWindow(WM_hWin hWin, WM_hWin hParent)
{
    WM_Obj *pWin = hWin, *pObj, *ptr;

    if (hWin == NULL) {
        return;
    }
    GUI_LOCK();
    if (hParent == NULL) { /* hParent为NULL时作为根窗口的子窗口 */
        pObj = _pRootWin;
    } else {
        pObj = hParent;
    }
    pWin->hNext = NULL;
    pWin->hParent = pObj;
    if (pObj->hFirstChild == NULL) { /* 父窗口没有子窗口 */
        pObj->hFirstChild = pWin;
    } else { /* 父窗口已经有子窗口 */
        pObj = pObj->hFirstChild;
        while (pObj->hNext) { /* 直到窗口的左兄弟 */
            pObj = pObj->hNext;
        }
        pObj->hNext = pWin;
        /* 取这个窗口Z序最高的子窗口 */
        pObj = WM_GetTopChildWindow(pObj);
    }
    /* 连接链表 */
    ptr = WM_GetTopChildWindow(pWin);
    ptr->hNextLine = pObj->hNextLine;
    pObj->hNextLine = pWin;
    GUI_UNLOCK();
}

/* 将一个窗口从窗口树中移除 */
void WM_RemoveWindow(WM_hWin hWin)
{
    WM_Obj *pWin = hWin, *pObj;
    
    /* 窗口不能为NULL,根窗口无需移除 */
    if (hWin == NULL || hWin == _pRootWin) {
        return;
    }
    pObj = pWin->hParent;
    GUI_LOCK();
    if (pObj->hFirstChild == pWin) { /* 它是父窗口的第一个子窗口 */
        pObj->hFirstChild = pWin->hNext;   
    } else {
        pObj = pObj->hFirstChild;
        while (pObj->hNext != pWin) { /* 直到目标窗口的左兄弟 */
            pObj = pObj->hNext;
        }
        pObj->hNext = pWin->hNext;
        pObj = WM_GetFrontWindow(pWin);
    }
    /* 孤立节点 */
    pWin->hParent = NULL;
    pWin->hNext = NULL;
    /* 连接链表 */
    pWin = WM_GetTopChildWindow(pWin);
    pObj->hNextLine = pWin->hNextLine;
    pWin->hNextLine = NULL;
    GUI_UNLOCK();
}

/* 删除窗口 */
void WM_DeleteWindow(WM_hWin hWin)
{
    WM_Obj *pWin = hWin;

    if (hWin == NULL) { /* hWin不能为NULL */
        return;
    }
    GUI_LOCK();
    /* 被遮挡的窗口无效化 */
    WM_InvalidCoverWindow(pWin, &pWin->Rect);
    WM_RemoveWindow(pWin); /* 先移除窗口 */
    while (pWin) {
        hWin = pWin;
        pWin = pWin->hNextLine;
        /* 删除窗口的定时器 */
        GUI_SetWindowTimer(hWin, 0);
        /* 调用回调函数删除节点 */
        WM_SendMessage(hWin, WM_DELETE, (GUI_PARAM)NULL);
    }
    GUI_UNLOCK();
}

/* 获取当前活动窗口的句柄 */
WM_hWin WM_GetActiveMainWindow(void)
{
    WM_Obj *pWin = _RootWin.hFirstChild;

    GUI_LOCK();
    while (pWin->hNext != NULL) { /* 遍历同属节点 */
        pWin = pWin->hNext;
    }
    GUI_UNLOCK();
    return pWin;
}

/*
 * 设置活动窗口(这个窗口会放在最前面显示)
 * hWin:要设置的窗口句柄
 * 返回值:GUI_OK,正常;GUI_ERR,错误,没有该窗口
 **/
GUI_RESULT WM_SetActiveMainWindow(WM_hWin hWin)
{
    WM_Obj *pObj = hWin;

    if (hWin == NULL) {
        return GUI_ERR;
    }
    GUI_LOCK();
    while (pObj && pObj->hParent != _pRootWin) {
        pObj = pObj->hParent;
    }
    if (WM_GetActiveMainWindow() != pObj) {
        WM_RemoveWindow(pObj); /* 先移除窗口 */
        WM_AttachWindow(pObj, NULL); /* 插入窗口到最后 */
        WM_InvalidTree(pObj); /* 窗口及其所有的子窗口无效化 */
    }
    GUI_UNLOCK();
    return GUI_OK;
}

/*
 * 创建一个窗口作为指定窗口的子窗口
 * 当hParent为NULL时,窗口将是RootWindow的子窗口
 **/
WM_hWin WM_CreateWindowAsChild(i_16 x0,             /* 橫坐标 */
                               i_16 y0,             /* 纵坐标 */
                               u_16 xSize,          /* 宽度 */
                               u_16 ySize,          /* 高度 */
                               WM_hWin hParent,     /* 父窗口句柄 */
                               u_16 Flag,           /* 标志或指令 */
                               u_8 Sign,            /* 窗口标识符 */
                               u_16 Id,             /* 窗口ID */
                               WM_CALLBACK *WinCb,  /* 窗口功能回调函数 */
                               u_32 bytes)          /* 窗口结构体多出的字节 */
{
    WM_Obj *pObj, *pParent = hParent;

    GUI_LOCK();
    /* 检查窗口ID是否已经注册 */
    pObj = WM_GetWindowHandle(Id);
    if (pObj != NULL) {
        WM_SetActiveMainWindow(pObj); /* 将已经注册的窗口放到最前 */
        GUI_UNLOCK();
        return NULL;
    }
    pObj = GUI_fastmalloc(sizeof(WM_Obj) + bytes);
    if (pObj == NULL) {
        GUI_UNLOCK();
        return NULL;
    }
    pObj->hFirstChild = NULL;
    WM_AttachWindow(pObj, pParent); /* 注册到父窗口 */
    pObj->Style = Flag;
    pObj->Sign = Sign;
    pObj->Id = Id;
    pObj->WinCb = WinCb;
    if (pParent) {
        x0 += pParent->UserRect.x0;
        y0 += pParent->UserRect.y0;
    }
    pObj->Rect.x0 = x0;
    pObj->Rect.y0 = y0;
    pObj->Rect.x1 = x0 + xSize - 1;
    pObj->Rect.y1 = y0 + ySize - 1;
    WM_Invalidate(pObj); /* 整个窗口失效 */
    GUI_UNLOCK();
    /* 向消息队列发送窗口已创建消息(稍后会处理消息) */
    WM_PostMessage(pObj, WM_CREATED, (GUI_PARAM)NULL);
    return pObj;
}

/* 
 * 检查窗口标识符 
 * 返回值:0,标识符匹配;1,标识符不匹配
 **/
GUI_RESULT WM_CheckWindowSign(WM_hWin hWin, u_16 Sign)
{
    if (hWin && ((WM_Obj*)hWin)->Sign == Sign) {
        return GUI_OK;
    }
    return GUI_ERR;
}

/* 窗口部分区域无效化 */
void WM_InvalidateRect(WM_hWin hWin, GUI_RECT *pRect)
{
    WM_Obj* pWin = hWin;
    GUI_RECT *r = &pWin->Rect, *ri = &pWin->InvalidRect;

    GUI_LOCK();
    if (pRect == NULL) {
        pWin->InvalidRect = pWin->Rect;
    } else { /* 或操作 */
        if (ri->x0 > pRect->x0) {
            ri->x0 = pRect->x0 >= r->x0 ? pRect->x0 : r->x0;
        }
        if (ri->y0 > pRect->y0) {
            ri->y0 = pRect->y0 >= r->y0 ? pRect->y0 : r->y0;
        }
        if (ri->x1 < pRect->x1) {
            ri->x1 = pRect->x1 <= r->x1 ? pRect->x1 : r->x1;
        }
        if (ri->y1 < pRect->y1) {
            ri->y1 = pRect->y1 <= r->y1 ? pRect->y1 : r->y1;
        }
    }
    ++__InvalidWindowNum;
    GUI_UNLOCK();
}

/* 整个窗口无效化 */
void WM_Invalidate(WM_hWin hWin)
{
    WM_InvalidateRect(hWin, NULL);
}

/* 将一个窗口及其子窗口无效化 */
GUI_RESULT WM_InvalidTree(WM_hWin hWin)
{
    WM_Obj *pWin = hWin;

    if (hWin == NULL) {
        return GUI_ERR;
    }
    GUI_LOCK();
    /* 找到遍历子窗口的终点 */
    hWin = WM__FindChildEnd(pWin);
    while (pWin && pWin != hWin) {
        WM_InvalidateRect(pWin, NULL);
        pWin = pWin->hNextLine;
    }
    GUI_UNLOCK();
    return GUI_OK;
}

/* 
 * 判断一个窗口是否无效
 * 返回0:非无效,1:无效
 **/
u_8 WM_CheckInvalid(WM_hWin hWin)
{
    GUI_RECT Rect;
    
    Rect = WM_GetWindowInvalidRect(hWin);
    return GUI_CheckRectNull(&Rect);
}

/* 清除窗口无效化 */
void WM_CleanInvalid(WM_hWin hWin)
{
    WM_Obj* pWin = hWin;

    GUI_LOCK();
    /* x0,y0设置为屏幕尺寸的值 */
    pWin->InvalidRect.x0 = GUI_GetScreenWidth();
    pWin->InvalidRect.y0 = GUI_GetScreenHeight();
    pWin->InvalidRect.x1 = 0;
    pWin->InvalidRect.y1 = 0;
    GUI_UNLOCK();
}

/* 获取窗口的尺寸 */
GUI_RECT WM_GetWindowRect(WM_hWin hWin)
{
    return ((WM_Obj*)hWin)->Rect;
}

/* 获取窗口用户区域尺寸 */
GUI_RECT WM_GetWindowUserRect(WM_hWin hWin)
{
    return ((WM_Obj*)hWin)->UserRect;
}

/* 获取窗口无效区域尺寸 */
GUI_RECT WM_GetWindowInvalidRect(WM_hWin hWin)
{
    return ((WM_Obj*)hWin)->InvalidRect;
}

/*
 * 在窗口树中寻找一个窗口
 * 返回值:0,没有这个窗口;1,有这个窗口
 * 外部调用
 **/
GUI_RESULT WM_FindWindow(WM_hWin hWin)
{
    WM_Obj *pWin = _hRootWin;
    
    if (pWin) {
        GUI_LOCK();
        while (pWin && pWin != hWin) { /* 遍历窗口树 */
            pWin = pWin->hNextLine;
        }
        GUI_UNLOCK();
        if (pWin == hWin) {
            return GUI_OK; /* 找到了窗口 */
        }
    }
    return GUI_ERR; /* 不存在该窗口 */
}

/* 获得指定ID的子窗口句柄 */
WM_hWin WM_GetDialogItem(WM_hWin hWin, u_16 Id)
{
    WM_Obj *pWin = hWin;

    /* WM_NULL_ID不可寻找 */
    if (Id == WM_NULL_ID) {
        return NULL;
    }
    GUI_LOCK();
    if (pWin && pWin->hFirstChild) {
        pWin = pWin->hFirstChild; /* 从子窗口开始寻找 */
        /* 找到遍历子窗口的终点 */
        hWin = WM__FindChildEnd(hWin);
        while (pWin && pWin->Id != Id && pWin != hWin) {
            pWin = pWin->hNextLine;
        }
    } else {
        pWin = NULL;
    }
    GUI_UNLOCK();
    return pWin;
}

/* 获得指定ID的窗口句柄 */
WM_hWin WM_GetWindowHandle(u_16 Id)
{
    WM_Obj *pWin = _pRootWin;

    /* WM_NULL_ID不可寻找 */
    if (Id == WM_NULL_ID) {
        return NULL;
    }
    GUI_LOCK();
    while (pWin && pWin->Id != Id) { /* 遍历窗口 */
        pWin = pWin->hNextLine;
    }
    GUI_UNLOCK();
    return pWin;
}

/* 获取控件ID */
u_16 WM_GetDialogId(WM_hWin hWin)
{
    if (hWin) {
        return ((WM_Obj*)(hWin))->Id;
    }
    return 0;
}

/*
 * 获得在输入坐标下暴露的窗口
 * 返回值:目标窗口的句柄
 **/
WM_hWin WM_GetExposedWindow(u_16 x, u_16 y)
{
    WM_Obj *pWin = NULL, *p;
    GUI_RECT r;

    GUI_LOCK();
    for (p = _pRootWin;  p; p = p->hNextLine) {
        r = WM_GetWindowAreaRect(p);
        if (GUI_CheckPointAtRect(x, y, &r) == GUI_OK) {
            pWin = p;
        }
    }
    GUI_UNLOCK();
    return pWin;
}

/* 获取父窗口句柄 */
WM_hWin WM_GetParentHandle(WM_hWin hWin)
{
    return ((WM_Obj*)hWin)->hParent;
}

/* 将被一个窗口遮盖的窗口及其子窗口无效化，
 * 例如在删除窗口时需要把将要被删除的窗口遮挡的窗口无效化 
 **/
void WM_InvalidCoverWindow(WM_hWin hWin, GUI_RECT *pRect)
{
    WM_Obj *p = hWin;
    GUI_RECT r;

    /* hWin不能为NULL,也不能是RootWindow */
    if (hWin == NULL && p->hParent == NULL) { 
        return;
    }
    /* 空矩形直接返回 */
    if (GUI_CheckRectNull(pRect) == 0) {
        return;
    }
    GUI_LOCK();
    /*这个窗口最多只能遮盖到它的父窗口*/
    for (p = p->hParent;  p && p != hWin; p = p->hNextLine) {
        if (GUI_RectOverlay(&r, &p->Rect, pRect) == GUI_OK) {
            WM_InvalidateRect(p, &r);
        }
    }
    GUI_UNLOCK();
}

/* 移动窗口及它所有的子窗口 */
void WM_MoveWindow(WM_hWin hWin, i_16 dX, i_16 dY)
{
    WM_Obj *p = hWin;
    
    GUI_LOCK();
    if (p && p->Style & WM_WINDOW_MOVE && (dX || dY)) {
        /* 先将被遮挡的窗口无效化 */
        WM_InvalidCoverWindow(p, &p->Rect);
        /* 找到遍历子窗口的终点 */
        hWin = WM__FindChildEnd(p);
        for (; p != hWin; p = p->hNextLine) { /* 遍历子窗口 */
            GUI_MoveRect(&p->Rect, dX, dY);
            GUI_MoveRect(&p->UserRect, dX, dY);
            WM_Invalidate(p); /* 窗口无效化 */
        }
    }
    GUI_UNLOCK();
}

/* 设置可移动的窗口 */
void WM_SetMoveWindow(WM_hWin hWin)
{
    ((WM_Obj*)hWin)->Style |= WM_WINDOW_MOVE;
}

/* 为一个窗口计算裁剪矩形链表 */
RECT_LIST GUI_CalcWindowRectList(WM_hWin hWin)
{
    WM_Obj *pWin = hWin, *pObj;
    GUI_RECT Rect1, Rect2;
    RECT_LIST RectList;

    /* 在考虑遮挡之前,窗口就只有一个裁剪矩形 */
    RectList = GUI_GetRectList(1);
    if (RectList) {
        Rect1 = WM_GetTaliorInvalidRect(pWin);
        RectList->Rect = Rect1;
    }
    /* 窗口会被它的儿子们或者右边的兄弟们(如果有的话)裁剪,也就是遮挡，遍历这些儿子和兄弟们，
     *逐个计算窗口的裁剪矩形表，最后就能得到这个窗口被它们遮挡后的裁剪矩形表
     **/
    if (pWin->hFirstChild) {
        /* 先遍历它所有的子窗口 */
        pObj = pWin->hFirstChild;
        while (pObj && RectList) {
                RectList = GUI_ReCalcRectList(RectList, &pObj->Rect);
            if (pObj->Style & WM_WINDOW_TRANS /* 与之相交的透明窗口无效化 */
              && GUI_RectOverlay(&Rect2, &pObj->Rect, &Rect1) == GUI_OK) {
                WM_InvalidateRect(pObj, &Rect2);
            }
            pObj = pObj->hNext; /* 向右遍历 */
        }
    }
    /* 再遍历它右边的同属窗口及祖先窗口的同属窗口 */
    pObj = pWin;
    while (pObj != _pRootWin && RectList) {
        while (pObj->hNext && RectList){
            pObj = pObj->hNext; /* 向右遍历 */
                RectList = GUI_ReCalcRectList(RectList, &pObj->Rect);
            if (pObj->Style & WM_WINDOW_TRANS /* 与之相交的透明窗口无效化 */
                && GUI_RectOverlay(&Rect2, &pObj->Rect, &Rect1) == GUI_OK) {
                WM_InvalidateRect(pObj, &Rect2);
            }
        }
        pObj = pObj->hParent; /* 向上遍历 */
    }
    return RectList;
}

/*****************************************************************************
* 这部分代码用于实窗口透明效果。
* 注意：
* 这部分程序的效率可能很低。
* 
* 窗口透明的原理：
* 当发现一个窗口有透明属性时，会在重绘了所有被透明窗口遮挡的窗口之后再去重绘当前的透明窗口，
* 以保证显示不会重叠。由于事先已经计算出了要现实的透明窗口的裁剪矩形表，所以直接以这个裁剪矩
* 形表为基础生成被遮挡窗口的裁剪矩形表，这样，被遮挡的窗口就不会覆盖多余的部分，而是完全包含
* 在可见的透明窗口下（也就是我们不会看到被遮挡的窗口越过它的边界显示）。
* 一个值得注意的情况是，如果透明窗口遮挡了另一个透明窗口，上述过程就会又进行一次，直到所有透
* 明窗口都不再遮挡透明窗口为止。这一机制使用递归很容易就实现了，但是递归的效率不高，并且对堆
* 栈的消耗也很大。所以一定要控制透明窗口对透明窗口的遮挡层数。
*****************************************************************************/

/* 为一个窗口计算裁剪矩形链表，直到遇到某个窗口 */
static RECT_LIST GUI__RectListTo(WM_Obj *pWin, WM_Obj *pEnd, RECT_LIST List)
{
    u_16 i;
    WM_Obj *pObj = NULL;
    GUI_RECT Rect;
    RECT_LIST pt, RectList = List;

    /* 统计裁剪矩形链表中的相交数量 */
    for (i = 0; RectList; RectList = RectList->pNext) {
        if (GUI_CheckRectIntersect(&RectList->Rect, &pWin->Rect)) {
            ++i;
        }
    }
    RectList = GUI_GetRectList(i);
    pt = RectList;
    while (pt && List) { /* 复制相交部分的矩形链表 */
        if (GUI_RectOverlay(&Rect, &List->Rect, &pWin->Rect) == GUI_OK) {
            pt->Rect = Rect;
            pt = pt->pNext;
        }
        List = List->pNext;
    }
    /* 窗口会被它的儿子们或者右边的兄弟们(如果有的话)裁剪,也就是遮挡 */
    if (pWin->hFirstChild) {
        /* 先遍历它所有的子窗口 */
        pObj = pWin->hFirstChild;
        while (pObj && pObj != pEnd && RectList) {
            RectList = GUI_ReCalcRectList(RectList, &pObj->Rect);
            pObj = pObj->hNext; /* 向右遍历 */
        }
    }
    if (!pEnd || pObj != pEnd) { /* pEnd为NULL或者还没遇到pEnd */
        /* 再遍历它右边的同属窗口及祖先窗口的同属窗口 */
        pObj = pWin;
        while (pObj != _pRootWin && RectList) {
            while (pObj->hNext && pObj->hNext != pEnd && RectList) {
                pObj = pObj->hNext; /* 向右遍历 */
                RectList = GUI_ReCalcRectList(RectList, &pObj->Rect);
            }
            pObj = pObj->hParent; /* 向上遍历 */
        }
    }
    return RectList;
}

/* 刷新指定区域内被某个窗口遮挡的区域的回调函数 */
static void WM__RefBackCb(WM_Obj *pWin, WM_Obj *pObj, RECT_LIST List)
{
    /* 检查两个窗口是否相交 */
    if (GUI_CheckRectIntersect(&pObj->Rect, &pWin->Rect)) {
        /* 计算裁剪链表 */
        List = GUI__RectListTo(pWin, pObj, List);
        if (List != NULL) {
            WM_MESSAGE Msg;

            /* 透明窗口先刷新遮挡部分 */
            if (pWin->Style & WM_WINDOW_TRANS) {
                WM__RefreshBackgnd(pWin, List); /* 注意递归！ */
            }
            /* 这是被上面的透明窗口遮挡的一个窗口，它现在需要先被刷新，
             * 然后才能使上面的透明窗口得到刷新。
             */
            Msg.MsgId = WM_PAINT;
            Msg.hWin = (WM_hWin)pWin;
            GUI_SetNowRectList(List, &pWin->Rect);
            pWin->WinCb(&Msg);
            GUI_FreeRectList(List);
        }
    }
}

/* 刷新指定区域内被某个窗口遮挡的区域，用于透明窗口刷新背景 */
static void WM__RefreshBackgnd(WM_Obj *pWin, RECT_LIST RectList)
{
    WM_Obj *pObj;

    /* 从这个窗口的父窗口开始遍历 */
    for (pObj = pWin->hParent; pObj != pWin; pObj = pObj->hNextLine) {
        WM__RefBackCb(pObj, pWin, RectList);
    }
}

/*********         窗口透明实现部分结束         *********/
