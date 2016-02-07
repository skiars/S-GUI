#ifndef __GUI_TOUCHPAD_H
#define __GUI_TOUCHPAD_H

#include "GUI_Queue.h"

#define GUI_TP_CHECKED   1
#define GUI_TP_REMOVED   0

void GUI_TouchPadSendValue(i_16 x, i_16 y, u_16 State);
GUI_RESULT GUI_TouchPadMessageHandle(GUI_MESSAGE *pMsg);

#endif /* __GUI_TOUCHPAD_H */
