#ifndef __GUI_TOUCHPAD_H
#define __GUI_TOUCHPAD_H

#include "GUI_Queue.h"

#define GUI_TP_CHECKED   1
#define GUI_TP_REMOVED   0

void GUI_TouchPadSendValue(int x, int y, int State);
GUI_RESULT GUI_TouchPadMessageProc(GUI_MESSAGE *pMsg);

#endif /* __GUI_TOUCHPAD_H */
