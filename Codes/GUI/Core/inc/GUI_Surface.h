#ifndef __GUI_SURFACE_H
#define __GUI_SURFACE_H

#include "GUI_Config.h"
#include "GUI_Typedef.h"

typedef struct {
    void *FrameBuffer; /* 帧缓冲 */
    GUI_POINT Pos;     /* 相对偏移 */
    u_16 Width;        /* 宽度 */
    u_16 Height;       /* 高度 */
    int BytesPerPixel; /* 颜色深度 */
    int PixelFormat;   /* 像素格式 */
} GUI_SURFACE;

#endif
