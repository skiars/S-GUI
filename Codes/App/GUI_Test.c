#include "GUI_Test.h"
#include "GUI.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <math.h>

#include "Bitmap.h"
#include "game.h"

#define WINDOW1      (WM_USER_ID + 0x0010)
#define WINDOW2      (WM_USER_ID + 0x0020)
#define WINDOW3      (WM_USER_ID + 0x0030)
#define WINDOW4      (WM_USER_ID + 0x0040)

#define WIN1_BTN1    (WINDOW1 + 0x0001)
#define WIN1_BTN2    (WINDOW1 + 0x0002)

#define WIN2_BTN1    (WINDOW2 + 0x0001)
#define WIN2_BTN2    (WINDOW2 + 0x0002)
#define WIN2_BTN3    (WINDOW3 + 0x0002)

void Create_Window1(void);
static void Create_GraphicTest(void);

static char _Str[30];
static u_16 _FpsVal; /* 帧率 */
static u_8 _CPUUsage; /* CPU占用率 */

static void _RootWinPaint(WM_HWIN hWin)
{
    /* 绘制背景 */
    GUI_SetFGColor(0x00C0C0C0);
    GUI_FillRect(0, 0, GUI_GetScreenWidth(), GUI_GetScreenHeight());
    //GUI_DrawBitmap(0, 0, 480, 320, &bmpic_rootwin);
    GUI_DispString(10, 300, _Str);
}

static void _RootWinTimer(WM_HWIN hWin)
{
    //GUI_RECT Rect = { 10, 300, 150, 320 };

#ifdef _MSC_VER
    sprintf_s(_Str, sizeof(_Str), "FPS: %d, CPU: %d%%", _FpsVal, _CPUUsage);
#else
    sprintf(_Str, "FPS: %d, CPU: %d%%", _FpsVal, _CPUUsage);
#endif // _WIN32
    
    //WM_InvalidateRect(_hRootWin, &Rect);
    WM_Invalidate(_hRootWin);
    _FpsVal = 0; /* 帧率清零 */
}

/* GUI测试 */
void GUI_Test(void)
{
    GUI_Init();
    /* 设置根窗口回调函数 */
    RootWinPaint_Cb = _RootWinPaint;
    RootWinTimer_Cb = _RootWinTimer;
    GUI_SetRootWindowTimer(1000);
    Create_Window1();
    Create_GraphicTest();
    GUI_SetFont(&GUI_FontUI17_4pp);
    while (1) {
        GUI_Delay(20);
        ++_FpsVal; /* 统计帧率 */
    }
}

void Window2_Cb(WM_MESSAGE *pMsg)
{
    static u_8 Alpha;
    WM_HWIN hWin, hItem;

    hWin = WM_GetClientWindow(pMsg->hWin);
    switch (pMsg->MsgId) {
    case WM_CREATED:
        hItem = BUTTON_Create(12, 20, 70, 20, hWin, WIN2_BTN2, 0);
        BUTTON_SetTitle(hItem, "Alpha+");
        hItem = BUTTON_Create(108, 20, 70, 20, hWin, WIN2_BTN3, 0);
        BUTTON_SetTitle(hItem, "Alpha-");
        hItem = BUTTON_Create(70, 70, 70, 20, hWin, WIN2_BTN1, 0);
        BUTTON_SetTitle(hItem, "Exit");
        break;
    case WM_BUTTON_RELEASED:
        if (WM_GetDialogId(pMsg->hWinSrc) == WIN2_BTN1) {
            WM_DeleteWindow(pMsg->hWin);
            Alpha = 0;
        } else if (WM_GetDialogId(pMsg->hWinSrc) == WIN2_BTN2) {
            hWin = pMsg->hWin;
            WINDOW_SetAllAlpha(hWin, Alpha);
            if (Alpha < 200) Alpha += 10;
        } else if (WM_GetDialogId(pMsg->hWinSrc) == WIN2_BTN3) {
            hWin = pMsg->hWin;
            WINDOW_SetAllAlpha(hWin, Alpha);
            if (Alpha >= 10) Alpha -= 10;
        }
        break;
    case WM_KEYDOWN:
        switch (pMsg->Param) {
        case KEY_ESC:
            WM_DeleteWindow(pMsg->hWin);
            break;
        }
        break;
    }
}

void Create_Window2(void)
{
    WM_HWIN hWin;

    hWin = WINDOW_Create(7, 10, 200, 140, NULL, WINDOW2, WM_WS_MOVE, Window2_Cb);
    WINDOW_SetTitle(hWin, "Hello World!");
}

void Create_Window3(void)
{
    WINDOW_Create(0, 0, GUI_GetScreenWidth(),
        GUI_GetScreenHeight(), NULL, WINDOW3, WM_WS_MOVE, GameWin_Cb);
}

