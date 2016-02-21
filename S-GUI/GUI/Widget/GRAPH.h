#ifndef __GRAPH_H
#define __GRAPH_H

#include "WIDGET.h"

typedef struct
{
    WIDGET Widget;
    u_16 xDist;
    u_16 yDist;
    i_16 *xData;
    i_16 *yData;
    i_16 Len;
} GRAPH_Obj;

GUI_HWIN GRAPH_Create(i_16 x0,
    i_16 y0,
    u_16 xSize,
    u_16 ySize,
    WM_HWIN hParent,
    u_16 Id,
    u_8 Flag);
GUI_RESULT GRAPH_SetData(GUI_HWIN hWin, i_16 *x, i_16 *y, u_16 len);

#endif
