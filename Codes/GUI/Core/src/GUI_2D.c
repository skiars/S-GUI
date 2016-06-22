/*****************************************************************************
 * S-GUI 2D图形库
 *****************************************************************************/

#include "GUI_2D.h"
#include "GUI.h"

/* 全透明直接返回 */
#define RETURN_TRANSPARENT() \
    if (GUI_Context.FGColor >> 24 == 0xff) return;

#define FG_COLOR (GUI_Context.FGColor)
#define BG_COLOR (GUI_Context.BGColor)

static void _SetPixelClip(i_16 x, i_16 y)
{
    GUI_COLOR Color = FG_COLOR;

    CHECK_X(x);
    CHECK_Y(y);
    if (Color >> 24) {
        Color = GUI_AlphaBlend(Color, GL_GetPixel(x, y), Color >> 24);
    }
    GL_SetPixel(x, y, Color);
}

/* 画直径为1的点 */
static void _DrawPoint(i_16 x, i_16 y)
{
    GUI_RECT r1;

    RETURN_TRANSPARENT();
    GUI_ClientToScreen(&x, &y); /* 转换到屏幕坐标系 */
    GUI_Val2Rect(&r1, x, y, 1, 1);
    GUI_DrawAreaInit(&r1);
    while (GUI_GetNextArea()) { /* 遍历所有的显示区域 */
        _SetPixelClip(x, y);
    }
}

/* 画点 */
void GUI_DrawPoint(i_16 x,i_16 y)
{
    if (GUI_Context.LineWidth > 1) {
        GUI_FillCircle(x, y, GUI_Context.LineWidth / 2);
    } else {
        _DrawPoint(x, y);
    }
}

static void _VertLine(i_16 x0, i_16 y0, u_16 len)
{
    GUI_RECT r;

    RETURN_TRANSPARENT();
    GUI_ClientToScreen(&x0, &y0); /* 转换到屏幕坐标系 */
    if (GUI_Val2Rect(&r, x0, y0, 1, len)) {
        GUI_DrawAreaInit(&r);
        while (GUI_GetNextArea()) { /* 遍历所有的显示区域 */
            GL_DrawVLine(x0, y0, r.y1);
        }
    }
}

/* 画垂直线
 * x0,y0:坐标
 * len:线长度
 * color:颜色
 **/
void GUI_VertLine(i_16 x0,i_16 y0,u_16 len)
{
    int w = GUI_Context.LineWidth;
    
    if (w > 1) {
        GUI_FillRect(x0, y0, w, len);
    } else {
        _VertLine(x0, y0, len);
    }
}

static void _HoriLine(i_16 x0, i_16 y0, u_16 len)
{
    GUI_RECT r;

    RETURN_TRANSPARENT();
    GUI_ClientToScreen(&x0, &y0); /* 转换到屏幕坐标系 */
    if (GUI_Val2Rect(&r, x0, y0, len, 1)) {
        GUI_DrawAreaInit(&r);
        while (GUI_GetNextArea()) { /* 遍历所有的显示区域 */
            GL_DrawHLine(x0, y0, r.x1);
        }
    }
}

/* 画水平线
 * x0,y0:坐标
 * len:线长度
 * color:颜色
 **/
void GUI_HoriLine(i_16 x0, i_16 y0, u_16 len)
{
    int w = GUI_Context.LineWidth;

    if (w > 1) {
        GUI_FillRect(x0, y0, len, w);
    } else {
        _HoriLine(x0, y0, len);
    }
}

static void _DrawRect(i_16 x0, i_16 y0, i_16 x1, i_16 y1)
{
    GL_DrawHLine(x0, y0, x1);
    GL_DrawHLine(x0, y1, x1);
    GL_DrawVLine(x0, y0 + 1, y1 - 1);
    GL_DrawVLine(x1, y0 + 1, y1 - 1);
}

/* 画矩形框 */
void GUI_DrawRect(i_16 x0, i_16 y0, u_16 xSize, u_16 ySize)
{
    GUI_RECT r;

    GUI_ClientToScreen(&x0, &y0); /* 转换到屏幕坐标系 */
    if (GUI_Val2Rect(&r, x0, y0, xSize, ySize)) {
        GUI_DrawAreaInit(&r);
        while (GUI_GetNextArea()) {
            _DrawRect(x0, y0, r.x1, r.y1);
        }
    }
}

/* 填充矩形 */
void GUI_FillRect(i_16 x0, i_16 y0, u_16 xSize, u_16 ySize)
{
    GUI_RECT r;
    
    RETURN_TRANSPARENT();
    GUI_ClientToScreen(&x0, &y0); /* 转换到屏幕坐标系 */
    /* 将矩形坐标转换为结构体 */
    if (GUI_Val2Rect(&r, x0, y0, xSize, ySize)) {
        GUI_DrawAreaInit(&r);
        while (GUI_GetNextArea()) { /* 遍历所有的显示区域 */
            GL_FillRect(x0, y0, r.x1, r.y1);
        }
    }
}

