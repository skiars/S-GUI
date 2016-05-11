/*****************************************************************************
 * S-GUI 2D图形库
 *****************************************************************************/

#include "GUI_2D.h"
#include "GUI.h"

/* 全透明直接返回 */
#define RETURN_TRANSPARENT() \
    if (Color >> 24 == 0xff) return;

/* RGB888格式转换为RGB565格式 */
u_16 GUI_RGB888To565(u_32 RGB)
{
    u_16 R, G, B;
    R = (RGB >> 19) & 0x1F;
    G = (RGB >> 10) & 0x3F;
    B = (RGB >> 3) & 0x1F;
    return (R << 11) | (G << 5) | B;
}

/* RGB565格式转换为RGB888格式 */
u_32 GUI_RGB565To888(u_16 RGB)
{
    u_32 Color;
    Color = ((u_32)RGB & 0xF800) << 8;   /* R */
    Color |= ((u_32)RGB & 0x07E0) << 5;   /* G */
    Color |= ((u_32)RGB & 0x001F) << 3;   /* B */
    return Color;
}

/*
 * Alpha混合
 * BkColor:背景颜色
 * Color:目标颜色
 * Alpha:透明度(0~256)
 **/
GUI_COLOR GUI_AlphaBlend(GUI_COLOR Color, GUI_COLOR BkColor, u_16 Alpha)
{
    /* Calc Color seperations for FgColor first */
    GUI_COLOR R = (BkColor & 0xFF)    * Alpha;
    GUI_COLOR G = (BkColor & 0xFF00)  * Alpha;
    GUI_COLOR B = (BkColor & 0xFF0000)* Alpha;
    /* Add Color seperations for BkColor */
    Alpha = 256 - Alpha;
    R += (Color & 0xFF)     * Alpha;
    G += (Color & 0xFF00)   * Alpha;
    B += (Color & 0xFF0000) * Alpha;
    R = (R >> 8);
    G = (G >> 8) & 0xFF00;
    B = (B >> 8) & 0xFF0000;
    return R | G | B;
}

static void _SetPixel(i_16 x, i_16 y, GUI_COLOR Color)
{
    CHECK_X(x);
    CHECK_Y(y);
    GL_SetPixel(x, y, Color);
}

/* 画点 */
void GUI_DrawPoint(i_16 x,i_16 y,GUI_COLOR Color)
{
    GUI_RECT r1;

    RETURN_TRANSPARENT();
    GUI_Val2Rect(&r1, x, y, 1, 1);
    GUI_DrawAreaInit(&r1);
    while (GUI_GetNextArea()) { /* 遍历所有的显示区域 */
        if (Color >> 24) {
            Color = GUI_AlphaBlend(Color, GL_GetPixel(x, y), Color >> 24);
        }
        _SetPixel(x, y, Color);
    }
}

/* 读取点 */
GUI_COLOR GUI_ReadPoint(i_16 x, i_16 y)
{
    u_16 w = GUI_GetScreenWidth(), h = GUI_GetScreenHeight();
    
    if (x > 0 && x < w && y > 0 && y < h) {
            return GL_GetPixel(x, y);
    }
    return 0x00000000;
}

/* 画有透明效果的垂直线 */
static void __VertLineAlpha(i_16 x0, i_16 y0, u_16 len, GUI_COLOR Color)
{
    i_16  y1 = y0 + len - 1;
    u_16 Alpha = Color >> 24;
    GUI_COLOR tColor;
    
    CHECK_X(x0);
    CLIP_Y(y0, y1);
    len = y1 >= y0 ? y1 - y0 + 1 : 0;
    while (len--) {
        tColor = GUI_AlphaBlend(Color, GL_GetPixel(x0, y0 + len), Alpha);
        GL_SetPixel(x0, y0 + len, tColor);
    }
}

/* 画非透明垂直线 */
static void __VertLine(i_16 x0, i_16 y0, u_16 len, GUI_COLOR Color)
{
    i_16  y1 = y0 + len - 1;

    CHECK_X(x0);
    CLIP_Y(y0, y1);
    len = y1 >= y0 ? y1 - y0 + 1 : 0;
    while (len--) {
        GL_SetPixel(x0, y0 + len, Color);
    }
}

/* 画垂直线
 * x0,y0:坐标
 * len:线长度
 * color:颜色
 **/
