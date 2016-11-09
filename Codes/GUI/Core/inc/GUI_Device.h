#ifndef __GUI_DEVICE_H
#define __GUI_DEVICE_H

#include "GUI_Config.h"
#include "GUI_Typedef.h"
#include "GUI_Bitmap.h"

#define GUI_DEF_SCREEN   0
#define GUI_MEN_SEREEN   0xFF

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
typedef struct GUI_GDEV {
    u_16 Width, Height; /* 显示设备的宽度和高度高度 */
    unsigned char Id;   /* 设备ID */
    int PixelFormat;    /* 像素颜色格式 */
    void (*SetPixel)(u_16, u_16, GUI_COLOR); /* 显示某个像素 */
    GUI_COLOR (*GetPixel)(u_16, u_16);       /* 读取某个像素 */
    void (*DrawHLine)(u_16, u_16, u_16, GUI_COLOR);
    void (*DrawVLine)(u_16, u_16, u_16, GUI_COLOR);
    void (*FillRect)(GUI_FLIPOUT *);
    void(*DrawBitmap)(GUI_FLIPOUT *);
    struct GUI_GDEV *pNext;
} GUI_GDEV;

/* 绘图API定义 */
typedef struct {
    void (*SetPixelClip)(i_16, i_16);
    void (*DrawHLine)(i_16, i_16, i_16);
    void (*FillRect)(i_16, i_16, i_16, i_16);
} GUI_GLAPI;

extern GUI_GDEV   *GUI_GDev;
extern GUI_GLAPI   GUI_glAPI;

void _GL_SetPixel(i_16 x, i_16 y, GUI_COLOR);
GUI_COLOR _GL_GetPixel(i_16 x, i_16 y);
void _GL_DrawVLine(i_16 x0, i_16 y0, i_16 y1);
void _GL_DrawBitmap(u_8 PixelFormat, const unsigned char *pPixel,
    i_16 x0, i_16 y0, u_16 xSize, u_16 ySize, int Offset,
    const LCD_LOGPALETTE *pLog);
void GUI_DeviceInit(void);
GUI_RESULT GUI_SelectDevice(unsigned char id);
GUI_GDEV * GUI_GetDevice(unsigned char id);
void GUI_DeleteDevice(unsigned char id);
void GUI_DeleteDeviceList(void);

#endif /* __GUI_DEVICE_H */
