#ifndef __GUI_CLIPAREAHEAP_H
#define __GUI_CLIPAREAHEAP_H

#include "GUI_Typedef.h"

/* ¾ØÐÎÁ´±í */
typedef struct AREA_NODE {
    GUI_RECT Rect;
    struct AREA_NODE *pNext;
} AREA_NODE, *GUI_AREA;

GUI_RESULT GUI_RectListInit(void);
void GUI_FreeIdleRectList(void);
GUI_AREA GUI_GetRectList(int num);
GUI_RESULT GUI_FreeRectList(GUI_AREA Area);
void GUI_FreeRectListNode(GUI_AREA Area);
GUI_AREA GUI_RectCut(GUI_RECT *Src, GUI_RECT *Dst);

#endif // !__GUI_CLIPAREAHEAP_H
