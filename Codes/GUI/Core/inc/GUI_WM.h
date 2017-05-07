#ifndef __GUI_WM_H
#define __GUI_WM_H

#include "GUI_Typedef.h"
#include "GUI_Queue.h"
#include "GUI_ClipAreaHeap.h"

/* 系统保留消息 */
#define WM_INIT_DIALOG       0x0100     /* 对话框初始化 */
#define WM_PAINT             0x0101     /* 重绘 */
#define WM_DELETE            0x0102     /* 删除窗口 */
#define WM_CREATED           0x0103     /* 窗口已经创建 */
#define WM_TIMER             0x0104     /* 窗口定时器更新 */
#define WM_GET_CLIENT        0x0105     /* 获取客户区句柄 */
#define WM_SET_FOCUS         0x0106     /* 设置输入焦点 */
#define WM_GET_FOCUS         0x0107     /* 获取输入焦点 */
#define WM_KILL_FOCUS        0x0108     /* 窗口失去焦点 */
#define WM_QUIT              0x0109     /* 退出 */
#define WM_TP_CHECKED        0x0120     /* 触摸板按下 */
#define WM_TP_REMOVED        0x0121     /* 触摸板松开 */
#define WM_TP_PRESS          0x0122     /* 触摸坐标一直在当前窗口 */
#define WM_TP_LEAVE          0x0123     /* 触摸坐标离开当前窗口 */
#define WM_BUTTON_CLICKED    0x0200     /* BUTTON控件按下 */
#define WM_BUTTON_RELEASED   0x0201     /* BUTTON控件已经释放 */
#define WM_NUTTON_MOVED_OUT  0x0202     /* BUTTON被点击然后指针移开 */
#define WM_WIONDOW_CHECKED   0x0203     /* 窗口被点击 */
#define WM_LISTBOX_CHECK     0x0204     /* LISTBOX被点击(新的选中项) */
#define WM_KEYDOWN           0x0210     /* 按键按下 */
#define WM_KEYUP             0x0211     /* 按键松开 */
#define WM_USER_MESSAGE      0x1000     /* 用户自定义的消息 */
/* 0x0400以后为扩展消息(用户自定义) */

/* 窗口状态定义(Window status define) */
#define WM_WS_ACTIVE         (1 << 0)   /* 活动窗口 */
#define WM_WS_TRANS          (1 << 1)   /* 透明窗口 */
#define WM_WS_MOVE           (1 << 2)   /* 可移动的窗口 */
#define WM_WS_INVAILD        (1 << 3)   /* 无效窗口  */
#define WM_WS_BACKGND        (1 << 4)   /* 背景窗口 */
#define WM_WS_STICK          (1 << 5)   /* 置顶窗口 */
#define WM_WS_HIDE           (1 << 6)   /* 隐藏的窗口 */
#define WM_WS_MEMDEV         (1 << 7)   /* 使用内存设备 */

/* 保留ID */
#define WM_NULL_ID          0x0000
#define WM_ROOTWIN_ID       0x0001
#define WM_USER_ID          0x0200

#define GUI_COUNTOF(a)      (sizeof(a) / sizeof(a[0]))

typedef GUI_HWIN WM_HWIN;
typedef GUI_MESSAGE WM_MESSAGE;

/* 消息处理回调函数 */
typedef void WM_CALLBACK (WM_MESSAGE *pMsg);

/* 窗口管理器基类 */
typedef struct {
    WM_HWIN hParent;        /* 父窗口指针 */
    WM_HWIN hFirstChild;    /* 第一个子窗口指针 */
    WM_HWIN hNext;          /* 指向下一个同属窗口 */
    WM_HWIN hNextLine;      /* 指向下一链节 */
    GUI_RECT rect;          /* 窗口尺寸 */
    GUI_RECT invalidRect;   /* 窗口无效区域 */
    GUI_AREA clipArea;      /* 窗口剪切域 */
    WM_CALLBACK *winCb;     /* 窗口信息响应回调函数 */
    int status;             /* 窗口状态 */
    int id;                 /* 窗口Id */
} WM_Obj;

