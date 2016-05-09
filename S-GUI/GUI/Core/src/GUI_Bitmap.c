#include "GUI_Bitmap.h"
#include "GUI.h"

/* 绘制位图，不支持透明效果 */
void GUI_DrawBitmap24b(i_16 x0,
    i_16 y0,
    u_16 xSize,
    u_16 ySize,
    const unsigned char *pPixel,
    const LCD_LOGPALETTE *pLogPal,
    int xMag,
    int yMag)
{
    i_16 x1, y1, x = x0, y = y0;
    u_32 Offset;
    GUI_RECT r1;

    xSize = (u_16)GUI_MIN(xSize, xMag); /* 图片宽度 */
    ySize = (u_16)GUI_MIN(ySize, yMag); /* 图片高度 */
    GUI_Val2Rect(&r1, x0, y0, xSize, ySize);
    GUI_DrawAreaInit(&r1);
    while (GUI_GetNextArea()) { /* 遍历所有的显示区域 */
        x0 = r1.x0;
        y0 = r1.y0;
        x1 = r1.x1;
        y1 = r1.y1;
        CLIP_X(x0, x1);
        CLIP_Y(y0, y1);
        Offset = (u_32)((y0 - y) * xMag + (x0 - x)) * 3;
        LCD_DrawBitmap(GUI_RGB888, pPixel + Offset,
            x0, y0, x1 - x0 + 1, y1 - y0 + 1, xMag - (x1 - x0 + 1));
    }
}

/* 绘制位图，不支持透明效果 */
void GUI_DrawBitmap16b(i_16 x0,
    i_16 y0,
    u_16 xSize,
    u_16 ySize,
    const unsigned char *pPixel,
    const LCD_LOGPALETTE *pLogPal,
    int xMag,
    int yMag)
{
    i_16 x = x0, y = y0, x1 = x0 + xSize - 1, y1 = y0 + ySize - 1;
    u_32 Offset, PixelBytes = 2;
    GUI_RECT r1;

    xSize = (u_16)GUI_MIN(xSize, xMag); /* 图片宽度 */
    ySize = (u_16)GUI_MIN(ySize, yMag); /* 图片高度 */
    GUI_Val2Rect(&r1, x0, y0, xSize, ySize);
    GUI_DrawAreaInit(&r1);
    while (GUI_GetNextArea()) { /* 遍历所有的显示区域 */
        CLIP_X(x, x1);
        CLIP_Y(y, y1);
        Offset = (u_32)((y - y0) * xMag + (x - x0)) * PixelBytes;
        xSize = x1 - x + 1;
        ySize = y1 - y + 1;
#if GUI_USE_GRAPHPHY
        LCD_DrawBitmap(GUI_RGB565, pPixel + Offset,
            x, y, xSize, ySize, xMag - xSize);
#else
        _DrawBitmap24bp(pPixel + Offset, x,
            y, xSize, ySize, (u_16)(xMag - xSize));
#endif
    }
}

void GUI_DrawBitmap(i_16 x0,
    i_16 y0,
    u_16 xSize,
    u_16 ySize,
    const GUI_BITMAP *Bmp)
{
    if (Bmp->pDraw) {
        Bmp->pDraw(x0, y0, xSize, ySize,
            Bmp->pData, Bmp->pPal, Bmp->xSize, Bmp->ySize);
    }
}
