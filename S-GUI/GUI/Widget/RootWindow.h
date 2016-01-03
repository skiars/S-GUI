#ifndef __ROOTWINDOW_H
#define __ROOTWINDOW_H

#include "GUI.h"

/* 用户自定义的根窗口绘制函数 */
extern void (*RootWinPaint_Cb)(WM_hWin hWin);
/* 用户自定义的根窗口定时器处理函数 */
extern void (*RootWinTimer_Cb)(WM_hWin hWin);

void WM_RootWindowInit(WM_Obj *pObj);

#endif
