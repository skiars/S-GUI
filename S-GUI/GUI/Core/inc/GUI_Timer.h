#ifndef __GUI_TIMER_H
#define __GUI_TIMER_H

#include "GUI_Config.h"
#include "GUI_Typedef.h"

typedef struct GUI_TIMER {
    GUI_hWin hWin;
    GUI_TIME LastTime;  /* 上次更新时间 */
    GUI_TIME Interval;  /* 时间间隔 */
    struct GUI_TIMER *pNext;
} GUI_TIMER;

void GUI_TimerHandle(void);
void GUI_SetWindowTimer(GUI_hWin hWin, GUI_TIME Interval);
GUI_TIME GUI_GetWindowTimer(GUI_hWin hWin);

#endif /* __GUI_TIMER_H */
