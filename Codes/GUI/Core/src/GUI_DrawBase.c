/*****************************************************************************
 * 本文件是S-GUI的底层绘图代码, 主要负责图形裁剪并绘制到屏幕
 *****************************************************************************/

#include "GUI_DrawBase.h"
#include "GUI.h"

static void set_pixel(int x, int y, GUI_COLOR color);
static GUI_COLOR get_pixel(int x, int y);
static void draw_hline(int x0, int y, int x1, GUI_COLOR color);
static void draw_vline(int x, int y0, int y1, GUI_COLOR color);
static void fill_rect(int x0, int y0, int x1, int y1, GUI_COLOR color);

/* default graphic layer APIs. */
static GUI_GLAPI _gl_apis = {
    .setPixel = set_pixel,
    .getPixel = get_pixel,
    .drawHLine = draw_hline,
    .drawVLine = draw_vline,
    .fillRect = fill_rect
};

/* GL APIs pionter. */
GUI_GLAPI *gui_gl_apis = &_gl_apis;

#define clip_x(r, a, b) \
{ \
    if (a < (r)->x0) a = (r)->x0; \
    if (b > (r)->x1) b = (r)->x1; \
}

#define clip_y(r, a, b) \
{ \
    if (a < (r)->y0) a = (r)->y0; \
    if (b > (r)->y1) b = (r)->y1; \
}

#define check_x(r, x) { if ((x) < (r)->x0 || (x) > (r)->x1) return; }
#define check_y(r, y) { if ((y) < (r)->y0 || (y) > (r)->y1) return; }

/* draw pixel, clip. */
static void set_pixel_clip(int x, int y, GUI_COLOR color)
{
    GUI_RECT *r = GUI_CurrentClipRect();

    check_x(r, x);
    check_y(r, y);
    HL_SetPixel(x, y, color);
}

/* draw pixel, ergodic all clip-areas. */
static void set_pixel(int x, int y, GUI_COLOR color)
{
    GUI_RECT r;

    GUI_ClientToScreen(&x, &y);
    GUI_Val2Rect(&r, x, y, 1, 1);
    GUI_PaintAreaInit(&r);
    while (GUI_NextPaintArea()) { /* ergodic all clip-areas. */
        set_pixel_clip(x, y, color);
    }
}

/* read pixel. */
static GUI_COLOR get_pixel(int x, int y)
{
    GUI_ClientToScreen(&x, &y);
    /* clip to screen. */
    if (x >= 0 && x < GUI_GetScreenWidth() &&
        y >= 0 && y < GUI_GetScreenHeight()) {
        return HL_GetPixel(x, y);
    }
    return 0;
}

/* draw horizontal line, clip. */
static void draw_hline_clip(int x0, int y, int x1, GUI_COLOR color)
{
    GUI_RECT *r = GUI_CurrentClipRect();

    check_y(r, y);
    clip_x(r, x0, x1);
    if (x0 > x1) return;
    HL_DrawHLine(x0, y, x1, color);
}

/* draw horizontal line, ergodic all clip-areas. */
static void draw_hline(int x0, int y, int x1, GUI_COLOR color)
{
    if (x0 <= x1) {
        GUI_RECT r = { x0, y, x1, y };

        GUI_ClientToScreenRect(&r);
        GUI_PaintAreaInit(&r);
        while (GUI_NextPaintArea()) { /* ergodic all clip-areas. */
            draw_hline_clip(r.x0, r.y0, r.x1, color);
        }
    }
}

/* draw vertical line, clip. */
static void draw_vline_clip(int x, int y0, int y1, GUI_COLOR color)
{
    GUI_RECT *r = GUI_CurrentClipRect();

    check_x(r, x);
    clip_y(r, y0, y1);
    if (y0 > y1) return;
    HL_DrawVLine(x, y0, y1, color);
}

/* draw vertical line, ergodic all clip-areas. */
static void draw_vline(int x, int y0, int y1, GUI_COLOR color)
{
    if (y0 <= y1) {
        GUI_RECT r = { x, y0, x, y1 };

        GUI_ClientToScreenRect(&r);
        GUI_PaintAreaInit(&r);
        while (GUI_NextPaintArea()) { /* ergodic all clip-areas. */
            draw_vline_clip(r.x0, r.y0, r.y1, color);
        }
    }
}

/* fill rect, clip. */
static void fill_rect_clip(int x0, int y0, int x1, int y1, GUI_COLOR color)
{
    GUI_FLIPOUT Cmd;
    GUI_RECT *r = GUI_CurrentClipRect();

    clip_x(r, x0, x1);
    clip_y(r, y0, y1);
    Cmd.x0 = x0;
    Cmd.y0 = y0;
    Cmd.x1 = x1;
    Cmd.y1 = y1;
    Cmd.Color = color;
    gui_graph_dev->fillRect(&Cmd);
}

/* fill rect, ergodic all clip-areas. */
static void fill_rect(int x0, int y0, int x1, int y1, GUI_COLOR color)
{
    GUI_RECT r = { x0, y0, x1, y1 };

    GUI_ClientToScreenRect(&r);
    GUI_PaintAreaInit(&r);
    while (GUI_NextPaintArea()) { /* ergodic all clip-areas. */
        fill_rect_clip(r.x0, r.y0, r.x1, r.y1, color);
    }
}
