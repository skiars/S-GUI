#ifndef __GUI_BOTTON_H
#define __GUI_BOTTON_H

#include "GUI_Typedef.h"

typedef struct {
    u_16 xSize; /* 显示设备的宽度 */
    u_16 ySize; /* 显示设备的高度 */
    void (*SetPixel)(u_16 x, u_16 y, GUI_COLOR Color); /* 显示某个像素 */
    GUI_COLOR (*GetPixel)(u_16 x, u_16 y);            /* 读取某个像素 */
    /* 显示一个区域到屏幕,pBuffer是一块宽度为rowlen的屏幕缓冲区 */
    void (*DispArea)(u_16 x,
        u_16 y,
        u_16 xSize,
        u_16 ySize,
        GUI_COLOR *pBuffer,
        u_16 rowlen);
} GUI_PHY_INFO;

void GUI_InitOS(void);
void GUI_Lock(void);
void GUI_Unlock(void);
GUI_TIME GUI_GetTime(void);
void _GUI_Delay_ms(GUI_TIME tms);

void *GUI_malloc(u_32 size);
void GUI_free(void *ptr);
void *GUI_fastmalloc(u_32 size);
void GUI_fastfree(void *ptr);

void GUI_Phy_Init(GUI_PHY_INFO *phys);

#endif /* __GUI_BOTTON_H */
