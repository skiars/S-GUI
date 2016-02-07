#ifndef __GUI_WM_H
#define __GUI_WM_H

#include "GUI_Typedef.h"
#include "GUI_Core.h"

/* 消息定义 */
/* 0保留,表示没有消息 */
#define WM_NULL_EVENT    0x0000

/* GUI系统保留消息 */
#define WM_INIT_DIALOG       0x0100    /* 对话框初始化 */
#define WM_PAINT             0x0101    /* 重绘 */
#define WM_PRE_PAINT         0x0102
#define WM_POST_PAINT        0x0103
#define WM_DELETE            0x0104
#define WM_TP_CHECKED        0x0105    /* 触摸板按下 */
#define WM_TP_REMOVED        0x0106    /* 触摸板松开 */
#define WM_TP_PRESS          0x0107    /* 触摸坐标一直在当前窗口 */
#define WM_TP_LEAVE          0x0108    /* 触摸坐标离开当前窗口 */
#define WM_BUTTON_CLICKED    0x0200    /* BUTTON控件按下 */
#define WM_BUTTON_RELEASED   0x0201    /* BUTTON控件已经释放 */
#define WM_NUTTON_MOVED_OUT  0x0202    /* BUTTON被点击然后指针移开 */
#define WM_KEY_CHECKED       0x0210
#define WM_WIONDOW_CHECKED   0x0203     /* 窗口被点击 */
#define WM_TIME_UPDATA       0x0210     /* 时间更新 */
/* 1024以后为扩展消息(用户可定义) */

/* Window状态定义 */
#define WM_WINDOW_ACTIVE    0x0001   /* 活动窗口 */
#define WM_WINDOW_TRANS     0x0002   /* 透明窗口 */
#define WM_WINDOW_MOVE      0x0004   /* 可移动的窗口 */
#define WM_WINDOW_TIMER     0x0008   /* 窗口需要计时 */


#define WM_USER_ID          0x0200

#define _pRootWin ((WM_Obj*)_hRootWin)
#define _RootWin (*_pRootWin)

typedef GUI_hWin WM_hWin;
typedef GUI_MESSAGE WM_MESSAGE;

/* 窗口信息处理回调函数 */
typedef void WM_CALLBACK (WM_MESSAGE *pMsg);
/* 窗口重绘回调函数 */
typedef void GUI_PAINT_CALLBACK (GUI_hWin hWin);

/* 窗口管理注册表节点 */
typedef struct {
    u_8 z;               /* 窗口的z坐标 */
    u_8 IadFlag;         /* 窗口无效标记 */
    WM_MESSAGE *pMsg;    /* 窗口信息指针 */
    WM_CALLBACK *WinCb;  /* 窗口功能回调函数 */
    GUI_PAINT_CALLBACK *PaintCallback;  /* 窗口重绘函数 */
} WM_REGISTRY_NODE;

/* 窗口管理器基类 */
typedef struct {
    GUI_RECT Rect;          /* 窗口尺寸 */
    GUI_RECT InvalidRect;   /* 窗口无效区域 */
    GUI_RECT UserRect;      /* 窗口用户区域 */
    WM_hWin hParent;        /* 父窗口指针 */
    WM_hWin hFirstChild;    /* 第一个子窗口指针 */
    WM_hWin hNext;          /* 指向下一个同属窗口 */
    WM_hWin hNextLine;      /* 指向下一链节 */
    WM_CALLBACK *WinCb;     /* 窗口信息响应回调函数 */
    u_16 Style;             /* 窗口风格 */
    u_8 Sign;               /* 窗口类型辨识符 */
    u_16 Id;                /* 窗口Id */
    GUI_TIME LastTime;      /* 上次更新时间 */
    u_16 TimeCount;         /* 时间间隔计数 */
} WM_Obj;

GUI_RESULT WM_Init(void);
void WM_Exec(void);
void WM_SendMessage(WM_hWin hWin, u_16 MsgId, void *data);
void WM_PostMessageToParent(WM_hWin hWin, u_16 MsgId, void *data);
GUI_RECT WM_GetWindowAreaRect(WM_hWin hWin);
GUI_RECT WM_GetTaliorInvalidRect(WM_hWin hWin);
void WM_AttachWindow(WM_hWin hWin, WM_hWin hParent);
WM_hWin WM_GetActiveMainWindow(void);
GUI_RESULT WM_SetActiveMainWindow(WM_hWin hWin);
WM_hWin WM_CreateWindowAsChild(i_16 x0, i_16 y0, u_16 xSize,
                               u_16 ySize, WM_hWin hParent,
                               u_16 Flag, u_8 Sign, u_16 Id,
                               WM_CALLBACK *WinCb, u_32 bytes);
void WM_DeleteWindow(WM_hWin hWin);
u_8 WM_CheckWindowSign(WM_hWin hWin, u_16 Sign);
void WM_InvalidateRect(WM_hWin hWin, GUI_RECT *pRect);
void WM_Invalidate(WM_hWin hWin);
GUI_RESULT WM_InvalidTree(WM_hWin hWin);
u_8 WM_CheckInvalid(WM_hWin hWin);
void WM_CleanInvalid(WM_hWin hWin);
GUI_RECT WM_GetWindowRect(WM_hWin hWin);
GUI_RECT WM_GetWindowUserRect(WM_hWin hWin);
GUI_RECT WM_GetWindowInvalidRect(WM_hWin hWin);
GUI_RESULT WM_FindWindow(WM_hWin hWin);
WM_hWin WM_GetDialogItem(WM_hWin hWin, u_16 Id);
u_16 WM_GetDialogId(WM_hWin hWin);
WM_hWin WM_GetExposedWindow(u_16 x, u_16 y);
WM_hWin WM_GetParentHandle(WM_hWin hWin);
void WM_MoveWindow(WM_hWin hWin, i_16 dX, i_16 dY);
void WM_SetWindowTimer(WM_hWin hWin, u_16 Count);
void WM_SetWindowTimerCount(WM_hWin hWin, u_16 Count);
u_16 WM_GetWindowTimerCount(WM_hWin hWin);

RECT_LIST GUI_CalcWindowRectList(WM_hWin hWin);
void WM_InvalidCoverWindow(WM_hWin hWin, GUI_RECT *pRect);

#endif /* __GUI_WM_H */