/* 画任意直线, 文件内部调用 */
static void _DrawLine(i_16 x0, i_16 y0, i_16 x1, i_16 y1)
{
    int dx, dy, p, diff, addx, addy, i;

    if (x0 < x1) {
        dx = x1 - x0;
        addx = 1;
    } else {
        dx = x0 - x1;
        addx = -1;
    }
    if (y0 < y1) {
        dy = y1 - y0;
        addy = 1;
    } else {
        dy = y0 - y1;
        addy = -1;
    }
    if (dx >= dy) {
        dy <<= 1;
        p = dy - dx;
        diff = p - dx;
        for (i = 0; i <= dx; ++i) {
            _SetPixelClip(x0, y0);
            x0 += addx;
            if (p < 0) {
                p += dy;
            } else {
                y0 += addy;
                p += diff;
            }
        }
    } else {
        dx <<= 1;
        p = dx - dy;
        diff = p - dy;
        for (i = 0; i <= dy; ++i) {
            _SetPixelClip(x0, y0);
            y0 += addy;
            if (p < 0) {
                p += dx;
            } else {
                x0 += addx;
                p += diff;
            }
        }
    }
}

/* 画任意直线, 线宽大于1 */
static void _DrawLineW(i_16 x0, i_16 y0, i_16 x1, i_16 y1)
{
    int dx, dy, p, diff, addx, addy, i;
    int w = GUI_Context.LineWidth;

    if (x0 < x1) {
        dx = x1 - x0;
        addx = 1;
    } else {
        dx = x0 - x1;
        addx = -1;
    }
    if (y0 < y1) {
        dy = y1 - y0;
        addy = 1;
    } else {
        dy = y0 - y1;
        addy = -1;
    }
    /* 线绘制线帽 */

    if (dx >= dy) {
        dy <<= 1;
        p = dy - dx;
        diff = p - dx;
        for (i = 0; i <= dx; ++i) {
            GL_DrawVLine(x0, y0, y0 + w - 1);
            x0 += addx;
            if (p < 0) {
                p += dy;
            } else {
                y0 += addy;
                p += diff;
            }
        }
    } else {
        dx <<= 1;
        p = dx - dy;
        diff = p - dy;
        for (i = 0; i <= dy; ++i) {
            GL_DrawHLine(x0, y0, x0 + w - 1);
            _SetPixelClip(x0, y0);
            y0 += addy;
            if (p < 0) {
                p += dx;
            } else {
                x0 += addx;
                p += diff;
            }
        }
    }
}

/* 画任意直线, 公共代码 */
void GUI_DrawLine(i_16 x0, i_16 y0, i_16 x1, i_16 y1)
{
    int w = GUI_Context.LineWidth, d = w >> 1;;
    GUI_RECT r;
    void (*line)(i_16, i_16, i_16, i_16);

    r.x0 = x0 < x1 ? x0 : x1;
    r.y0 = y0 < y1 ? y0 : y1;
    r.x1 = x0 > x1 ? x0 : x1;
    r.y1 = y0 > y1 ? y0 : y1;
    if (x0 == x1) { /* 竖直线 */
        GUI_VertLine(r.x0, r.y0, r.y1 - r.y0 + 1);
        return;
    }
    if (y0 == y1) { /* 水平线 */
        GUI_HoriLine(r.x0, r.y0, r.x1 - r.x0 + 1);
        return;
    }
    if (w > 1) {
        line = _DrawLineW;
        if (GUI_ABS(x1 - x0) >= GUI_ABS(y1 - y0)) {
            r.y0 -= d;
            r.y1 += w - d;
            y0 -= d;
            y1 -= w - d;
        } else {
            r.x0 -= d;
            r.x1 += w - d;
            x0 -= d;
            x1 -= w - d;
        }
    } else {
        line = _DrawLine;
    }
    GUI_ClientToScreenRect(&r);
    GUI_DrawAreaInit(&r);
    while (GUI_GetNextArea()) {
        line(x0, y0, x1, y1);
    }
}

/* 画8点 */
static void _Draw8Point(int x0, int y0, int x, int y)
{
    
    _SetPixelClip(x0 + x, y0 - y);
    _SetPixelClip(x0 - y, y0 - x);
    _SetPixelClip(x0 - x, y0 + y);
    _SetPixelClip(x0 + y, y0 + x);
    if (x && x != y) {
        _SetPixelClip(x0 - x, y0 - y);
        _SetPixelClip(x0 - y, y0 + x);
        _SetPixelClip(x0 + x, y0 + y);
        _SetPixelClip(x0 + y, y0 - x);
    }
}

