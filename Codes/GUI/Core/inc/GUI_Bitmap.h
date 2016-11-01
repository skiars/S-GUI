#ifndef __GUI_BITMAP_H
#define __GUI_BITMAP_H

#include "GUI_Typedef.h"

#define GUI_DRAW_BMP24    &GUI_DrawBitmap24b
#define GUI_DRAW_BMPM565  &GUI_DrawBitmap16b
#define GUI_DRAW_GIF      &GUI_DrawGif

/* 调色板 */
typedef struct {
    u_32             NumEntries;
    char             HasTrans;
    const LCD_COLOR *pPalEntries;
} LCD_LOGPALETTE;

typedef LCD_LOGPALETTE GUI_LOGPALETTE;

/* 位图结构体 */
typedef struct {
    u_16 xSize, ySize;
    u_32 LineBytes;
    u_8 PixelBits;
    const unsigned char *pData;
    const LCD_LOGPALETTE *pPal;
    void(*pDraw)(i_16 x0,
        i_16 y0,
        u_16 xSize,
        u_16 ySize,
        const unsigned char *pPixel,
        const LCD_LOGPALETTE *pLogPal,
        int xMag,
        int yMag);
} GUI_BITMAP;

void GUI_DrawBitmap24b(i_16 x0, i_16 y0, u_16 xSize, u_16 ySize,
    const unsigned char *pPixel, const LCD_LOGPALETTE *pLogPal,
    int xMag, int yMag);
void GUI_DrawBitmap16b(i_16 x0, i_16 y0, u_16 xSize, u_16 ySize,
    const unsigned char *pPixel, const LCD_LOGPALETTE *pLogPal,
    int xMag, int yMag);
void GUI_DrawGif(i_16 x0, i_16 y0, u_16 xSize, u_16 ySize,
    const unsigned char *pPixel, const LCD_LOGPALETTE *pLogPal,
    int xMag, int yMag);
void GUI_DrawBitmap(i_16 x0, i_16 y0, u_16 xSize, u_16 ySize,
    const GUI_BITMAP *Bmp);

#endif /* __GUI_BITMAP_H */
