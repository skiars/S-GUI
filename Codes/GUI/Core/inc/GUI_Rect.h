#ifndef __GUI_RECT_H
#define __GUI_RECT_H

#include "GUI_Core.h"

GUI_BOOL GUI_Val2Rect(GUI_RECT *pDst,
    i_16 x0, i_16 y0, u_16 xSize, u_16 ySize);
GUI_BOOL GUI_RectOverlay(GUI_RECT *pDst, GUI_RECT *a, GUI_RECT *b);
void GUI_RectSum(GUI_RECT *pDst, GUI_RECT *a, GUI_RECT *b);
GUI_RECT GUI_RectAndCalc(GUI_RECT *pRect1, GUI_RECT *pRect2);
GUI_RECT GUI_RectOrCalc(GUI_RECT *pRect1, GUI_RECT *pRect2);
GUI_BOOL GUI_CheckPointAtRect(i_16 x, i_16 y, GUI_RECT *Rect);
GUI_BOOL GUI_CheckRectNull(GUI_RECT *Rect);
GUI_BOOL GUI_CheckRectIntersect(GUI_RECT *pRect1, GUI_RECT *pRect2);
GUI_BOOL GUI_RectInclude(GUI_RECT *pSrc, GUI_RECT *pDst);
void GUI_MoveRect(GUI_RECT *Rect, i_16 dX, i_16 dY);
GUI_RESULT GUI_RectListInit(u_16 num);
GUI_AREA GUI_GetRectList(u_16 num);
GUI_RESULT GUI_FreeRectList(GUI_AREA Area);
GUI_AREA GUI_RectCut(GUI_RECT *Src, GUI_RECT *Dst);
GUI_AREA GUI_ReCalcRectList(GUI_AREA Area, GUI_RECT *Rect);

#endif /* __GUI_RECT_H */