static void _DrawCircle(int x0, int y0, int r)
{
    int x, y = r, d = -(r >> 1);

    for (x = 0; x <= y; ++x) {
        if (d < 0) {
            d += x;
        } else {
            d += x - y;
            --y;
        }
        _Draw8Point(x0, y0, x, y);
    }
}

/* 画8点, 画线宽不为1的圆时调用 */
static void _Draw8PointW(int x0, int y0, int x, int y, int y1)
{
    if (y1 - x < 0) {
        y1 = x;
    }
    GL_DrawVLine(x0 + x, y0 - y, y0 - y1);
    GL_DrawHLine(x0 - y, y0 - x, x0 - y1);
    GL_DrawVLine(x0 - x, y0 + y1, y0 + y);
    GL_DrawHLine(x0 + y1, y0 + x, x0 + y);
    if (x) {
        if (y1 == x) {
            y1 = x + 1;
        }
        GL_DrawVLine(x0 - x, y0 - y, y0 - y1);
        GL_DrawHLine(x0 - y, y0 + x, x0 - y1);
        GL_DrawVLine(x0 + x, y0 + y1, y0 + y);
        GL_DrawHLine(x0 + y1, y0 - x, x0 + y);
    }
}

/* 画线宽不为1的圆 */
static void _DrawCircleW(int x0, int y0, int r)
{
    int x, y = r, d = -(r >> 1);
    int r1 = r - GUI_Context.LineWidth + 1;
    int y1 = r1, d1 = -(r1 >> 1);

    for (x = 0; x <= y; ++x) {
        if (d1 < 0) {
            d1 += x;
        } else {
            d1 += x - y1;
            --y1;
        }
        if (d < 0) {
            d += x;
        } else {
            d += x - y;
            --y;
        }
        _Draw8PointW(x0, y0, x, y, y1);
    }
}

/* 画圆 */
void GUI_DrawCircle(i_16 x0, i_16 y0, u_16 r)
{
    int w = GUI_Context.LineWidth;
    GUI_RECT Rect;
    void (*circle)(int, int, int);

    if (w > 1) {
        w >>= 1;
        if (r <= w) {
            GUI_FillCircle(x0, y0, r);
            return;
        }
        circle = _DrawCircleW;
        r += w;
    } else {
        circle = _DrawCircle;
    }
    GUI_ClientToScreen(&x0, &y0); /* 转换到屏幕坐标系 */
    Rect.x0 = x0 - r;
    Rect.x1 = x0 + r;
    Rect.y0 = y0 - r;
    Rect.y1 = y0 + r;
    GUI_DrawAreaInit(&Rect);
    while (GUI_GetNextArea()) { /* 遍历所有的显示区域 */
        circle(x0, y0, r);
    }
}

static void _FillCircle(int x0, int y0, int r)
{
    i_32 i;
    int imax = (r * 707) / 1000 + 1;
    i_32 sqmax = r * r + r / 2;
    int x = r;

    GL_DrawHLine(x0 - r, y0, x0 + r);
    for (i = 1; i <= imax; ++i) {
        if (i * i + x * x > sqmax) {
            /* draw lines from outside */
            if (x > imax) {
                GL_DrawHLine(x0 - i + 1, y0 + x, x0 + i - 1);
                GL_DrawHLine(x0 - i + 1, y0 - x, x0 + i - 1);
            }
            --x;
        }
        /* draw lines from inside (center) */
        GL_DrawHLine(x0 - x, y0 + i, x0 + x);
        GL_DrawHLine(x0 - x, y0 - i, x0 + x);
    }
}

/* 填充圆 */
void GUI_FillCircle(i_16 x0, i_16 y0, i_16 r)
{
    GUI_RECT Rect;

    GUI_ClientToScreen(&x0, &y0); /* 转换到屏幕坐标系 */
    Rect.x0 = x0 - r;
    Rect.x1 = x0 + r;
    Rect.y0 = y0 - r;
    Rect.y1 = y0 + r;
    GUI_DrawAreaInit(&Rect);
    while (GUI_GetNextArea()) { /* 遍历所有的显示区域 */
        _FillCircle(x0, y0, r);
    }
}

void GUI_2DTest(void)
{
    GUI_SetFGColor(0x00ff00ff);
    //GUI_DrawCircle(200, 200, 220);
    GUI_DrawCircle(200, 200, 80);
    //GUI_FillCircle(200, 200, 100);
    GUI_DrawLine(300, 100, 400, 120);
    GUI_DrawLine(300, 100, 400, 250);
    GUI_SetLineWidth(10);
    GUI_DrawLine(270, 100, 150, 300);
    GUI_DrawLine(270, 100, 270, 30);
    GUI_DrawLine(270, 100, 150, 100);
    GUI_DrawCircle(200, 200, 220);
    GUI_SetFGColor(0x000000ff);
    GUI_DrawRect(150, 100, 121, 201);
}
