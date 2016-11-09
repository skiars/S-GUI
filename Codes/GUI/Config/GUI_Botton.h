#ifndef __GUI_BOTTON_H
#define __GUI_BOTTON_H

#include "GUI_Device.h"

void * GUI_TaskCreateLock(void);
void GUI_TaskLock(void *pLock);
void GUI_TaskUnlock(void *pLock);
u_32 GUI_TaskGetId(void);
GUI_TIME GUI_GetTime(void);
void _GUI_Delay_ms(GUI_TIME tms);
void * _GUI_GetHeapBuffer(int Page, u_32 *Size);
void GUI_UserConfig(GUI_GDEV *phy);
void _GUI_DebugOut(const char *s);

#endif /* __GUI_BOTTON_H */
