#ifndef __GUI_RECTCALC_H
#define __GUI_RECTCALC_H

#include "GUI_Core.h"

GUI_RESULT GUI_Val2Rect(GUI_RECT *pDst,
                        i_16 x0, i_16 y0, u_16 xSize, u_16 ySize);
GUI_RESULT GUI_RectOverlay(GUI_RECT *pDst, GUI_RECT *a, GUI_RECT *b);
GUI_RECT GUI_RectAndCalc(GUI_RECT *pRect1, GUI_RECT *pRect2);
GUI_RECT GUI_RectOrCalc(GUI_RECT *pRect1, GUI_RECT *pRect2);
GUI_RESULT GUI_CheckPointAtRect(u_16 x, u_16 y, GUI_RECT *Rect);
u_8 GUI_CheckRectNull(GUI_RECT *Rect);
u_8 GUI_CheckRectIntersect(GUI_RECT *pRect1, GUI_RECT *pRect2);
u_8 GUI_RectInclude(GUI_RECT *pSrc, GUI_RECT *pDst);
void GUI_MoveRect(GUI_RECT *Rect, i_16 dX, i_16 dY);
u_8 GUI_RectListInit(u_16 num);
RECT_LIST GUI_GetRectList(u_16 num);
GUI_RESULT GUI_FreeRectList(RECT_NODE *pNode);
RECT_LIST GUI_RectCut(GUI_RECT *Src, GUI_RECT *Dst);
RECT_LIST GUI_ReCalcRectList(RECT_LIST List, GUI_RECT *Rect);

#endif /* __GUI_RECTCALC_H */
