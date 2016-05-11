#ifndef __GUI_BOTTON_H
#define __GUI_BOTTON_H

#include "GUI_Device.h"

void GUI_InitOS(void);
void GUI_TaskLock(void);
void GUI_TaskUnlock(void);
u_32 GUI_GetTaskId(void);
GUI_TIME GUI_GetTime(void);
void _GUI_Delay_ms(GUI_TIME tms);
void * _GUI_GetHeapBuffer(int Page, u_32 *Size);
void GUI_Phy_Init(GUI_DEVICE *phy);
void _GUI_DebugOut(const char *s);

#if GUI_USE_GRAPHPHY
void LCD_FillRect(i_16 x0, i_16 y0, i_16 x1, i_16 y1, GUI_COLOR Color);
void LCD_DrawBitmap(u_32 ColorFormat, const unsigned char *pPixel,
    i_16 x0, i_16 y0, u_16 xSize, u_16 ySize, u_16 Offset);
#endif

#endif /* __GUI_BOTTON_H */
