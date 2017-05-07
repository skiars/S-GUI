#ifndef __GUI_PAINTAREA_H
#define __GUI_PAINTAREA_H

#include "GUI_Typedef.h"
#include "GUI_Config.h"
#include "GUI_Context.h"

GUI_BOOL GUI_PaintStart(GUI_HWIN hWin, GUI_CONTEXT *Backup);
void GUI_PaintEnd(GUI_CONTEXT *Backup);
void GUI_PaintAreaInit(GUI_RECT *p);
GUI_BOOL GUI_NextPaintArea(void);

#endif
