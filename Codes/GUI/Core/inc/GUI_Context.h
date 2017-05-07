#ifndef __GUI_CONTEXT_H
#define __GUI_CONTEXT_H

#include "GUI_Typedef.h"
#include "GUI_Font.h"
#include "GUI_ClipAreaHeap.h"

/* GUI上下文结构体 */
typedef struct {
    GUI_HWIN hWin;          /* 当前绘制窗口 */
    GUI_POINT winPos;       /* 窗口原点绝对坐标 */
    GUI_HWIN hActive;       /* 当前活动窗口 */
    GUI_RECT *invalidRect;  /* 当前窗口的的无效矩形 */
    GUI_RECT drawRect;      /* 当前要绘制图形范围的矩形 */
    GUI_RECT clipRect;      /* 当前屏幕上的裁剪矩形 */
    GUI_AREA area;          /* 当前绘制窗口的剪切域 */
    GUI_AREA pAreaNode;     /* 当前的剪切域节点 */
    GUI_FONT *font;         /* 当前字体 */
    GUI_COLOR foreground;   /* 当前前景色 */
    GUI_COLOR background;   /* 当前背景色 */
    GUI_COLOR fontColor;    /* 当前字体颜色 */
    int penSize;            /* 绘制线宽 */
}GUI_CONTEXT;

extern GUI_CONTEXT gui_context;   /* GUI context */

/* inline functions */

/* set current font */
static inline void GUI_SetFont(GUI_FONT *Font)
{
    gui_context.font = Font;
}

/* get current font */
static inline GUI_FONT* GUI_CurrentFont(void)
{
    return gui_context.font;
}

/* get current background color */
static inline GUI_COLOR GUI_CurrentBackground(void)
{
    return gui_context.background;
}

/* set current background color */
static inline void GUI_SetBackground(GUI_COLOR Color)
{
    gui_context.background = Color;
}

/* get current foreground color */
static inline GUI_COLOR GUI_CurrentForeground(void)
{
    return gui_context.foreground;
}

/* set current foreground color */
static inline void GUI_SetForeground(GUI_COLOR Color)
{
    gui_context.foreground = Color;
}

/* set current font color */
static inline void GUI_SetFontColor(GUI_COLOR Color)
{
    gui_context.fontColor = Color;
}

/* get current font color */
static inline GUI_COLOR GUI_CurrentFontColor(void)
{
    return gui_context.fontColor;
}

/* set current brush size */
static inline void GUI_SetPenSize(int Width)
{
    gui_context.penSize = Width;
}

/* get current brush size */
static inline int GUI_PenSize(void)
{
    return gui_context.penSize;
}

/* get current clip-rect */
static inline GUI_RECT* GUI_CurrentClipRect(void)
{
    return &gui_context.clipRect;
}

/* set current active window */
static inline void GUI_SetActiveWindow(GUI_HWIN hWin)
{
    gui_context.hActive = hWin;
}

/* get current active window */
static inline GUI_HWIN GUI_CurrentActiveWindow(void)
{
    return gui_context.hActive;
}

/* get current paint window */
static inline GUI_HWIN GUI_CurrentPaintWindow(void)
{
    return gui_context.hWin;
}

/* get current clip-area */
static inline GUI_AREA GUI_CurrentClipArea(void)
{
    return gui_context.area;
}

void GUI_ClientToScreen(int *x, int *y);
void GUI_ClientToScreenRect(GUI_RECT *pRect);
void GUI_ScreenToClient(int *x, int *y);
void GUI_GetClientRect(GUI_RECT *pRect);

#endif
