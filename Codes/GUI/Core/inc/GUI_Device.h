#ifndef __GUI_DRIVER_H
#define __GUI_DRIVER_H

#include "GUI_Config.h"
#include "GUI_Typedef.h"
#include "GUI_Bitmap.h"

#define GL_SetPixel       _GL_SetPixel
#define GL_GetPixel       _GL_GetPixel
#define GL_DrawBitmap     _GL_DrawBitmap
#define GL_SetPixelClip   GUI_glAPI.SetPixelClip
#define GL_DrawHLine      GUI_glAPI.DrawHLine
#define GL_DrawVLine      _GL_DrawVLine
#define GL_FillRect	      GUI_glAPI.FillRect

/* 绘图指令 */
typedef struct {
    i_16 x0, y0, x1, y1;
    u_16 xSize, ySize;
    u_32 Offset;
    GUI_COLOR Color;
    const LCD_LOGPALETTE *pLog;
    const unsigned char *pSrc;
    unsigned char *pDst;
    u_8 SrcFormat, DstFormat;
}GUI_FLIPOUT;

/* 图形设备驱动函数 */
typedef struct {
    u_16 xSize; /* 显示设备的宽度 */
    u_16 ySize; /* 显示设备的高度 */
    void (*SetPixel)(u_16, u_16, GUI_COLOR); /* 显示某个像素 */
    GUI_COLOR (*GetPixel)(u_16, u_16);       /* 读取某个像素 */
    void (*DrawHLine)(u_16, u_16, u_16, GUI_COLOR);
    void (*DrawVLine)(u_16, u_16, u_16, GUI_COLOR);
    void (*FillRect)(GUI_FLIPOUT *);
    void (*DrawBitmap)(GUI_FLIPOUT *);
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

/* 绘图API定义 */
typedef struct {
    void (*SetPixelClip)(i_16, i_16);
    void (*DrawHLine)(i_16, i_16, i_16);
    void (*FillRect)(i_16, i_16, i_16, i_16);
} GUI_GLAPI;

extern GUI_DRIVER GUI_GDev;
extern GUI_GLAPI   GUI_glAPI;

void _GL_SetPixel(i_16 x, i_16 y, GUI_COLOR);
GUI_COLOR _GL_GetPixel(i_16 x, i_16 y);
void _GL_DrawVLine(i_16 x0, i_16 y0, i_16 y1);
void _GL_DrawBitmap(u_8 PixelFormat, const unsigned char *pPixel,
    i_16 x0, i_16 y0, u_16 xSize, u_16 ySize, int Offset,
    const LCD_LOGPALETTE *pLog);
void GUI_DeviceInit(void);

#endif /* __GUI_DRIVER_H */
