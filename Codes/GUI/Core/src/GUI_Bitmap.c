#include "GUI_Bitmap.h"
#include "GUI.h"

#define CLIP_X(X0, X1) \
{ \
    X0 = X0 > GUI_CurrentClipRect()->x0 ? X0 : GUI_CurrentClipRect()->x0; \
    X1 = X1 < GUI_CurrentClipRect()->x1 ? X1 : GUI_CurrentClipRect()->x1; \
}

#define CLIP_Y(Y0, Y1) \
{ \
    Y0 = Y0 > GUI_CurrentClipRect()->y0 ? Y0 : GUI_CurrentClipRect()->y0; \
    Y1 = Y1 < GUI_CurrentClipRect()->y1 ? Y1 : GUI_CurrentClipRect()->y1; \
}

/* 绘制位图，不支持透明效果 */
void GUI_DrawBitmap24b(int x0,
    int y0,
    int xSize,
    int ySize,
    const unsigned char *pPixel,
    const LCD_LOGPALETTE *pLogPal,
    int xMag,
    int yMag)
{
    int x1, y1, x = x0, y = y0;
    u_32 Offset;
    GUI_RECT r1;

    xSize = GUI_MIN((int)xSize, (int)xMag); /* 图片宽度 */
    ySize = GUI_MIN((int)ySize, (int)yMag); /* 图片高度 */
    GUI_Val2Rect(&r1, x0, y0, xSize, ySize);
    GUI_PaintAreaInit(&r1);
    while (GUI_NextPaintArea()) { /* 遍历所有的显示区域 */
        x0 = r1.x0;
        y0 = r1.y0;
        x1 = r1.x1;
        y1 = r1.y1;
        CLIP_X(x0, x1);
        CLIP_Y(y0, y1);
        Offset = (u_32)((y0 - y) * xMag + (x0 - x)) * 3;
        GL_DrawBitmap(GUI_RGB888, pPixel + Offset,
            x0, y0, x1 - x0 + 1, y1 - y0 + 1, xMag - (x1 - x0 + 1),
            NULL);
    }
}

/* 绘制位图，不支持透明效果 */
void GUI_DrawBitmap16b(int x0,
    int y0,
    int xSize,
    int ySize,
    const unsigned char *pPixel,
    const LCD_LOGPALETTE *pLogPal,
    int xMag,
    int yMag)
{
    int x = x0, y = y0, x1, y1;
    u_32 Offset, PixelBytes = 2;
    GUI_RECT r1;

    xSize = GUI_MIN((int)xSize, (int)xMag); /* 图片宽度 */
    ySize = GUI_MIN((int)ySize, (int)yMag); /* 图片高度 */
    GUI_Val2Rect(&r1, x0, y0, xSize, ySize);
    GUI_PaintAreaInit(&r1);
    while (GUI_NextPaintArea()) { /* 遍历所有的显示区域 */
        x0 = r1.x0;
        y0 = r1.y0;
        x1 = r1.x1;
        y1 = r1.y1;
        CLIP_X(x0, x1);
        CLIP_Y(y0, y1);
        Offset = (u_32)((y0 - y) * xMag + (x0 - x)) * PixelBytes;
        xSize = x1 - x0 + 1;
        ySize = y1 - y0 + 1;
        GL_DrawBitmap(GUI_RGB565, pPixel + Offset,
            x0, y0, xSize, ySize, xMag - xSize,
            NULL);
    }
}

void GUI_DrawGif(int x0,
    int y0,
    int xSize,
    int ySize,
    const unsigned char *pPixel,
    const LCD_LOGPALETTE *pLogPal,
    int xMag,
    int yMag)
{
    int x = x0, y = y0, x1, y1;
    u_32 Offset, PixelBytes = 1;
    GUI_RECT r1;

    xSize = GUI_MIN((int)xSize, (int)xMag); /* 图片宽度 */
    ySize = GUI_MIN((int)ySize, (int)yMag); /* 图片高度 */
    GUI_Val2Rect(&r1, x0, y0, xSize, ySize);
    GUI_PaintAreaInit(&r1);
    while (GUI_NextPaintArea()) { /* 遍历所有的显示区域 */
        x0 = r1.x0;
        y0 = r1.y0;
        x1 = r1.x1;
        y1 = r1.y1;
        CLIP_X(x0, x1);
        CLIP_Y(y0, y1);
        Offset = (u_32)((y0 - y) * xMag + (x0 - x)) * PixelBytes;
        xSize = x1 - x0 + 1;
        ySize = y1 - y0 + 1;
        GL_DrawBitmap(GUI_LOG, pPixel + Offset,
            x0, y0, xSize, ySize, xMag - xSize, pLogPal);
    }
}

void GUI_DrawBitmap(int x0,
    int y0,
    int xSize,
    int ySize,
    const GUI_BITMAP *Bmp)
{
    if (Bmp->pDraw) {
        GUI_ClientToScreen(&x0, &y0);
        Bmp->pDraw(x0, y0, xSize, ySize,
            Bmp->pData, Bmp->pPal, Bmp->xSize, Bmp->ySize);
    }
}
