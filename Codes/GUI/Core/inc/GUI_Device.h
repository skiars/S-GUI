#ifndef __GUI_DEVICE_H
#define __GUI_DEVICE_H

#include "GUI_Config.h"
#include "GUI_Typedef.h"

#define GL_SetPixel  GUI_LCD.SetPixel
#define GL_GetPixel  GUI_LCD.GetPixel
#define GL_DrawHLine GUI_LCD.DrawHLine
#define GL_FillRect  GUI_LCD.FillRect

typedef struct {
    u_16 xSize; /* 显示设备的宽度 */
    u_16 ySize; /* 显示设备的高度 */
    void (*SetPixel)(u_16 x, u_16 y, GUI_COLOR Color); /* 显示某个像素 */
    GUI_COLOR (*GetPixel)(u_16 x, u_16 y);             /* 读取某个像素 */
    void (*DrawHLine)(i_16 x0, i_16 y0, u_16 len, GUI_COLOR Color);
    void (*FillRect)(i_16 x0, i_16 y0, i_16 x1, i_16 y1, GUI_COLOR Color);
} GUI_DEVICE;

extern GUI_DEVICE GUI_LCD;

void GUI_DeviceInit(void);

#endif /* __GUI_DEVICE_H */
