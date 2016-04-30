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

u_8 GUI_GetNextAreaP(GUI_RECT ** p);
/* 画点 */
void GUI_DrawPoint(i_16 x,i_16 y,GUI_COLOR Color)
{
    GUI_RECT r1, *r2;

    RETURN_TRANSPARENT();
    GUI_Val2Rect(&r1, x, y, 1, 1);
    GUI_DrawAreaInit(&r1);
    while (GUI_GetNextAreaP(&r2)) { /* 遍历所有的显示区域 */
        if (x >= r2->x0 && x <= r2->x1 && y >= r2->y0 && y <= r2->y1) {
            if (Color >> 24) {
                Color = GUI_AlphaBlend(Color, GUI_ReadPixel(x, y), Color >> 24);
            }
            GUI_DrawPixel(x, y, Color);
        }
    }
}

/* 读取点 */
GUI_COLOR GUI_ReadPoint(i_16 x, i_16 y)
{
    GUI_RECT *r;
    
    r = GUI_GetNowArea();
    if (r && x >= r->x0 && x <= r->x1 &&
        y >= r->y0 && y <= r->y1) {
            return GUI_ReadPixel(x, y);
    }
    return 0x00000000;
}

/* 画有透明效果的垂直线 */
static void __VertLineAlpha(i_16 x0, i_16 y0, u_16 len, GUI_COLOR Color)
{
    u_16 Alpha = Color >> 24;
    GUI_COLOR tColor;
    
    while (len--) {
        tColor = GUI_AlphaBlend(Color, GUI_ReadPixel(x0, y0 + len), Alpha);
        GUI_DrawPixel(x0, y0 + len, tColor);
    }    
}

/* 画非透明垂直线 */
static void __VertLine(i_16 x0, i_16 y0, u_16 len, GUI_COLOR Color)
{
    while (len--) {
        GUI_DrawPixel(x0, y0 + len, Color);
    }
}

/* 画垂直线
 * x0,y0:坐标
 * len:线长度
 * color:颜色
 **/
void GUI_VertLine(i_16 x0,i_16 y0,u_16 len,GUI_COLOR Color)
{
    GUI_RECT r1, r2;

    RETURN_TRANSPARENT();
    if (GUI_Val2Rect(&r1, x0, y0, 1, len) == FALSE) {
        return; /* 长度为0 */
    }
    GUI_DrawAreaInit(&r1);
    while (GUI_GetNextArea(&r2)) { /* 遍历所有的显示区域 */
        if (GUI_RectOverlay(&r2, &r2, &r1)) {
            x0 = r2.x0;
            y0 = r2.y0;
            len = r2.y1 - r2.y0 + 1;
            if (Color >> 24) {
                __VertLineAlpha(x0, y0, len, Color);
            } else {
                __VertLine(x0, y0, len, Color);
            }
        }
    }
}

/*  
 * 画有透明效果的水平线
 **/
static void __HoriLineAlpha(i_16 x0,i_16 y0,u_16 len,GUI_COLOR Color)
{
    u_16 Alpha = Color >> 24;
    GUI_COLOR tColor;
    while (len--) {
        tColor = GUI_AlphaBlend(Color, GUI_ReadPixel(x0 + len, y0), Alpha);
        GUI_DrawPixel(x0 + len, y0, tColor);
    }
}

/* 画非透明水平线
 * x0,y0:坐标
 * len:线长度
 * color:颜色
 **/
static void __HoriLine(i_16 x0,i_16 y0,u_16 len,GUI_COLOR Color)
{
    while (len--) {
        GUI_DrawPixel(x0 + len, y0, Color);
    }    
}

/* 画水平线
 * x0,y0:坐标
 * len:线长度
 * color:颜色
 **/
