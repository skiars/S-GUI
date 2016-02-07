#ifndef __BUTTON_H
#define __BUTTON_H

#include "GUI.h"

typedef struct {
    WIDGET Widget;
    u_16 Check;
    char *Title;
} BUTTON_Obj;

WM_hWin BUTTON_Create(i_16 x0, i_16 y0, u_16 xSize, u_16 ySize, WM_hWin hParent, u_16 Id, u_8 Flag);
GUI_RESULT BUTTON_SetTitle(WM_hWin hWin, const char *str);
GUI_RESULT BUTTON_SetFont(WM_hWin hWin, GUI_FontType Font);
GUI_RESULT BUTTON_Check(WM_hWin hWin, u_8 NewStatus);

#endif
