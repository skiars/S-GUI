#ifndef __TEXTBOX_H
#define __TEXTBOX_H

#include "GUI.h"

typedef struct {
    WIDGET Widget;
    char *Text;
}TEXTBOX_Obj;

WM_HWIN TEXTBOX_Create(i_16 x0,
                      i_16 y0,
                      u_16 xSize,
                      u_16 ySize,
                      WM_HWIN hParent,
                      u_16 Id,
                      u_8 Style);
GUI_RESULT TEXTBOX_SetText(WM_HWIN hWin, const char *str);
GUI_RESULT TEXTBOX_SetFont(WM_HWIN hWin, GUI_FONT Font);
GUI_RESULT TEXTBOX_SetAllAlpha(WM_HWIN hWin, u_8 Alpha);


#endif