void Window1_Cb(WM_MESSAGE *pMsg)
{
    WM_HWIN hItem, hWin = pMsg->hWin, hClient;

    switch (pMsg->MsgId) {
    case WM_CREATED:
        hClient = WM_GetClientWindow(hWin);
        hItem = BUTTON_Create(5, 5, 100, 20, hClient, WIN1_BTN1, 0);
        BUTTON_SetTitle(hItem, "Alpha Test");
        hItem = BUTTON_Create(5, 30, 100, 20, hClient, WIN1_BTN2, 0);
        BUTTON_SetTitle(hItem, "Cube Field");
        BUTTON_SetFont(hItem, &GUI_FontUI17_4pp);
        break;
    case WM_BUTTON_RELEASED:
        /* 根据点击的按键创建窗口 */
        switch (WM_GetDialogId(pMsg->hWinSrc)) {
        case WIN1_BTN1:
            Create_Window2();
            break;
        case WIN1_BTN2:
            Create_Window3();
            break;
        }
        break;
    }
}

void Create_Window1(void)
{
    GUI_HWIN hWin;

    hWin = WINDOW_Create(20, 20, 120, 80, NULL, WINDOW1, WM_WS_MOVE, Window1_Cb);
    WINDOW_SetTitle(hWin, "S-GUI Demo");
    WINDOW_SetFont(hWin, &GUI_FontUI17_4pp);
}

void CLockInit(GUI_RECT *r, int s)
{
    i_16 t;
    int i;
    float angle;

    /* 旋转刻度 */
    for (i = 0; i < 60; ++i) {
        angle = 3.1415926f / 30 * i;
        if (!(i % 5)) {
            t = 84;
        } else {
            t = 90;
        }
        r->x0 = (i_16)(sinf(angle) * t * s);
        r->y0 = (i_16)(cosf(angle) * t * s);
        r->x1 = (i_16)(sinf(angle) * 96 * s);
        r->y1 = (i_16)(cosf(angle) * 96 * s);
        r->x0 = 105 * s + r->x0;
        r->y0 = 105 * s + r->y0;
        r->x1 = 105 * s + r->x1;
        r->y1 = 105 * s + r->y1;
        ++r;
    }
}

/* 钟面 */
void ClockDisp(int time, GUI_RECT *r, int s)
{
    int i;
    i_16 x0, x1, y0, y1;
    float angle;
    GUI_POINT Points[3];
    static const GUI_POINT points[3] = {
        { 105, 105 - 96 },
        { 105 - 2, 105 },
        { 105 + 2, 105 }
    };

    GUI_SetFGColor(0x00803050);
    GUI_SetAAFactor(s);
    /* 旋转刻度 */
    for (i = 0; i < 60; ++i) {
        if (!(i % 5)) {
            GUI_SetPenSize(3 * s);
        } else {
            GUI_SetPenSize(2 * s);
        }
        GUI_DrawLineAA(r->x0, r->y0, r->x1, r->y1);
        ++r;
    }
    /* 旋转指针 */
    angle = 3.1415926f / 30 * time;
    for (i = 0; i < 3; ++i) {
        Points[i].x = (int)(((points[i].x - 105) * s) * cosf(angle)
            - ((points[i].y - 105) * s) * sinf(angle) + 105 * s);
        Points[i].y = (int)(((points[i].x - 105) * s) * sinf(angle)
            + ((points[i].y - 105) * s) * cosf(angle) + 105 * s);
    }
    GUI_SetFGColor(0x00808050);
    GUI_FillPolygonAA(Points, 3);
    GUI_SetFGColor(0x00303030);
    GUI_FillCircleAA(105 * s, 105 * s, 5 * s);
}

void Graphic_Cb(WM_MESSAGE *pMsg)
{
    int i;
    static GUI_RECT Rect[60];
    static int time;

    switch (pMsg->MsgId) {
    case WM_PAINT:
        if (GUI_GetPaintWindow() != pMsg->hWin) {
            ClockDisp(time, Rect, 15);
        }
        break;
    case WM_CREATED:
        CLockInit(Rect, 15);
        break;
    case WM_TIMER:
        WM_Invalidate(WM_GetClientWindow(pMsg->hWin));
        if (time < 59) {
            time++;
        } else {
            time = 0;
        }
        break;
    }
}

static void Create_GraphicTest(void)
{
    GUI_HWIN hWin;

    hWin = WINDOW_Create(190, 50, 240, 240,
        NULL, WINDOW4, WM_WS_MOVE, Graphic_Cb);
    WINDOW_SetTitle(hWin, "Graphics Library Test");
    GUI_TimerCreate(hWin, 0, 1000, GUI_TMR_AUTO);

    /* 置顶窗口实验 */
    hWin = WINDOW_Create(40, 140, 120, 80,
        NULL, 0x1000, WM_WS_MOVE | WM_WS_STICK, NULL);
    WINDOW_SetTitle(hWin, "Top window 1");
    hWin = WINDOW_Create(60, 160, 120, 80,
        NULL, 0x1001, WM_WS_MOVE | WM_WS_STICK, NULL);
    WINDOW_SetTitle(hWin, "Top window 2");
    hWin = WINDOW_Create(70, 170, 180, 180,
        NULL, 0x102, WM_WS_MOVE | WM_WS_BACKGND, NULL);
    WINDOW_SetTitle(hWin, "Bottom window");
}
