#ifndef __GUI_DRIVER_H
#define __GUI_DRIVER_H

#include "GUI_Config.h"
#include "GUI_Typedef.h"

#define GL_SetPixel   GUI_GDev.SetPixel
#define GL_GetPixel   GUI_GDev.GetPixel
#define GL_DrawBitmap GUI_GDev.DrawBitmap

/* 图形设备驱动函数 */
typedef struct {
    u_16 xSize; /* 显示设备的宽度 */
    u_16 ySize; /* 显示设备的高度 */
    void (*SetPixel)(u_16 x, u_16 y, GUI_COLOR Color); /* 显示某个像素 */
    GUI_COLOR (*GetPixel)(u_16 x, u_16 y);             /* 读取某个像素 */
    void (*DrawHLine)(i_16 x0, i_16 y0, i_16 x1, GUI_COLOR Color);
    void(*DrawVLine)(i_16 x0, i_16 y0, i_16 x1, GUI_COLOR Color);
    void (*FillRect)(i_16 x0, i_16 y0, i_16 x1, i_16 y1, GUI_COLOR Color);
    void (*DrawBitmap)(u_8 cFormat, const unsigned char *pData,
        i_16 x0, i_16 y0, u_16 xSize, u_16 ySize, u_32 Offset);
} GUI_DRIVER;

/* 图形设备抽象结构 */
typedef struct {
    void *FrameBuffer; /* 帧缓冲 */
    void *Position;    /* 当前工作的位置 */
    i_16 x0;           /* 在物理设备中的x0 */
    i_16 y0;           /* 在物理设备中的y0 */
    u_16 Width;
    u_16 Height;
    int BytesPerPixel; /* 颜色深度 */
    int PixelFormat;   /* 像素颜色格式 */
    GUI_DRIVER Phy;    /* 驱动程序 */
} GUI_GDEV;

extern GUI_DRIVER GUI_GDev;

void GUI_DeviceInit(void);
void GL_DrawHLine(i_16 x0, i_16 y0, i_16 x1);
void GL_DrawVLine(i_16 x0, i_16 y0, i_16 y1);
void GL_FillRect(i_16 x0, i_16 y0, i_16 x1, i_16 y1);

#endif /* __GUI_DRIVER_H */
