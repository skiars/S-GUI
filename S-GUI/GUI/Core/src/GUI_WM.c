#include "GUI_WM.h"
#include "GUI.h"
#include "RootWindow.h"

#if GUI_USE_MEMORY
static GUI_RECT __AllDraw_Rect;
#endif

static void WM__WindowTimer(WM_Obj *pObj);
static void WM__RefreshBackgnd(WM_Obj *pWin, RECT_LIST RectList);

/*
 * 注意,窗口管理器有多个全局变量,因此在使用操作系统时
 * 要注意线程安全性(可重入性).
 **/

/* 窗口管理器初始化 */
void WM_Init(void)
{
    GUI_hWin *p = &_hRootWin;
    
    *p = GUI_fastmalloc(sizeof(WM_Obj));
    WM_RootWindowInit(*p);
}

/*
 * 遍历窗口树
 * pTop:最上面的窗口,将会遍历它的所有子窗口.
 * pData:传给pFunc的参数,可以提供泛型编程的接口.
 * pFunc:遍历时相关实现功能的函数,pFunc返回非零时本函数将会返回1,
 *      否者本函数将会继续遍历,并在最后返回0.
 **/
static u_8 WM__TraversalWindows(WM_Obj *pRoot, void *pData, u_8 (*pFunc)(WM_Obj*, void*))
{
    WM_Obj *pWin = pRoot;

    while (1) {
        while (1) {
            if (pFunc(pWin, pData)) { /* 实现功能 */
                return 1;
            }
            if (pWin->hFirstChild == NULL) {
                break;
            }
            pWin = pWin->hFirstChild; /* 遍历子节点 */
        }
        while (pWin->hNext == NULL) {
            pWin = pWin->hParent; /* 回溯 */
            if (pWin == pRoot) {
                return 0;
            }
        }
        pWin = pWin->hNext; /* 遍历同属节点 */
    }
}

/* 窗口管理器重绘 */
static u_8 WM__WindowRepaint(WM_Obj *pWin, void *pData)
{
    RECT_LIST RectList;

    WM__WindowTimer(pWin); /* 窗口计时器 */
    if (WM_CheckInvalid(pWin)) { /* 窗口需要重绘 */
        /* 计算窗口裁剪矩形,并设置为当前的绘制链表 */
        RectList = GUI_CalcWindowRectList(pWin);
        if (RectList != NULL) {
            WM_MESSAGE Msg;
            GUI_RECT Rect;

            /* 透明窗口先刷新遮挡部分 */
            if (pWin->Style & WM_WINDOW_TRANS) {
                WM__RefreshBackgnd(pWin, RectList);
            }
            /* 重绘窗口 */
            Msg.MsgId = WM_PAINT;
            Msg.hWin = (WM_hWin)pWin;
            Rect = WM_GetTaliorInvalidRect(pWin);
            GUI_SetNowRectList(RectList, &Rect);
            pWin->WinCb(&Msg);
            GUI_FreeRectList(RectList);
#if GUI_USE_MEMORY
            /* 需要刷新的矩形 */
            __AllDraw_Rect = GUI_RectOrCalc(&pWin->Rect, &__AllDraw_Rect);
#endif
        }
        WM_CleanInvalid(pWin); /* 清除无效化 */
    }
    return 0;
}

/* 通过执行回调重绘无效窗口(所有工作) */
void WM_Exec(void)
{
    GUI_Lock();
    WM__TraversalWindows(_pRootWin, NULL, WM__WindowRepaint);
#if GUI_USE_MEMORY
    /* 将内存数据更新到LCD上 */
    __AllDraw_Rect = GUI_RectAndCalc(&_pRootWin->Rect, &__AllDraw_Rect);
    GUI_DrawFromMem(__AllDraw_Rect.x0, __AllDraw_Rect.y0,
                    __AllDraw_Rect.x1 - __AllDraw_Rect.x0 + 1,
                    __AllDraw_Rect.y1 - __AllDraw_Rect.y0 + 1);
    __AllDraw_Rect.x0 = 0;
    __AllDraw_Rect.y0 = 0;
    __AllDraw_Rect.x1 = 0;
    __AllDraw_Rect.y1 = 0;
#endif
    GUI_Unlock();
}

/*
 * 注意：下面的几个消息处理函数实际上很有可能工作在线程不安全的环境里，
 * 因为他们没有GUI资源锁，有时会因为调用这些函数发生内存访问冲突。
 **/

/*
 * 向窗口管理器的指定窗口发送消息
 * hWin:要指定窗口的句柄,为NULL时会将消息发送至当前活动窗口
 **/