void GUI_VertLine(i_16 x0,i_16 y0,u_16 len,GUI_COLOR Color)
{
    GUI_RECT r1;

    RETURN_TRANSPARENT();
    if (GUI_Val2Rect(&r1, x0, y0, 1, len) == FALSE) {
        return; /* 长度为0 */
    }
    GUI_DrawAreaInit(&r1);
    while (GUI_GetNextArea()) { /* 遍历所有的显示区域 */
        if (Color >> 24) {
            __VertLineAlpha(x0, y0, len, Color);
        } else {
            __VertLine(x0, y0, len, Color);
        }
    }
}

/*  
 * 画有透明效果的水平线
 **/
static void __HoriLineAlpha(i_16 x0,i_16 y0,u_16 len,GUI_COLOR Color)
{
    i_16 x1 = x0 + len - 1;
    u_16 Alpha = Color >> 24;
    GUI_COLOR tColor;

    CHECK_Y(y0);
    CLIP_X(x0, x1);
    len = x1 >= x0 ? x1 - x0 + 1 : 0;
    while (len--) {
        tColor = GUI_AlphaBlend(Color, GL_GetPixel(x0 + len, y0), Alpha);
        GL_SetPixel(x0 + len, y0, tColor);
    }
}

/* 画水平线
 * x0,y0:坐标
 * len:线长度
 * color:颜色
 **/
void GUI_HoriLine(i_16 x0,i_16 y0,u_16 len,GUI_COLOR Color)
{
    GUI_RECT r1;

    RETURN_TRANSPARENT();
    if (GUI_Val2Rect(&r1, x0, y0, len, 1) == FALSE) {
        return; /* 长度为0 */
    }
    GUI_DrawAreaInit(&r1);
    while (GUI_GetNextArea()) { /* 遍历所有的显示区域 */
        if (Color >> 24) {
            __HoriLineAlpha(x0, y0, len, Color);
        } else {
            GL_DrawHLine(x0, y0, len, Color);
        }
    }
}

/* 画矩形框 */
void GUI_DrawRect(i_16 x0,i_16 y0,u_16 xSize,u_16 ySize,GUI_COLOR Color)
{
    GUI_HoriLine(x0 + 1, y0, xSize - 2, Color);
    GUI_HoriLine(x0 + 1, y0 + ySize - 1, xSize - 2, Color);
    GUI_VertLine(x0, y0, ySize, Color);
    GUI_VertLine(x0 + xSize - 1, y0, ySize, Color);
}

/* 填充透明矩形 */
#if GUI_USE_GRAPHPHY == 0
static void __FillRectAlpha(i_16 x0, i_16 y0,u_16 x1,u_16 y1,GUI_COLOR Color)
{
    i_16 x, y;
    u_16 Alpha = Color >> 24;
    GUI_COLOR tColor; 
    
    for (y = y0; y <= y1; y++) {
        for (x = x0; x <= x1; x++) {
            tColor = GUI_AlphaBlend(Color, GUI_ReadPixel(x, y), Alpha);
            GUI_DrawPixel(x, y, tColor);
        }
    }
}
#endif

/* 填充非透明矩形 */
#if GUI_USE_GRAPHPHY == 0
static void __FillRect(i_16 x0, i_16 y0, u_16 x1, u_16 y1, GUI_COLOR Color)
{                                     
    i_16 x, y;

    for (y = y0; y <= y1; y++) {
        for (x = x0; x <= x1; ++x) {
            GUI_DrawPixel(x, y, Color);
        }
    }
}
#endif

void _FillRect(i_16 x0, i_16 y0, i_16 x1, i_16 y1, GUI_COLOR Color)
{
    CLIP_X(x0, x1);
    CLIP_Y(y0, y1);
    LCD_FillRect(x0, y0, x1, y1, Color);
}

/* 填充矩形 */
void GUI_FillRect(i_16 x0, i_16 y0, u_16 xSize, u_16 ySize, GUI_COLOR Color)
{
    i_16 x1 = x0 + xSize - 1, y1 = y0 + ySize - 1;
    GUI_RECT r1;
    
    RETURN_TRANSPARENT();
    /* 将矩形坐标转换为结构体 */
    if (GUI_Val2Rect(&r1, x0, y0, xSize, ySize) == FALSE) {
        return; /* 非法 */
    }
    GUI_DrawAreaInit(&r1);
    while (GUI_GetNextArea()) { /* 遍历所有的显示区域 */
        _FillRect(x0, y0, x1, y1, Color);
    }
}

