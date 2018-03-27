#include "GUI_PaintArea.h"
#include "GUI_WM.h"
#include "GUI_ClipArea.h"
#include "GUI_Rect.h"

/* 绘制时默认的上下文*/
static GUI_PAINTCONTEXT paint_context;

/* 备份默认的上下文 */
void GUI_BackupContext(void)
{
    paint_context.font = gui_context.font;
    paint_context.background = gui_context.background;
    paint_context.foreground = gui_context.foreground;
    paint_context.fontColor = gui_context.fontColor;
    paint_context.penSize = gui_context.penSize;
}

/* 同步绘制上下文 */
void GUI_SyncContext(void)
{
    gui_context.font = paint_context.font;
    gui_context.background = paint_context.background;
    gui_context.foreground = paint_context.foreground;
    gui_context.fontColor = paint_context.fontColor;
    gui_context.penSize = paint_context.penSize;
}

/* 填充图形上下文 */
void GUI_SetContext(GUI_HWIN hWin)
{
    GUI_RECT *r;
    GUI_AREA Area;

    Area = GUI_GetWindowClipArea(hWin); /* 获取窗口的剪切域 */
    r = WM_GetWindowRect(hWin);
    gui_context.hWin = hWin;
    gui_context.area = Area;
    gui_context.invalidRect = WM_GetWindowInvalidRect(hWin);
    gui_context.winPos.x = r->x0;
    gui_context.winPos.y = r->y0;
}

/* 初始化绘制区域 */
void GUI_PaintAreaInit(GUI_RECT *p)
{
    GUI_RECT *pInvalid = gui_context.invalidRect;

    if (GUI_RectOverlay(&gui_context.drawRect, pInvalid, p)) {
        gui_context.pAreaNode = gui_context.area;
    } else {
        gui_context.pAreaNode = NULL; /* 绘图区域与当前的有效绘制区域不相交 */
    }
}

/* 获取下一个绘制矩形 */
GUI_BOOL GUI_NextPaintArea(void)
{
    GUI_BOOL res = FALSE;
    GUI_AREA area;

    while (gui_context.pAreaNode && res == FALSE) { /* 直到找到下一个相交的矩形 */
        area = gui_context.pAreaNode;
        gui_context.pAreaNode = area->pNext;
        res = GUI_RectOverlay(&gui_context.clipRect, &gui_context.drawRect, &area->Rect);
    }
    return res;
}
