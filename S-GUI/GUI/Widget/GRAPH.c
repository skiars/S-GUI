#include "GRAPH.h"
#include "GUI.h"

#define DFT_BACKCOLOR        0x00000000
#define DFT_FONTCOLOR        0x00ffffff
#define DFT_EDGECOLOR        0x00ffffff
#define DFT_LINECOLOR        0x00ff0000
#define DFT_FONT             Font_ASCII_8X16
#define DFT_X_DISTANCE       25
#define DFT_Y_DISTANCE       50

static void _DrawData(GRAPH_Obj *pObj, GUI_RECT *pr);
static void _DeleteData(GRAPH_Obj *pWin);

static void __Paint(GRAPH_Obj *pObj)
{
    i_16 x0, y0, x1, y1;
    u_16 xSize, ySize;
    GUI_COLOR Color;
    GUI_RECT Rect = WM_GetWindowRect(pObj);
    
    x0 = Rect.x0;
    y0 = Rect.y0;
    x1 = Rect.x1;
    y1 = Rect.y1;
    xSize = x1 - x0 + 1;
    ySize = y1 - y0 + 1;
    /* 绘制背景 */
    Color = pObj->Widget.Skin.BackColor[0];
    GUI_FillRect(x0, y0, xSize, ySize, Color);
    Rect = WM_GetWindowRect(pObj);
    x0 = Rect.x0;
    y0 = Rect.y0;
    x1 = Rect.x1;
    y1 = Rect.y1;
    xSize = x1 - x0 + 1;
    ySize = y1 - y0 + 1;
    /* 绘制边框 */
    Color = pObj->Widget.Skin.EdgeColor[0];
    GUI_DrawRect(x0, y0, xSize, ySize, Color);
    /* 绘制网格 */

    GUI_VertLine(x1, y0, ySize, Color);
    GUI_HoriLine(x0, y1, xSize, Color);
    _DrawData(pObj, &Rect); /* 绘制曲线 */
}

/* GRAPH控件自动回调函数 */
static void __Callback(WM_MESSAGE *pMsg)
{
    GRAPH_Obj *pObj = pMsg->hWin;

    /* 检测是否为GRPHA控件 */
    WIDGET_SignErrorReturnVoid(pMsg->hWin, WIDGET_GRAPH);
    switch (pMsg->MsgId) {
    case WM_PAINT:
        __Paint(pMsg->hWin);
        break;
    case WM_DELETE:
        _DeleteData(pMsg->hWin);
        break;
    default:
        WM_DefaultProc(pMsg);
    }
}

/*
* 创建GRAPH
* x0:GRAPH控件的最左像素(相对于父窗口)
* y0:GRAPH控件的最右像素(相对于父窗口)
* xSize:GRAPH控件的水平宽度
* ySize:GRAPH控件的竖直高度
* hParent:父窗口句柄
* Id:窗口ID
* Flag:窗口状态
**/
GUI_HWIN GRAPH_Create(i_16 x0,
    i_16 y0,
    u_16 xSize,
    u_16 ySize,
    WM_HWIN hParent,
    u_16 Id,
    u_8 Flag)
{
    GRAPH_Obj *pObj;

    pObj = WM_CreateWindowAsChild(x0, y0, xSize, ySize, hParent, Flag,
        WIDGET_GRAPH, Id, __Callback,
        sizeof(GRAPH_Obj) - sizeof(WM_Obj));
    if (pObj == NULL) {
        return NULL;
    }
    /* 设置颜色 */
    pObj->Widget.Skin.BackColor[0] = DFT_BACKCOLOR;
    pObj->Widget.Skin.FontColor[0] = DFT_FONTCOLOR;
    pObj->Widget.Skin.EdgeColor[0] = DFT_EDGECOLOR;
    pObj->Widget.Skin.EdgeColor[1] = DFT_LINECOLOR;
    pObj->Widget.Skin.Font = DFT_FONT;
    GRAPH_SetScale(pObj, 0, 100, 0, 100, 50, 50);
    pObj->xDist = DFT_X_DISTANCE;
    pObj->yDist = DFT_Y_DISTANCE;
    pObj->Len = 0;
    pObj->List = List_Init();
    return pObj;
}

/* 设置比例尺 */
GUI_RESULT GRAPH_SetScale(GUI_HWIN hWin,
    i_16 x0,
    i_16 y0,
    u_16 xScale,
    u_16 yScale,
    u_16 xDist,
    u_16 yDist)
{
    GRAPH_Obj *pObj = hWin;

    /* 检测是否为GRAPH控件 */
    WIDGET_SignErrorReturn(hWin, WIDGET_GRAPH);
    pObj->Scale.x0 = x0;
    pObj->Scale.y0 = y0;
    pObj->Scale.xScale = xScale;
    pObj->Scale.yScale = yScale;
    pObj->Scale.xDist = xDist;
    pObj->Scale.yDist = yDist;
    return GUI_OK;
}

