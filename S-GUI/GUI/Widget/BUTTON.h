#ifndef __BUTTON_H
#define __BUTTON_H

#include "GUI.h"

typedef struct {
    WIDGET Widget;
    u_16 Check;
    char *Title;
} BUTTON_Obj;

WM_hWin BUTTON_Create(i_16 x0, i_16 y0, u_16 xSize, u_16 ySize, WM_hWin hParent, u_16 Id, u_8 Flag);
u_8 BUTTON_SetTitle(WM_hWin hWin, const char *str);
u_8 BUTTON_SetFont(WM_hWin hWin, GUI_FontType Font);
void BUTTON_Check(WM_hWin hWin, u_8 NewStatus);

void BUTTON_Test(void);

#endif
