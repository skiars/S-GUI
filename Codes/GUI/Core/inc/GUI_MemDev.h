#ifndef __GUI_MEMDEV_H
#define __GUI_MEMDEV_H

#include "GUI_Config.h"
#include "GUI_Typedef.h"

typedef struct GUI_MEMDEV {
    int x0, y0;
    int Width, Height;
    GUI_HWIN hWin;
    u_8 PixelFormat;
    u_8 BytesPerPixel;
    u_8 Alpha;
    unsigned char *Pixels;
    struct GUI_MEMDEV *pNext;
} GUI_MEMDEV;

#endif
