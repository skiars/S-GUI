#ifndef __GUI_DISPCHAR_H
#define __GUI_DISPCHAR_H

#include "GUI_Typedef.h"
#include "GUI_Font.h"

int GUI_GetCharHeight(void);
int GUI_GetCharWidth(int Char);
int GUI_GetStringWidth(const char *Str);
int GUI_DispChar(int x, int y, int Char);
int GUI_DispCharInRect(GUI_RECT *pRect, int Char);
void GUI_DispString(int x0, int y0, const char *Str);
void GUI_DispStringInRect(GUI_RECT *pRect, const char *Str, u_8 Align);

#endif
