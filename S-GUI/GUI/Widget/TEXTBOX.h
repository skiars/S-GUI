#ifndef __TEXTBOX_H
#define __TEXTBOX_H

#include "GUI.h"

typedef struct {
    WIDGET Widget;
    char *Text;
}TEXTBOX_Obj;

WM_hWin TEXTBOX_Create(i_16 x0,
                      i_16 y0,
                      u_16 xSize,
                      u_16 ySize,
                      WM_hWin hParent,
                      u_16 Id,
                      u_8 Flag);
u_8 TEXTBOX_SetText(WM_hWin hWin, const char *str);
u_8 TEXTBOX_SetFont(WM_hWin hWin, GUI_FontType Font);
void TEXTBOX_SetAllAlpha(WM_hWin hWin, u_8 Alpha);


#endif
