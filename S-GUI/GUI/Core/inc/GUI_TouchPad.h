#ifndef __GUI_TOUCHPAD_H
#define __GUI_TOUCHPAD_H

#include "GUI.h"

#define GUI_TP_CHECKED   1
#define GUI_TP_REMOVED   0

void GUI_TouchPadSendValue(u_16 x, u_16 y, u_16 State);
i_16 GUI_GetTouchPad_X(void);
i_16 GUI_GetTouchPad_Y(void);
i_16 GUI_GetTouchPad_MoveX(void);
i_16 GUI_GetTouchPad_MoveY(void);

#endif
