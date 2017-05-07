#ifndef __GUI_ROOTWIN_H
#define __GUI_ROOTWIN_H

#include "GUI_Typedef.h"

/* 用户自定义的根窗口绘制函数 */
extern void (*RootWinPaint_Cb)(GUI_HWIN hWin);
/* 用户自定义的根窗口定时器处理函数 */
extern void (*RootWinTimer_Cb)(GUI_HWIN hWin);

void WM_RootWindowInit(GUI_HWIN hWin);
void GUI_SetRootWindowTimer(GUI_TIME timer);

#endif
