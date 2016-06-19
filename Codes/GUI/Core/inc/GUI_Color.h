#ifndef __GUI_COLOR_H
#define __GUI_COLOR_H

#include "GUI_Config.h"
#include "GUI_Typedef.h"

u_16 GUI_RGB888To565(u_32 rgb);
u_32 GUI_RGB565To888(u_16 rgb);
GUI_COLOR GUI_AlphaBlend(GUI_COLOR Color, GUI_COLOR BkColor, u_16 Alpha);

#endif
