#ifndef __GUI_DEVICE_H
#define __GUI_DEVICE_H

#include "GUI_Config.h"
#include "GUI_Typedef.h"
#include "GUI_Bitmap.h"

#define GUI_DEF_SCREEN   0
#define GUI_MEN_SEREEN   0xFF

/* 绘图指令 */
typedef struct {
    int x0, y0, x1, y1;
    int xSize, ySize;
    u_32 Offset;
    GUI_COLOR Color;
    const LCD_LOGPALETTE *pLog;
    const unsigned char *pSrc;
    unsigned char *pDst;
    u_8 SrcFormat, DstFormat;
}GUI_FLIPOUT;

/* 图形设备驱动函数 */
typedef struct GUI_GDEV {
    int width, height; /* 显示设备的宽度和高度高度 */
    unsigned char id;   /* 设备ID */
    int pixelFormat;    /* 像素颜色格式 */
    void (*setPixel)(int, int, GUI_COLOR); /* 显示某个像素 */
    GUI_COLOR (*getPixel)(int, int);       /* 读取某个像素 */
    void (*drawHLine)(int, int, int, GUI_COLOR);
    void (*drawVLine)(int, int, int, GUI_COLOR);
    void (*fillRect)(GUI_FLIPOUT *);
    void(*drawBitmap)(GUI_FLIPOUT *);
    struct GUI_GDEV *pNext;
} GUI_GDEV;

extern GUI_GDEV      *gui_graph_dev;

/* write screen pixel */
static inline void HL_SetPixel(int x, int y, GUI_COLOR Color)
{
    gui_graph_dev->setPixel(x, y, Color);
}

/* read screen pixel */
static inline GUI_COLOR HL_GetPixel(int x, int y)
{
    return gui_graph_dev->getPixel(x, y);
}

/* draw screen hline */
static inline void HL_DrawHLine(int x0, int y, int x1, GUI_COLOR Color)
{
    gui_graph_dev->drawHLine(x0, y, x1, Color);
}

/* draw screen vline */
static inline void HL_DrawVLine(int x, int y0, int y1, GUI_COLOR Color)
{
    gui_graph_dev->drawVLine(x, y0, y1, Color);
}

/* 获取屏幕尺寸 */
static inline void GUI_ScreenSize(int *width, int *height)
{
    *width = gui_graph_dev->width;
    *height = gui_graph_dev->height;
}

/* 获取屏幕宽度 */
static inline int GUI_GetScreenWidth(void)
{
    return gui_graph_dev->width;
}

/* 获取屏幕高度 */
static inline int GUI_GetScreenHeight(void)
{
    return gui_graph_dev->height;
}

void GL_DrawBitmap(u_8 PixelFormat, const unsigned char *pPixel,
    int x0, int y0, int xSize, int ySize, int Offset,
    const LCD_LOGPALETTE *pLog);
void GUI_DeviceInit(void);
GUI_RESULT GUI_SelectDevice(unsigned char id);
GUI_GDEV * GUI_GetDevice(unsigned char id);
void GUI_DeleteDevice(unsigned char id);
void GUI_DeleteDeviceList(void);

#endif /* __GUI_DEVICE_H */