void WM__SendMessage(WM_hWin hWin, WM_MESSAGE *pMsg)
{
    static u_8 __MseeageCount;
    
    if (__MseeageCount < GUI_MAX_MSG_NEST) {
        ++__MseeageCount;
        pMsg->hWin = hWin;
        if (((WM_Obj*)hWin)->WinCb) {
            ((WM_Obj*)hWin)->WinCb(pMsg);
        }
        --__MseeageCount;
    }
}

/* 向窗口管理器的指定窗口发送不带数据的消息 */
void MW__SendMsgNoData(WM_hWin hWin, u_16 MsgId)
{
    WM_MESSAGE Msg;

    Msg.data = NULL;
    Msg.hWinSrc = NULL;
    Msg.MsgId = MsgId;
    WM__SendMessage(hWin, &Msg);
}

/* 向窗口管理器的指定窗口发送消息 */
void WM_SendMessage(WM_hWin hWin, u_16 MsgId, void *data)
{          
    WM_MESSAGE Msg;

    Msg.MsgId = MsgId;
    Msg.data = data;
    WM__SendMessage(hWin, &Msg);
}

/* 向窗口管理器的指定窗口的父窗口发送消息 */
void WM_PostMessageToParent(WM_hWin hWin, u_16 MsgId, void *data)
{
    WM_MESSAGE Msg;

    Msg.hWinSrc = hWin;
    Msg.MsgId = MsgId;
    Msg.data = data;
    WM__SendMessage(((WM_Obj*)hWin)->hParent, &Msg);
}

/*
 * 获取窗口有效的区域大小
 * -该函数通过将目标窗口的与它所有的祖先窗口的用户区取并集得到有效区域
 **/
GUI_RECT WM_GetWindowAreaRect(WM_hWin hWin)
{
    GUI_RECT Rect;

    Rect = ((WM_Obj*)hWin)->Rect;
    while (((WM_Obj*)hWin) != _pRootWin) {
        hWin = ((WM_Obj*)hWin)->hParent;
        Rect = GUI_RectAndCalc(&((WM_Obj*)hWin)->UserRect,&Rect);
    }
    return Rect;
}

/* 获得裁剪后的窗口无效区域大小 */
GUI_RECT WM_GetTaliorInvalidRect(WM_hWin hWin)
{
    GUI_RECT Rect;

    Rect = WM_GetWindowAreaRect(hWin);
    Rect = GUI_RectAndCalc(&Rect, &((WM_Obj*)hWin)->InvalidRect);
    return Rect;
}

/* 向窗口树中添加一个窗口 */
void WM_AttachWindow(WM_hWin hWin, WM_hWin hParent)
{
    WM_Obj *pObj,*pParent;
    if (hParent == NULL) {
        pParent = _pRootWin;
    } else {
        pParent = hParent;
    }
    pObj = hWin;
    pObj->hNext = NULL;
    pObj->hParent = pParent;
    if (pParent->hFirstChild == NULL) {
        pParent->hFirstChild = pObj;
    } else {
        WM_Obj *pWin = pParent->hFirstChild;
        while (pWin->hNext) {
            pWin = pWin->hNext;
        }
        pWin->hNext = pObj;
    }
}

/* 将一个窗口从窗口树中移除 */
static void WM__RemoveWindow(WM_hWin hWin)
{
    WM_Obj *pWin, *pt;
    
    pWin = hWin;
    pt = pWin->hParent;
    if (pt == NULL) { /* 根窗口,无需移除 */
        return;
    }
    if (pt->hFirstChild == pWin) {
        pt->hFirstChild = pWin->hNext;
    } else {
        pt = pt->hFirstChild;
        while (pt->hNext != pWin) {
            pt = pt->hNext;
        }
        pt->hNext = pWin->hNext;
    }
    pWin->hParent = NULL;
    pWin->hNext = NULL; /* 孤立节点 */
}

/*
 * 删除一个窗口,这个窗口必须已经脱离窗口树(它至少不能有同属节点) 
 * 内部调用
 **/
