#ifndef __GUI_CORE_H
#define __GUI_CORE_H

#include "GUI_Config.h"
#include "GUI_Typedef.h"
#include "GUI_Botton.h"
#include "GUI_Queue.h"

#define _hRootWin   (GUI_Data->RootWin)
#define _PaintArea  (GUI_Data->PaintArea)

/* 矩形 */
typedef struct { i_16 x0, y0, x1, y1; } GUI_RECT;
/* 点 */
typedef struct { i_16 x, y; } GUI_POINT;

/* 矩形链表 */
typedef struct RECT_NODE {
    GUI_RECT Rect;
    struct RECT_NODE *pNext;
} RECT_NODE, *RECT_LIST;

typedef struct{
    GUI_PHY_INFO phy_info; /* 硬件信息 */
    GUI_TIME guitime;      /* GUI运行时间 */
    RECT_LIST RectList;    /* 裁剪矩形私有堆 */
    RECT_LIST NowRectList; /* 当前绘制的裁剪矩链表 */
    GUI_RECT *PaintArea;   /* 当前绘制的区域 */
    GUI_hWin RootWin;      /* 根窗口 */
    GUI_QUEUE *MsgQueue;   /* GUI消息队列 */
#if GUI_USE_MEMORY
    GUI_COLOR *lcdbuf; /* LCD缓冲 */
#endif
}GUI_WORK_SPACE;

extern GUI_WORK_SPACE *GUI_Data;

GUI_RESULT GUI_Init(void);
void GUI_Unload(void);
void GUI_ScreenSize(u_16 *xSize, u_16 *ySize);
u_16 GUI_GetScreenWidth(void);
u_16 GUI_GetScreenHeight(void);
void GUI_Delay(GUI_TIME tms);
GUI_RESULT GUI_GetMessage(GUI_MESSAGE *pMsg);
GUI_RESULT GUI_PostMessage(GUI_MESSAGE *pMsg);
void GUI_LOCK(void);
void GUI_UNLOCK(void);

void GUI_SetNowRectList(RECT_LIST l, GUI_RECT *p);
void GUI_DrawAreaInit(GUI_RECT *p);
GUI_RECT *GUI_GetNowArea(void);
u_8 GUI_GetNextArea(GUI_RECT *pRect);

#endif /* __GUI_CORE_H */
