#ifndef __GUI_DISPCHAR_H
#define __GUI_DISPCHAR_H

#include "GUI_Typedef.h"
#include "GUI_Font.h"

u_16 GUI_GetCharHeight(void);
u_16 GUI_GetCharWidth(u_16 Char);
int GUI_GetStringWidth(const char *Str);
u_16 GUI_DispChar(i_16 x, i_16 y, u_16 Char);
u_16 GUI_DispCharInRect(GUI_RECT *pRect, u_16 Char);
void GUI_DispString(i_16 x0, i_16 y0, const char *Str);
void GUI_DispStringInRect(GUI_RECT *pRect, const char *Str, u_8 Align);

#endif