static void WM__DeleteWindow(WM_hWin hWin)
{
    WM_MESSAGE Msg;
    WM_Obj *pObj = NULL, *pWin = hWin; /* 注意这里的pObj */

    if (hWin == NULL) {
        return;
    }
    while (1) {
        while (pWin->hFirstChild) { /* 直到最深处的节点 */
            pWin = pWin->hFirstChild;
        }
        if (pWin->hNext == NULL) { /* pWin没有同属节点 */
            hWin = pWin;
            pWin = pWin->hParent; /* 返回到父节点 */
            if (pWin == NULL) { /* 遍历结束 */
                break;
            }
            if (pWin->hFirstChild == hWin) { /* hWin是pWin的第一个子节点 */
                pWin->hFirstChild = NULL; /* 解除hWin与父节点的关联 */
            } else { /* hWin不是pWin的第一个节点 */
                pObj->hNext = NULL; /* 解除hWin与前面的同属节点的关联 */
            }
            Msg.hWin = hWin;
            Msg.MsgId = WM_DELETE;
            ((WM_Obj*)hWin)->WinCb(&Msg); /* 调用回调函数删除节点 */
        } else { /* 遍历同属节点 */
            pObj = pWin; /* pObj最后会是这一层同属节点的倒数第二个节点 */
            pWin = pWin->hNext;
        }
    }
}

/* 删除窗口 */
void WM_DeleteWindow(WM_hWin hWin)
{    
    GUI_Lock();
    /* 被遮挡的窗口无效化 */
    WM_InvalidCoverWindow(hWin, &((WM_Obj*)hWin)->Rect);
    WM__RemoveWindow(hWin); /* 先移除窗口 */
    WM__DeleteWindow(hWin);
    GUI_Unlock();
}

/* 获取当前活动窗口的句柄 */
WM_hWin WM_GetActiveMainWindow(void)
{
    WM_Obj *pWin = _RootWin.hFirstChild;
    while (pWin->hNext != NULL) { /* 遍历同属节点 */
        pWin = pWin->hNext;
    }
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
    GUI_Lock();
    while (pObj->hParent != _pRootWin) {
        pObj = pObj->hParent;
    }
    if (WM_GetActiveMainWindow() != pObj) {
        WM__RemoveWindow(pObj); /* 先移除窗口 */
        WM_AttachWindow(pObj, NULL); /* 插入窗口到最后 */
        WM_InvalidTree(pObj); /* 窗口及其所有的子窗口无效化 */
    }
    GUI_Unlock();
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

    /* 检查窗口ID是否已经注册 */
    pObj = WM_GetDialogItem(hParent, Id);
    if (pObj != NULL) {
        WM_SetActiveMainWindow(pObj);
        return NULL;
    }
    pObj = GUI_fastmalloc(sizeof(WM_Obj) + bytes);
    if (pObj == NULL) {
        return NULL;
    }
    GUI_Lock();
    WM_AttachWindow(pObj, pParent); /* 注册到父窗口 */
    pObj->hFirstChild = NULL;
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
    GUI_Unlock();
    return pObj;
}

/* 
 * 检查窗口标识符 
 * 返回值:0,标识符匹配;1,标识符不匹配
 **/
u_8 WM_CheckWindowSign(WM_hWin hWin, u_16 Sign)
{
    if (((WM_Obj*)hWin)->Sign == Sign) {
        return 0;
    }
    return 1;
}

/* 窗口部分区域无效化 */
void WM_InvalidateRect(WM_hWin hWin, GUI_RECT *pRect)
{
    WM_Obj* pWin = hWin;

    if (pRect == NULL) {
        pWin->InvalidRect = pWin->Rect;
    } else { /* 或操作 */
        if (pWin->InvalidRect.x0 > pRect->x0) {
            pWin->InvalidRect.x0 = pRect->x0;
        }
        if (pWin->InvalidRect.y0 > pRect->y0) {
            pWin->InvalidRect.y0 = pRect->y0;
        }
        if (pWin->InvalidRect.x1 < pRect->x1) {
            pWin->InvalidRect.x1 = pRect->x1;
        }
        if (pWin->InvalidRect.y1 < pRect->y1) {
            pWin->InvalidRect.y1 = pRect->y1;
        }
    }
}

/* 整个窗口无效化 */
void WM_Invalidate(WM_hWin hWin)
{
    WM_InvalidateRect(hWin, NULL);
}

/* 将一个窗口及其子窗口无效化,内部回调函数 */
static u_8 WM__InvalidTree(WM_Obj *pWin, void *p)
{
    WM_InvalidateRect(pWin, NULL);
    return 0;
}

