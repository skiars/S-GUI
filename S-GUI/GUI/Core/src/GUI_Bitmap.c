#include "GUI_Bitmap.h"
#include "GUI.h"

#if GUI_USE_GRAPHPHY == 0
static void _DrawBitmap24bp(const unsigned char *pPixel,
    i_16 x0, i_16 y0,
    u_16 xSize, u_16 ySize,
    u_16 Offset)
{

}
#endif

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
    u_32 Offset;
    GUI_RECT r1, r2;

    xSize = (u_16)GUI_MIN(xSize, xMag); /* 图片宽度 */
    ySize = (u_16)GUI_MIN(ySize, yMag); /* 图片高度 */
    GUI_Val2Rect(&r1, x0, y0, xSize, ySize);
    GUI_DrawAreaInit(&r1);
    while (GUI_GetNextArea(&r2)) { /* 遍历所有的显示区域 */
        if (GUI_RectOverlay(&r2, &r2, &r1)) {
            Offset = (u_32)((r2.y0 - y0) * xMag + (r2.x0 - x0)) * 3;
            xSize = r2.x1 - r2.x0 + 1;
            ySize = r2.y1 - r2.y0 + 1;
#if GUI_USE_GRAPHPHY
            LCD_DrawBitmap(GUI_RGB888, pPixel + Offset,
                r2.x0, r2.y0, xSize, ySize, xMag - xSize);
#else
            _DrawBitmap24bp(pPixel + Offset, r2.x0,
                r2.y0, xSize, ySize, (u_16)(xMag - xSize));
#endif
        }
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
    u_32 Offset, PixelBytes = 2;
    GUI_RECT r1, r2;

    xSize = (u_16)GUI_MIN(xSize, xMag); /* 图片宽度 */
    ySize = (u_16)GUI_MIN(ySize, yMag); /* 图片高度 */
    GUI_Val2Rect(&r1, x0, y0, xSize, ySize);
    GUI_DrawAreaInit(&r1);
    while (GUI_GetNextArea(&r2)) { /* 遍历所有的显示区域 */
        if (GUI_RectOverlay(&r2, &r2, &r1)) {
            Offset = (u_32)((r2.y0 - y0) * xMag + (r2.x0 - x0)) * PixelBytes;
            xSize = r2.x1 - r2.x0 + 1;
            ySize = r2.y1 - r2.y0 + 1;
#if GUI_USE_GRAPHPHY
            LCD_DrawBitmap(GUI_RGB565, pPixel + Offset,
                r2.x0, r2.y0, xSize, ySize, xMag - xSize);
#else
            _DrawBitmap24bp(pPixel + Offset, r2.x0,
                r2.y0, xSize, ySize, (u_16)(xMag - xSize));
#endif
        }
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
