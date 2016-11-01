#ifndef __GUI_MATH_H
#define __GUI_MATH_H

#include "GUI_Config.h"
#include "GUI_Typedef.h"

#define GUI_45DEG  512
#define GUI_90DEG  (2*GUI_45DEG)
#define GUI_180DEG (4*GUI_45DEG)
#define GUI_360DEG (8*GUI_45DEG)

int GUI_sin(int angle);
int GUI_cos(int angle);

#endif
