#ifndef __GRAPH_H
#define __GRAPH_H

#include "WIDGET.h"
#include "linked_list.h"

#define GRAPH_XY_DATA  0
#define GRAPH_TY_DATA  1

typedef struct {
    void *pData;
    u_8 Status;
    char *Title;
    GUI_COLOR Color;
    u_8 Style;
} GRAPH_DATA;

typedef struct {
    int *xData;           /* x轴数据 */
    int *yData;           /* y轴数据 */
    int ItemNum;          /* 数据数量 */
    int MaxItemNum;       /* 最大的数据数量 */
} GRAPH_XYDATA;

typedef struct {
    int *yData;           /* x轴数据 */
    int tStart;           /* 开始时间值 */
    int ItemNum;          /* 数据数量 */
    int MaxItemNum;       /* 最大的数据数量 */
} GRAPH_TYDATA;

typedef struct {
    int x0, y0;            /* 起始坐标 */
    u_16 xScale, yScale;   /* x轴和y轴缩放比例 */
    u_16 xDist, yDist;     /* x轴和y轴网格线距离 */
} GRAPH_SCALE;

typedef struct {
    WIDGET Widget;
    GRAPH_SCALE Scale;
    LIST List;
} GRAPH_Obj;

GUI_HWIN GRAPH_Create(i_16 x0,
    i_16 y0,
    u_16 xSize,
    u_16 ySize,
    WM_HWIN hParent,
    u_16 Id,
    u_8 Flag);
GUI_RESULT GRAPH_SetScale(GUI_HWIN hWin, i_16 x0,
    i_16 y0, u_16 xScale, u_16 yScale, u_16 xDist, u_16 yDist);
GUI_HWIN GRAPH_XY_DataCreate(int *xData, int *yData,
    int ItemNum, int MaxItemNum, GUI_COLOR Color, u_8 Style);
GUI_HWIN GRAPH_GethData(GUI_HWIN hWin, int Num);
void GRAPH_XY_DataEdit(GUI_HWIN hData, int *pX, int *pY, int Num);
GUI_HWIN GRAPH_TY_DataCreate(int *yData, int tStart, int ItemNum,
    int MaxItemNum, GUI_COLOR Color, u_8 Style);
void GRAPH_TY_DataEdit(GUI_HWIN hData, int *pY, int tStart, int Num);
void GRAPH_AttachData(GUI_HWIN hGraph, GUI_HWIN hData);

#endif
