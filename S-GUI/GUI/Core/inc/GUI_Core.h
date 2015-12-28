#ifndef __GUI_CORE_H
#define __GUI_CORE_H

#include "GUI_Config.h"
#include "GUI_Typedef.h"
#include "GUI_Botton.h"

#define _hRootWin  (GUI_Data->RootWin)

/* 矩形 */
typedef struct { i_16 x0, y0, x1, y1; } GUI_RECT;

/* 矩形链表 */
typedef struct RECT_NODE {
    GUI_RECT Rect;
    struct RECT_NODE *pNext;
} RECT_NODE, *RECT_LIST;

typedef void * GUI_EVENT;   /* GUI事件 */

/* 队列结构定义 */
typedef struct {
    GUI_EVENT *pArray;  //队列指针
    u_16 capacity;      //队列数组容量
    u_16 ItemSize;      //队列中数据单元的长度，以字节为单位
    u_16 size;          //队列目前数据单元数量
    u_16 front;         //队头
    u_16 rear;          //队尾
} GUI_QUEUE;

typedef struct{
    GUI_PHY_INFO phy_info; /* 硬件信息 */
    GUI_TIME guitime;      /* GUI运行时间 */
    RECT_LIST RectList;    /* 裁剪矩形私有堆 */
    RECT_LIST NowRectList; /* 当前绘制的裁剪矩链表 */
    GUI_hWin RootWin;     /* 根窗口 */
    GUI_QUEUE *KeyQueue;   /* 按键队列缓冲 */
#if GUI_USE_MEMORY
    GUI_COLOR *lcdbuf; //LCD缓冲
#endif
}GUI_WORK_SPACE;

extern GUI_WORK_SPACE *GUI_Data;

void GUI_Init(void);
void GUI_Unload(void);
void GUI_ScreenSize(u_16 *xSize, u_16 *ySize);
u_16 GUI_GetScreenWidth(void);
u_16 GUI_GetScreenHeight(void);
void GUI_Delay(GUI_TIME tms); //GUI延时并更新
GUI_QUEUE* GUI_EventQueueInit(u_16 capacity, u_16 ItemSize); //事件队列初始化
void GUI_EventQueueDelete(GUI_QUEUE *pQue); //删除队列
u_8 GUI_GetEvent(GUI_QUEUE *pQue, GUI_EVENT event); //从事件队列里读取一个事件
u_8 GUI_SendEvent(GUI_QUEUE *pQue, GUI_EVENT event); //向事件队列发送一个事件
u_8 GUI_QueueIsEmpty(GUI_QUEUE *pQue);//检测事件队列是否为空
void GUI_CleanQueue(GUI_QUEUE *pQue);//清空事件队列

void GUI_SetNowRectList(RECT_LIST l);
void GUI_DrawAreaInit(void);
GUI_RECT *GUI_GetNowArea(void);
u_8 GUI_GetNextArea(GUI_RECT *pRect);

#endif
