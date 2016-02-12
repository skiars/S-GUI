#ifndef __GUI_H
#define __GUI_H

#include "GUI_Config.h"
#include "GUI_Typedef.h"
#include "GUI_Core.h"
#include "GUI_2D.h"
#include "GUI_Font.h"
#include "Text.h"
#include "GUI_WM.h"
#include "GUI_RectCalc.h"
#include "DialogBox.h"
#include "GUI_Botton.h"
#include "GUI_Memory.h"
#include "WIDGET.h"
#include "GUI_KeyBoard.h"
#include "GUI_TouchPad.h"

#include "GUI_Bitmap.h"

#include "WIDGET.h"
#include "BUTTON.h"
#include "DialogBox.h"
#include "RootWindow.h"
#include "TEXTBOX.h"
#include "WINDOW.h"
#include "LISTBOX.h"
#include "SCROLLBAR.h"
#include "GRAPH.h"

#define GUI_COUNTOF(a)          (sizeof(a) / sizeof(a[0]))

#ifndef NULL
  #ifdef __cplusplus
    #define NULL 0
  #else
    #define NULL ((void *)0)
  #endif
#endif

#endif /* __GUI_H */
