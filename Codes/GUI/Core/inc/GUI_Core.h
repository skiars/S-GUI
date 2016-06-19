#ifndef __GUI_CORE_H
#define __GUI_CORE_H

#include "GUI_Config.h"
#include "GUI_Typedef.h"
#include "GUI_Botton.h"
#include "GUI_Queue.h"
#include "GUI_Font.h"

#define GUI_ALIGN_TOP     0x01 /* 顶部对齐 */
#define GUI_ALIGN_BOTTOM  0x02 /* 底部对齐 */
#define GUI_ALIGN_VCENTER 0x04 /* 垂直中心对齐 */
#define GUI_ALIGN_LEFT    0x08 /* 左对齐 */
#define GUI_ALIGN_RIGHT   0x10 /* 右对齐 */
#define GUI_ALIGN_HCENTER 0x20 /* 水平对齐 */

#define GUI_HEAP_FAST     0 /* 快速的堆(MCU内部的SRAM)页面 */
#define GUI_HEAP_HCAP     1 /* 大容量的堆(比如SDRAM)页面 */

#define _hRootWin   GUI_RootWin

#define _ClipRect   (GUI_Context.ClipRect)

#define GUI_DEBUG_OUT(s) GUI_DebugOut(s);

#define GUI_malloc(S)     GUI_Malloc((S), GUI_Heap[GUI_HEAP_HCAP])
#define GUI_free(P)       GUI_Free((P), GUI_Heap[GUI_HEAP_HCAP])
#define GUI_fastmalloc(S) GUI_Malloc((S), GUI_Heap[GUI_HEAP_FAST])
#define GUI_fastfree(P)   GUI_Free((P), GUI_Heap[GUI_HEAP_FAST])

/* 矩形链表 */
typedef struct AREA_NODE {
    GUI_RECT Rect;
    struct AREA_NODE *pNext;
} AREA_NODE, *GUI_AREA;

/* GUI上下文结构体 */
typedef struct {
    GUI_HWIN hWin;          /* 当前绘制窗口 */
    GUI_POINT WinPos;       /* 窗口原点绝对坐标 */
    GUI_HWIN hFocus;        /* 当前的输入焦点 */
    GUI_RECT *InvalidRect;  /* 当前窗口的的无效矩形 */
    GUI_RECT DrawRect;      /* 当前要绘制图形范围的矩形 */
    GUI_RECT ClipRect;      /* 当前屏幕上的裁剪矩形 */
    GUI_AREA Area;          /* 当前绘制窗口的剪切域 */
    GUI_AREA pAreaNode;     /* 当前的剪切域节点 */
    GUI_FONT *Font;         /* 当前字体 */
    GUI_COLOR FGColor;      /* 当前前景色 */
    GUI_COLOR BGColor;      /* 当前背景色 */
    GUI_COLOR FontColor;    /* 当前字体颜色 */
}GUI_CONTEXT;

extern void *GUI_Heap[2];
extern GUI_CONTEXT GUI_Context;
extern GUI_HWIN GUI_RootWin;
extern GUI_AREA GUI_AreaHeap;

GUI_RESULT GUI_Init(void);
void GUI_Unload(void);
void GUI_ScreenSize(u_16 *xSize, u_16 *ySize);
u_16 GUI_GetScreenWidth(void);
u_16 GUI_GetScreenHeight(void);
void GUI_Delay(GUI_TIME tms);
void GUI_LOCK(void);
void GUI_UNLOCK(void);
void GUI_DebugOut(const char *s);
GUI_BOOL GUI_StartPaint(GUI_HWIN hWin, GUI_CONTEXT *Backup);
void GUI_EndPaint(GUI_CONTEXT *Backup);
GUI_AREA GUI_CurrentClipArea(void);
void GUI_DrawAreaInit(GUI_RECT *p);
GUI_BOOL GUI_GetNextArea(void);
void GUI_ClientToScreen(i_16 *x, i_16 *y);
void GUI_ScreenToClient(i_16 *x, i_16 *y);
void GUI_ClientToScreenRect(GUI_RECT *pRect);
void GUI_GetClientRect(GUI_RECT *pRect);
void GUI_SetFont(GUI_FONT *Font);
void GUI_SetBackgroundColor(GUI_COLOR Color);
void GUI_SetForegroundColor(GUI_COLOR Color);
void GUI_SetFontColor(GUI_COLOR Color);

#endif /* __GUI_CORE_H */
