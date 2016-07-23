#ifndef __GUI_TIMER_H
#define __GUI_TIMER_H

#include "GUI_Config.h"
#include "GUI_Typedef.h"

#define GUI_TMR_ONE      0 /* 单次触发 */
#define GUI_TMR_AUTO     1 /* 自动重载 */

typedef GUI_HWIN GUI_HTMR;

typedef struct GUI_TIMER {
    GUI_HWIN hWin;
    int      Id;
    u_8      Mode;
    GUI_TIME Period;    /* 时间间隔 */
    GUI_TIME SetTime;   /* 设定时间 */
    struct GUI_TIMER *pNext;
} GUI_TIMER;

void GUI_TimerHandler(void);
GUI_HTMR GUI_TimerCreate(GUI_HWIN hWin, int Id, int Period, u_8 Mode);
void GUI_TimerDetete(GUI_HTMR hTimer);
void GUI_TimerDeleteAtWindow(GUI_HWIN hWin);
u_16 GUI_GetTimerId(GUI_HTMR hTimer);
void GUI_ResetTimer(GUI_HTMR hTimer, int Period);

#endif /* __GUI_TIMER_H */
