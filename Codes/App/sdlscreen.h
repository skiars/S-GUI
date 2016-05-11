#ifndef __SDLSCREEN_H
#define __SDLSCREEN_H

#define HAL_SCREEN_W 480
#define HAL_SCREEN_H 320

#define HAL_RGB888   0
#define HAL_RGB565   1

typedef unsigned long UINT32;

void sdl_init(void);
void HAL_SetPixel(int x, int y, UINT32 Color);
UINT32 HAL_GetPixel(int x, int y);
void HAL_FillRect(int x0, int y0, int x1, int y1, UINT32 Color);
void HAL_DrawBitmap(int ColorFormat, const unsigned char *pPixel,
    int x0, int y0, int xSize, int ySize, int Offset);

#endif // !__SDL_SCREEN_H