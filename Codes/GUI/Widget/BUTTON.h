#ifndef __BUTTON_H
#define __BUTTON_H

#include "GUI.h"

typedef struct {
    WIDGET Widget;
    u_8 Check;
    char *Title;
} BUTTON_Obj;

WM_HWIN BUTTON_Create(int x0, int y0, int xSize,
    int ySize, WM_HWIN hParent, int Id, u_8 Style);
void BUTTON_SetTitle(WM_HWIN hWin, const char *str);
void BUTTON_SetFont(WM_HWIN hWin, GUI_FONT *Font);
void BUTTON_Check(WM_HWIN hWin, u_8 NewStatus);
u_8 BUTTON_GetStatus(WM_HWIN hWin);

#endif
