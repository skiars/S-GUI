#ifndef __GUI_CLIPAREA_H
#define __GUI_CLIPAREA_H

#include "GUI_Config.h"
#include "GUI_Typedef.h"
#include "GUI_Core.h"

GUI_AREA GUI_ClipExcludeRect(GUI_AREA Area, GUI_RECT *Rect);
void GUI_ClipNewWindow(GUI_HWIN hWin);
void GUI_WindowsClipArea(GUI_HWIN hWin, GUI_HWIN hEnd);
void GUI_DeleteWindowClipArea(GUI_HWIN hWin);
void GUI_ClipWindows(GUI_HWIN hWin);
GUI_AREA GUI_GetWindowClipArea(GUI_HWIN hWin);

#endif