extern WM_HWIN gui_rootwin;

/* inline functions */
static inline WM_Obj* WM_HandleToPtr(WM_HWIN hWin)
{
    return (WM_Obj *)hWin;
}

GUI_RESULT WM_Init(void);
void WM_Exec(void);
void WM__SendMessage(WM_HWIN hWin, WM_MESSAGE *pMsg);
void WM_SendMessage(WM_HWIN hWin, int MsgId, GUI_PARAM Param);
void WM_SendMessageToParent(WM_HWIN hWin, GUI_MESSAGE *pMsg);
GUI_RESULT WM_PostMessage(WM_HWIN hWin, int MsgId, GUI_PARAM Param);
void WM_GetWindowAreaRect(WM_HWIN hWin, GUI_RECT *pRect);
void WM_GetTaliorInvalidRect(WM_HWIN hWin, GUI_RECT *pRect);
WM_HWIN WM_GetFrontWindow(WM_HWIN hWin);
WM_HWIN WM_GetTopChildWindow(WM_HWIN hWin);
WM_HWIN WM_GetTopWindow(void);
void WM_AttachWindow(WM_HWIN hWin, WM_HWIN hParent);
GUI_RESULT WM_SetWindowFocus(WM_HWIN hWin);
WM_HWIN WM_GetWindowFocus(void);
GUI_RESULT WM_SetActiveWindow(WM_HWIN hWin);
WM_HWIN WM_GetActiveWindow(void);
GUI_RESULT WM_SetForegroundWindow(WM_HWIN hWin);
WM_HWIN WM_GetForegroundWindow(void);
void WM_MoveToBottom(WM_HWIN hWin);
void WM_MoveToTop(WM_HWIN hWin);
void WM_SetStickWindow(WM_HWIN hWin);
void WM_ResetStickWindow(WM_HWIN hWin);
void WM_SetBackgroundWindow(WM_HWIN hWin);
WM_HWIN WM_CreateWindowAsChild(int x0, int y0, int xSize, int ySize,
    WM_HWIN hParent, int Style, int Id, WM_CALLBACK *WinCb, u_32 bytes);
void WM_DeleteWindow(WM_HWIN hWin);
void WM_InvalidateRect(WM_HWIN hWin, GUI_RECT *pRect);
void WM_Invalidate(WM_HWIN hWin);
GUI_RESULT WM_InvalidTree(WM_HWIN hWin);
GUI_RECT * WM_GetWindowRect(WM_HWIN hWin);
GUI_RECT * WM_GetWindowInvalidRect(WM_HWIN hWin);
GUI_RESULT WM_FindWindow(WM_HWIN hWin);
WM_HWIN WM_GetDialogItem(WM_HWIN hWin, int Id);
WM_HWIN WM_GetWindowHandle(int Id);
int WM_GetDialogId(WM_HWIN hWin);
WM_HWIN WM_GetExposedWindow(int x, int y);
WM_HWIN WM_GetParentHandle(WM_HWIN hWin);
WM_HWIN WM_GetDsektopWindow(WM_HWIN hWin);
WM_HWIN WM_GetFrontHandle(WM_HWIN hWin);
void WM_MoveWindow(WM_HWIN hWin, int dx, int dy);
void WM_InvalidCoverWindow(WM_HWIN hWin, GUI_RECT *pRect);
WM_HWIN WM_GetClientWindow(WM_HWIN hWin);
GUI_RECT * WM_GetClientRect(WM_HWIN hWin);
GUI_BOOL WM_DefaultKeyProc(GUI_MESSAGE *pMsg);
void WM_DefaultProc(GUI_MESSAGE *pMsg);
void WM_SetTransWindow(WM_HWIN hWin, u_8 Status);

#endif /* __GUI_WM_H */