void GUI_HoriLine(i_16 x0,i_16 y0,u_16 len,GUI_COLOR Color)
{
    GUI_RECT r1, r2;

    RETURN_TRANSPARENT();
    if (GUI_Val2Rect(&r1, x0, y0, len, 1) == FALSE) {
        return; /* 长度为0 */
    }
    GUI_DrawAreaInit(&r1);
    while (GUI_GetNextArea(&r2)) { /* 遍历所有的显示区域 */
        if (GUI_RectOverlay(&r2, &r2, &r1)) {
            x0 = r2.x0;
            y0 = r2.y0;
            len = r2.x1 - r2.x0 + 1;
            if (Color >> 24) {
                __HoriLineAlpha(x0, y0, len, Color);
            } else {
                __HoriLine(x0, y0, len, Color);
            }
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

/* 填充矩形 */
void GUI_FillRect(i_16 x0, i_16 y0, u_16 xSize, u_16 ySize, GUI_COLOR Color)
{                                     
    GUI_RECT r1, r2;
    
    RETURN_TRANSPARENT();
    /* 将矩形坐标转换为结构体 */
    if (GUI_Val2Rect(&r1, x0, y0, xSize, ySize) == FALSE) {
        return; /* 非法 */
    }
    GUI_DrawAreaInit(&r1);
    while (GUI_GetNextArea(&r2)) { /* 遍历所有的显示区域 */
        if (GUI_RectOverlay(&r2, &r2, &r1)) {
            x0 = r2.x0;
            y0 = r2.y0;
            xSize = r2.x1;
            ySize = r2.y1;
            #if GUI_USE_GRAPHPHY
            LCD_FillRect(x0, y0, xSize, ySize, Color);
            #else
            if (Color >> 24) {
                __FillRectAlpha(x0, y0, xSize, ySize, Color);
            } else {
                __FillRect(x0, y0, xSize, ySize, Color);
            }
            #endif
        }
    }
}

/* 画任意抗锯齿线 */
void GUI_DrawLineNoSaw(i_16 x0, i_16 y0, i_16 x1, i_16 y1, GUI_COLOR Color)
{
    signed char sgn = 1;
    i_16 err = 0, dx = x1 - x0, dy = y1 - y0, d;
    GUI_COLOR Alpha = 0;

    if (dx == 0) { /* 垂直线 */
        if (dy < 0) {
            dy = -dy;
            y0 = y1;
        }
        GUI_VertLine(x0, y0, dy + 1, Color);
        return;
    } else if (dy == 0) { /* 水平线 */
        if (dx < 0) {
            dx = -dx;
            x0 = x1;
        }
        GUI_HoriLine(x0, y0, dx + 1, Color);
        return;
    }
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
            if (err < 0) {
                y1 = y0 - sgn;
                Alpha = (-err << 8) / d;
            } else {
                y1 = y0 + sgn;
                Alpha = (err << 8) / d;
            }
            GUI_DrawPoint(x0, y0, Color | Alpha << 24);
            GUI_DrawPoint(x0, y1, Color | (255 - Alpha) << 24);
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
            if (err < 0) {
                x1 = x0 - sgn;
                Alpha = (-err << 8) / d;
            } else {
                x1 = x0 + sgn;
                Alpha = (err << 8) / d;
            }
            GUI_DrawPoint(x0, y0, Color | Alpha << 24);
            GUI_DrawPoint(x1, y0, Color | (255 - Alpha) << 24);
            ++y0;
            err += dx << 1;
            if (err > dy) {
                err -= dy << 1;
                x0 += sgn;
            }
        }
    }
}

/* 画任意线 */
void GUI_DrawLine(i_16 x0, i_16 y0, i_16 x1, i_16 y1, GUI_COLOR Color)
{
    signed char sgn = 1;
    i_16 err = 0, dx = x1 - x0, dy = y1 - y0, d;

    if (dx == 0) { /* 垂直线 */
        if (dy < 0) {
            dy = -dy;
            y0 = y1;
        }
        GUI_VertLine(x0, y0, dy + 1, Color);
        return;
    } else if (dy == 0) { /* 水平线 */
        if (dx < 0) {
            dx = -dx;
            x0 = x1;
        }
        GUI_HoriLine(x0, y0, dx + 1, Color);
        return;
    }
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

/* 画圆 */

/* 画圆弧 */
