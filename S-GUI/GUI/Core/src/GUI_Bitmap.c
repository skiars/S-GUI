#include "GUI_Bitmap.h"
#include "GUI.h"

static void _DrawBitmap24b(i_16 x,
    i_16 y,
    i_16 x0,
    i_16 y0,
    u_16 x1,
    u_16 y1,
    const unsigned char *pPixel,
    int xMag,
    int yMag)
{
    GUI_COLOR tC;
    const unsigned char *pC, *pt;
    u_16 i = 0, j = 0, a1, a2, b1, b2;
    u_32 l = xMag * 3;

    a1 = y0 - y;
    b1 = x0 - x;
    a2 = y1 + 1;
    b2 = x1 + 1;
    pt = pPixel + a1 * l + b1 * 3;
    for (j = a1; j < a2 && j < yMag; ++j) {
        pC = pt;
        for (i = b1; i < b2 && i < xMag; ++i) {
            tC = *pC++ << 16;
            tC |= *pC++ << 8;
            tC |= *pC++;
            GUI_DrawPixel(i + x, j + y, tC);
        }
        pt += l;
    }
}

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
    GUI_RECT r1, r2;

    GUI_Val2Rect(&r1, x0, y0, xSize, ySize);
    GUI_DrawAreaInit(&r1);
    while (GUI_GetNextArea(&r2)) { /* 遍历所有的显示区域 */
        if (GUI_RectOverlay(&r2, &r2, &r1) == GUI_OK) {
            _DrawBitmap24b(x0, y0, r2.x0, r2.y0, r2.x1, r2.y1,
                pPixel, xMag, yMag);
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
