#include "GUI_Context.h"
#include "GUI.h"

GUI_CONTEXT gui_context;     /* GUI上下文 */

/* 转换到屏幕坐标 */
void GUI_ClientToScreen(int *x, int *y)
{
    *x += gui_context.winPos.x;
    *y += gui_context.winPos.y;
}

/* 矩形转换到屏幕坐标 */
void GUI_ClientToScreenRect(GUI_RECT *pRect)
{
    pRect->x0 += gui_context.winPos.x;
    pRect->y0 += gui_context.winPos.y;
    pRect->x1 += gui_context.winPos.x;
    pRect->y1 += gui_context.winPos.y;
}

/* 转换到窗口坐标 */
void GUI_ScreenToClient(int *x, int *y)
{
    *x -= gui_context.winPos.x;
    *y -= gui_context.winPos.y;
}

/* 获取当前窗口在窗口坐标系下的矩形 */
void GUI_GetClientRect(GUI_RECT *pRect)
{
    GUI_RECT *p;

    p = WM_GetWindowRect(gui_context.hWin);
    pRect->x0 = 0;
    pRect->y0 = 0;
    pRect->x1 = p->x1 - p->x0;
    pRect->y1 = p->y1 - p->y0;
}