/* 画任意线 */
void GUI_DrawLine(i_16 x0, i_16 y0, i_16 x1, i_16 y1, GUI_COLOR Color)
{
    signed char sgn = 1;
    i_16 err = 0, dx = x1 - x0, dy = y1 - y0, d;

    if (GUI_ABS(dx) >= GUI_ABS(dy)) { /* 斜率小于1 */
        if (dx < 0) { /* 交换坐标 */
            dx = -dx;
            dy = -dy;
            d = x0;
            x0 = x1;
            x1 = d;
            d = y0;
            y0 = y1;
            y1 = d;
        }
        if (dy < 0) {
            dy = -dy;
            sgn = -1;
        }
        d = dx << 1;
        while (x0 <= x1) {
            GUI_DrawPoint(x0, y0, Color);
            ++x0;
            err += dy << 1;
            if (err > dx) {
                err -= dx << 1;
                y0 += sgn;
            }
        }
    } else {
        if (dy < 0) { /* 交换坐标 */
            dx = -dx;
            dy = -dy;
            d = x0;
            x0 = x1;
            x1 = d;
            d = y0;
            y0 = y1;
            y1 = d;
        }
        if (dx < 0) {
            dx = -dx;
            sgn = -1;
        }
        d = dy << 1;
        while (y0 <= y1) {
            GUI_DrawPoint(x0, y0, Color);
            ++y0;
            err += dx << 1;
            if (err > dy) {
                err -= dy << 1;
                x0 += sgn;
            }
        }
    }
}

/* 画8点 */
static void _Draw8Point(int x0, int y0, int x, int y, GUI_COLOR Color)
{
    _SetPixel(x0 + x, y0 + y, Color);
    _SetPixel(x0 - x, y0 + y, Color);
    _SetPixel(x0 + y, y0 + x, Color);
    _SetPixel(x0 + y, y0 - x, Color);
    if (y) {
        _SetPixel(x0 + x, y0 - y, Color);
        _SetPixel(x0 - x, y0 - y, Color);
        _SetPixel(x0 - y, y0 + x, Color);
        _SetPixel(x0 - y, y0 - x, Color);
    }
}

static void _DrawCircle(int x0, int y0, int r, GUI_COLOR Color)
{
    int x, y = r, d = -(r >> 1);

    for (x = 0; x <= y; ++x) {
        if (d < 0) {
            d += x;
        } else {
            d += x - y;
            --y;
        }
        _Draw8Point(x0, y0, x, y, Color);
    }
}

/* 画圆 */
void GUI_DrawCircle(int x0, int y0, int r, GUI_COLOR Color)
{
    GUI_RECT Rect;

    Rect.x0 = x0 - r;
    Rect.x1 = x0 + r;
    Rect.y0 = y0 - r;
    Rect.y1 = y0 + r;
    GUI_DrawAreaInit(&Rect);
    while (GUI_GetNextArea()) { /* 遍历所有的显示区域 */
        _DrawCircle(x0, y0, r, Color);
    }
}

static void _FillCircle(int x0, int y0, int r, GUI_COLOR Color)
{
    i_32 i;
    int imax = (r * 707) / 1000 + 1;
    i_32 sqmax = r * r + r / 2;
    i_32 x = r;

    GL_DrawHLine(x0 - r, y0, r << 1, Color);
    for (i = 1; i <= imax; ++i) {
        if (i * i + x * x > sqmax) {
            /* draw lines from outside */
            if (x > imax) {
                GL_DrawHLine(x0 - i + 1, y0 + x, (i - 1) << 1, Color);
                GL_DrawHLine(x0 - i + 1, y0 - x, (i - 1) << 1, Color);
            }
            --x;
        }
        /* draw lines from inside (center) */
        GL_DrawHLine(x0 - x, y0 + i, x << 1, Color);
        GL_DrawHLine(x0 - x, y0 - i, x << 1, Color);
    }
}

/* 填充圆 */
void GUI_FillCircle(int x0, int y0, int r, GUI_COLOR Color)
{
    GUI_RECT Rect;

    Rect.x0 = x0 - r;
    Rect.x1 = x0 + r;
    Rect.y0 = y0 - r;
    Rect.y1 = y0 + r;
    GUI_DrawAreaInit(&Rect);
    while (GUI_GetNextArea()) { /* 遍历所有的显示区域 */
        _FillCircle(x0, y0, r, Color);
    }
}

void GUI_2DTest(void)
{
    GUI_DrawCircle(200, 200, 220, 0x00000000);
    GUI_DrawCircle(200, 200, 40, 0x00FF0000);
    GUI_FillCircle(200, 300, 50, 0x000000FF);
    GUI_DrawLine(320, 100, 400, 220, 0);
}