/* 绘制点 */
static void __DrawXY(GRAPH_Obj *pObj, GRAPH_DATA *pData, GUI_RECT *pr)
{
    u_16 i;
    int x0, y0, x1, y1;
    int xPix = pr->x1 - pr->x0 + 1, yPix = pr->y1 - pr->y0 + 1;
    int xScale, yScale;
    GUI_COLOR Color = pData->Color;
    GRAPH_XYDATA *Data = pData->pData;
    GRAPH_SCALE *pScale = &pObj->Scale;

    xScale = pScale->xScale;
    yScale = pScale->yScale;
    for (i = 1; i < Data->ItemNum; ++i) {
        /* 转换为绝对坐标 */
        x0 = pr->x0 + (Data->xData[i - 1] - pScale->x0) * xScale / 100;
        y0 = pr->y1 - (Data->yData[i - 1] - pScale->y0) * yScale / 100;
        x1 = pr->x0 + (Data->xData[i] - pScale->x0) * xScale / 100;
        y1 = pr->y1 - (Data->yData[i] - pScale->y0) * yScale / 100;
        /* 绘制点 */
        x0 = GUI_MAX(pr->x0 + 1, x0);
        y0 = GUI_MAX(pr->y0 + 1, y0);
        x1 = GUI_MIN(pr->x1 - 1, x1);
        y1 = GUI_MIN(pr->y1 - 1, y1);
        GUI_DrawLine(x0, y0, x1, y1, Color);
    }
}

static void _DrawData(GRAPH_Obj *pObj, GUI_RECT *pr)
{
    LIST List;

    if (pObj->List == NULL) {
        return;
    }
    for (List = pObj->List->pNext; List; List = List->pNext) {
        switch (((GRAPH_DATA *)List->pData)->Status) {
        case GRAPH_XY_DATA:
            __DrawXY(pObj, List->pData, pr);
            break;
        }
    }
}

/* 删除XY格式的数据 */
static void _DeleteXYData(GRAPH_DATA *p)
{
    GRAPH_XYDATA *pd = p->pData;

    GUI_fastfree(pd->xData);
    GUI_fastfree(pd->yData);
    GUI_fastfree(pd);
}

/* 删除数据 */
static void _DeleteData(GRAPH_Obj *pWin)
{
    LIST l, i;

    if (pWin->List == NULL) {
        return;
    }
    l = pWin->List;
    while (l) {
        i = l->pNext;
        if (l->pData) {
            switch (((GRAPH_DATA *)l->pData)->Status) {
            case GRAPH_XY_DATA:
                _DeleteXYData(l->pData);
                break;
            }
            GUI_fastfree(l->pData);
        }
        l = i;
    }
}

/* 建立一个XY格式的图像数据 */
GUI_HWIN GRAPH_XY_DataCreate(int *xData,
    int *yData,
    int ItemNum,
    int MaxItemNum,
    GUI_COLOR Color,
    u_8 Style)
{
    int i, *iData, *jData;
    GRAPH_DATA *pObj;
    GRAPH_XYDATA *pData;

    pObj = GUI_fastmalloc(sizeof(GRAPH_DATA));
    pData = GUI_fastmalloc(sizeof(GRAPH_XYDATA));
    iData = GUI_fastmalloc(MaxItemNum * sizeof(int));
    jData = GUI_fastmalloc(MaxItemNum * sizeof(int));
    if (!pObj || !pData || !iData || !jData ) {
        GUI_fastfree(pObj);
        GUI_fastfree(pData);
        GUI_fastfree(iData);
        GUI_fastfree(jData);
        return NULL;
    }
    pObj->Status = GRAPH_XY_DATA;
    pObj->pData = pData;
    pObj->Color = Color;
    pObj->Style = Style;
    pObj->Title = NULL;
    pData->xData = iData;
    pData->yData = jData;
    if (ItemNum > MaxItemNum) {
        ItemNum = MaxItemNum;
    }
    pData->ItemNum = ItemNum;
    pData->MaxItemNum = MaxItemNum;
    for (i = 0; i < ItemNum; ++i) {
        iData[i] = xData[i];
        jData[i] = yData[i];
    }
    return pObj;
}

/* 获取一个图像的句柄 */
GUI_HWIN GRAPH_GethData(GUI_HWIN hWin, int Num)
{
    LIST List;
    GRAPH_Obj *pWin = hWin;

    /* 检测是否为GRAPH控件 */
    if (WM_CheckWindowSign(hWin, WIDGET_GRAPH) == GUI_ERR) {
        return NULL;
    }
    if (Num && pWin->List) {
        List = pWin->List;
        while (Num-- && List) {
            List = List->pNext;
        }
        if (List) {
            return List->pData;
        }
    }
    return NULL;
}

/* 设置一个图像的数据 */
void GRAPH_XY_DataEdit(GUI_HWIN hData, int *pX, int *pY, int Num)
{
    int i;
    GRAPH_DATA *pObj = hData;
    GRAPH_XYDATA *pData;

    if (pObj == NULL || pObj->pData == NULL) {
        return;
    }
    pData = pObj->pData;
    if (Num < pData->MaxItemNum) {
        pData->ItemNum = Num;
    } else {
        pData->ItemNum = pData->MaxItemNum;
    }
    for (i = 0; i < pData->ItemNum; ++i) {
        pData->xData[i] = pX[i];
        pData->yData[i] = pY[i];
    }
}

/* 添加一组图像 */
void GRAPH_AttachData(GUI_HWIN hGraph, GUI_HWIN hData)
{
    GRAPH_Obj *pGraph = hGraph;

    List_InsertEnd(pGraph->List, hData, 0); /* 链表末尾插入节点 */
}