/* 将一个窗口及其子窗口无效化,外部调用,待完善 */
GUI_RESULT WM_InvalidTree(WM_hWin hWin)
{
    WM__TraversalWindows(hWin, NULL, WM__InvalidTree);
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

    /* x0,y0设置为屏幕尺寸的值 */
    pWin->InvalidRect.x0 = GUI_GetScreenWidth();
    pWin->InvalidRect.y0 = GUI_GetScreenHeight();
    pWin->InvalidRect.x1 = 0;
    pWin->InvalidRect.y1 = 0;
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
 * 内部调用
 **/
static u_8 WM__FindWindow(WM_Obj *pWin, void *fWin)
{
    if (fWin == pWin) {
        return 1;
    }
    return 0;
}

/*
 * 在窗口树中寻找一个窗口
 * 返回值:0,没有这个窗口;1,有这个窗口
 * 外部调用
 **/
u_8 WM_FindWindow(WM_hWin hWin)
{
    u_8 Res;
    
    GUI_Lock();
    Res = WM__TraversalWindows(_pRootWin, hWin, WM__FindWindow);
    GUI_Unlock();
    return Res;
}

struct _DLG_ITEM {
    WM_Obj* pObj;
    u_16 Id;
};

static u_8 WM__GetDialogItem(WM_Obj *pWin, void *pData)
{
    if (pWin->Id == ((struct _DLG_ITEM*)pData)->Id) {
        ((struct _DLG_ITEM*)pData)->pObj = pWin;
        return 1;
    }
        return 0;
}

/* 得到ID号为Id的窗口成员句柄 */
WM_hWin WM_GetDialogItem(WM_hWin hWin, u_16 Id)
{
    struct _DLG_ITEM data;

    if (hWin == NULL) {
        hWin = _pRootWin;
    }
    if (((WM_Obj*)hWin)->hFirstChild == NULL) {
        return NULL;
    }
    GUI_Lock();
    data.Id = Id;
    data.pObj = NULL;
    WM__TraversalWindows(hWin, &data, WM__GetDialogItem);
    GUI_Unlock();
    return data.pObj;
}

/* 获得指定ID的子窗口句柄(不包括孙窗口...) */
WM_hWin WM_GetChildItem(WM_hWin hWin, u_16 Id)
{
    WM_Obj *pObj = hWin;
    
    if (pObj == NULL) {
        pObj = _pRootWin;
    }
    pObj = pObj->hFirstChild;
    if (pObj == NULL) {
        return NULL;
    }
    do{
        if (pObj->Id == Id) {
            break;
        }
        pObj = pObj->hNext;
    } while(pObj);
    return pObj;
}

/* 获取控件ID */
u_16 WM_GetDialogId(WM_hWin hWin)
{
    if (hWin) {
        return ((WM_Obj*)(hWin))->Id;
    }
    return 0;
}

/* 获得在输入坐标下暴露的窗口 */
struct _EXPOS_WIN {
    u_16 x, y;
    WM_hWin fWin;
    GUI_RECT tRect;
};

/*
 * 获得在输入坐标下暴露的窗口
 * 内部调用
 **/
static u_8 WM__GetExposedWindow(WM_Obj *pObj, void *pData)
{    
    ((struct _EXPOS_WIN *)pData)->tRect = WM_GetWindowAreaRect(pObj);
    if (GUI_CheckPointAtRect(((struct _EXPOS_WIN *)pData)->x,
                             ((struct _EXPOS_WIN *)pData)->y,
                             &((struct _EXPOS_WIN *)pData)->tRect)) {
        ((struct _EXPOS_WIN *)pData)->fWin = pObj;
    }
    return 0;
}

/*
* 获得在输入坐标下暴露的窗口
* 返回值:目标窗口的句柄
* 外部调用
**/
WM_hWin WM_GetExposedWindow(u_16 x, u_16 y)
{
    struct _EXPOS_WIN data;
    
    GUI_Lock();
    data.x = x;
    data.y = y;
    data.fWin = NULL;
    WM__TraversalWindows(_pRootWin, &data, WM__GetExposedWindow);
    GUI_Unlock();
    return data.fWin;
}

/* 获取父窗口句柄 */
WM_hWin WM_GetParentHandle(WM_hWin hWin)
{
    return ((WM_Obj*)hWin)->hParent;
}

/* 这个结构体用于包装下面函数的输入数据 */
struct _INV_DATA {
    GUI_RECT *pRect;
    WM_hWin hWin;
};

/* 将被一个窗口遮盖的窗口无效化,内部回调函数 */
static u_8 WM__InvalidCoverWindow(WM_Obj *pWin, void *pData)
{
    GUI_RECT Rect;

    if (pWin == ((struct _INV_DATA*)pData)->hWin) {
        return 1;
    }
    if (GUI_RectOverlay(&Rect, &pWin->Rect,
        ((struct _INV_DATA*)pData)->pRect) == GUI_OK) {
        WM_InvalidateRect(pWin, &Rect);
    }
    return 0;
}

/* 将被一个窗口遮盖的窗口无效化，
 * 例如在删除窗口时需要把将要被删除的窗口遮挡的窗口无效化 
 **/
void WM_InvalidCoverWindow(WM_hWin hWin, GUI_RECT *pRect)
{
    struct _INV_DATA data;

    data.hWin = hWin;
    data.pRect = pRect;
    WM__TraversalWindows(_pRootWin, &data, WM__InvalidCoverWindow);
}

/* 移动窗口回调函数 */
static u_8 WM__MoveWindow(WM_Obj *pObj, void *pData)
{
    GUI_RECT *Rect;


    Rect = &pObj->Rect;
    GUI_MoveRect(Rect, ((i_16*)pData)[0],((i_16*)pData)[1]);
    Rect = &pObj->UserRect;
    GUI_MoveRect(Rect, ((i_16*)pData)[0],((i_16*)pData)[1]);
    WM_Invalidate(pObj); /* 窗口无效化 */
    return 0;
}

/* 移动窗口外部调用API */
void WM_MoveWindow(WM_hWin hWin, i_16 dX, i_16 dY)
{
    i_16 xy[2];
    
    if (!(((WM_Obj*)hWin)->Style & WM_WINDOW_MOVE)) {
        return; /* 不可移动的窗口 */
    }
    if (dX == 0 && dY == 0) {
        return;
    }
    GUI_Lock();
    xy[0] = dX;
    xy[1] = dY;
    /* 先将被遮挡的窗口无效化 */
    WM_InvalidCoverWindow(hWin, &((WM_Obj*)hWin)->Rect);
    WM__TraversalWindows(hWin, xy, WM__MoveWindow);
    WM_Invalidate(hWin); /* 窗口无效化 */
    GUI_Unlock();
}

/* 设置可移动的窗口 */
void WM_SetMoveWindow(WM_hWin hWin)
{
    ((WM_Obj*)hWin)->Style |= WM_WINDOW_MOVE;
}

/* 窗口计时器
 * 窗口定时器可以让开启了定时功能的窗口在定时时间到以后产生一个WM_TIME_UPDATA消息
 **/
static void WM__WindowTimer(WM_Obj *pObj)
{
    if (pObj->Style & WM_WINDOW_TIMER) {  /* 窗口有计时器 */
        if (GUI_GetTime() > pObj->LastTime + pObj->TimeCount) {
            WM_MESSAGE Msg;

            Msg.MsgId = WM_TIME_UPDATA;
            Msg.hWin = (WM_hWin)pObj;
            pObj->WinCb(&Msg);
            pObj->LastTime = GUI_GetTime(); /* 从新获取时间 */
        }
    }
}

/* 设置窗口计时器,时间间隔单位为ms */
void WM_SetWindowTimer(WM_hWin hWin, u_16 Count)
{
    WM_Obj *pObj = hWin;
    
    GUI_Lock();
    pObj->Style |= WM_WINDOW_TIMER;
    pObj->TimeCount = Count;
    pObj->LastTime = GUI_GetTime();
    GUI_Unlock();
}

/* 设置窗口时间间隔计数 */
void WM_SetWindowTimerCount(WM_hWin hWin, u_16 Count)
{
    ((WM_Obj*)hWin)->TimeCount = Count;
}

/* 返回窗口时间间隔计数 */
u_16 WM_GetWindowTimerCount(WM_hWin hWin)
{
    return ((WM_Obj*)hWin)->TimeCount;
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

/* 结构体，用来给下面的代码封装数据 */
struct REF_BACK {
    RECT_LIST List;
    WM_Obj *pObj;
};

/* 刷新指定区域内被某个窗口遮挡的区域的回调函数 */
static u_8 WM__RefBackCb(WM_Obj *pWin, void *pData)
{
    RECT_LIST List;

    if (pWin == ((struct REF_BACK*)pData)->pObj) {
        return 1;
    }
    /* 检查两个窗口是否相交 */
    if (GUI_CheckRectIntersect(&((struct REF_BACK*)pData)->pObj->Rect,
                               &pWin->Rect)) {
        /* 计算裁剪链表 */
        List = GUI__RectListTo(pWin, ((struct REF_BACK*)pData)->pObj,
                               ((struct REF_BACK*)pData)->List);
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
    return 0;
}

/* 刷新指定区域内被某个窗口遮挡的区域，用于透明窗口刷新背景 */
static void WM__RefreshBackgnd(WM_Obj *pWin, RECT_LIST RectList)
{
    struct REF_BACK data;

    data.pObj = pWin;
    data.List = RectList;
    /* 从这个窗口的父窗口开始遍历 */
    if (pWin->hParent == _hRootWin) {
        WM__TraversalWindows(_hRootWin, &data, WM__RefBackCb);
    }
    WM__TraversalWindows(pWin->hParent, &data, WM__RefBackCb);
}

/*********         窗口透明实现部分结束         *********/
