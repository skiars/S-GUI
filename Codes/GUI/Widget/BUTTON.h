#ifndef __BUTTON_H
#define __BUTTON_H

#include "GUI.h"

typedef struct {
    WIDGET Widget;
    u_8 Check;
    char *Title;
} BUTTON_Obj;

WM_HWIN BUTTON_Create(i_16 x0, i_16 y0, u_16 xSize,
    u_16 ySize, WM_HWIN hParent, u_16 Id, u_8 Style);
GUI_RESULT BUTTON_SetTitle(WM_HWIN hWin, const char *str);
GUI_RESULT BUTTON_SetFont(WM_HWIN hWin, GUI_FONT *Font);
GUI_RESULT BUTTON_Check(WM_HWIN hWin, u_8 NewStatus);
u_8 BUTTON_GetStatus(WM_HWIN hWin);

#endif
