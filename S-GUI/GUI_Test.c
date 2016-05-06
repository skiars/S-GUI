#include "GUI_Test.h"
#include "GUI.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <math.h>

#include "Bitmap.h"

#define WINDOW2      (WM_USER_ID + 0x0010)
#define WINDOW1      (WM_USER_ID + 0x0020)
#define WINDOW3      (WM_USER_ID + 0x0030)
#define WINDOW4      (WM_USER_ID + 0x0040)
#define WINDOW5      (WM_USER_ID + 0x0050)
#define WINDOW6      (WM_USER_ID + 0x0060)
#define WINDOW7      (WM_USER_ID + 0x0070)

#define WIN2_BTN1    (WINDOW2 + 0x0001)
#define WIN2_BTN2    (WINDOW2 + 0x0002)
#define WIN2_BTN3    (WINDOW2 + 0x0003)
#define WIN2_BTN4    (WINDOW2 + 0x0004)
#define WIN2_BTN5    (WINDOW2 + 0x0005)

#define WIN1_BTN1    (WINDOW1 + 0x0001)
#define WIN1_BTN2    (WINDOW1 + 0x0002)
#define WIN1_BTN3    (WINDOW1 + 0x0003)

#define WIN3_TBX1    (WINDOW3 + 0x0001)
#define WIN3_BTN1    (WINDOW3 + 0x0002)
#define WIN3_BTN2    (WINDOW3 + 0x0003)

#define WIN4_LBX1    (WINDOW4 + 0x0001)
#define WIN4_BTN1    (WINDOW4 + 0x0002)

#define WIN5_GPH1    (WINDOW5 + 0x0001)
#define WIN5_BTN1    (WINDOW5 + 0x0002)
#define WIN5_BTN2    (WINDOW5 + 0x0003)

#define WIN6_TBX1    (WINDOW6 + 0x0001)
#define WIN6_BTN1    (WINDOW6 + 0x0002)

void GUI_2D_Test(void);
void Create_Window2(void);

static char _Str[15] = "";
static u_16 _FpsVal; /* 帧率 */

void GUI_2DTest(void);

static void _RootWinPaint(WM_HWIN hWin)
{
    static i_16 y1 = 300;
    /* 绘制背景 */
    //GUI_FillRect(0, 0, 480, 320, 0x00ffffff);
    GUI_DrawBitmap(0, 0, 640, 320, &bmpic_rootwin);
    GUI_DispStringCurRect(10, 300, _Str, 0x00000000, Font_ASCII_8X16);
    //GUI_DrawLine(100, 100, 200, 5, 0);
    GUI_2DTest();
}

static void _RootWinTimer(WM_HWIN hWin)
{
    GUI_RECT Rect = { 10, 300, 100, 320 };

#ifdef _WIN32
    sprintf_s(_Str, sizeof(_Str), "FPS: %d", _FpsVal);
#else
    sprintf(_Str "FPS: %d", _FpsVal);
#endif // _WIN32
    
    WM_InvalidateRect(_hRootWin, &Rect);
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
    Create_Window2();
    while (1) {
        GUI_Delay(20);
        ++_FpsVal; /* 统计帧率 */
    }
}

void Window1_Cb(WM_MESSAGE *pMsg)
{
    static u_8 Alpha;
    WM_HWIN hWin, hItem;

    hWin = WM_GetClientWindow(pMsg->hWin);
    switch (pMsg->MsgId) {
    case WM_CREATED:
        hItem = BUTTON_Create(12, 20, 70, 20, hWin, WIN1_BTN2, 0);
        BUTTON_SetTitle(hItem, "Alpha+");
        hItem = BUTTON_Create(108, 20, 70, 20, hWin, WIN1_BTN3, 0);
        BUTTON_SetTitle(hItem, "Alpha-");
        hItem = BUTTON_Create(70, 70, 70, 20, hWin, WIN1_BTN1, 0);
        BUTTON_SetTitle(hItem, "Exit");
        break;
    case WM_BUTTON_RELEASED:
        if (WM_GetDialogId(pMsg->hWinSrc) == WIN1_BTN1) {
            WM_DeleteWindow(pMsg->hWin);
            Alpha = 0;
        } else if (WM_GetDialogId(pMsg->hWinSrc) == WIN1_BTN2) {
            hWin = pMsg->hWin;
            WINDOW_SetAllAlpha(hWin, Alpha);
            if (Alpha < 200) Alpha += 10;
        } else if (WM_GetDialogId(pMsg->hWinSrc) == WIN1_BTN3) {
            hWin = pMsg->hWin;
            WINDOW_SetAllAlpha(hWin, Alpha);
            if (Alpha >= 10) Alpha -= 10;
        }
        break;
    }
}

void Create_Window1(void)
{
    WM_HWIN hWin;

    hWin = WINDOW_Create(7, 10, 200, 140, NULL, WINDOW1, WM_WS_MOVE, Window1_Cb);
    WINDOW_SetTitle(hWin, "Hello World!");
}

void Window3_Cb(WM_MESSAGE *pMsg)
{
    u_16 Id;
    WM_HWIN hWin = pMsg->hWin, hItem;

    
    switch (pMsg->MsgId) {
    case WM_CREATED:
        hWin = WM_GetClientWindow(pMsg->hWin);
        hItem = TEXTBOX_Create(0, 0, 214, 210, hWin, WIN3_TBX1, 0);
        TEXTBOX_SetText(hItem, "This is a Small Graphical User Interface.\n"
            "It\'s author is Guan Wenliang.\n"
            "This is a demonstration...");
        hItem = BUTTON_Create(30, 250, 60, 20, hWin, WIN3_BTN2, 0);
        BUTTON_SetTitle(hItem, "Alpha");
        hItem = BUTTON_Create(100, 250, 60, 20, hWin, WIN3_BTN1, 0);
        BUTTON_SetTitle(hItem, "Exit");
        break;
    case WM_BUTTON_RELEASED:
        Id = WM_GetDialogId(pMsg->hWinSrc);
        if (Id == WIN3_BTN1) {
            WM_DeleteWindow(hWin);
        } else if (Id == WIN3_BTN2) {
            WINDOW_SetAllAlpha(hWin, 150);
            hItem = WM_GetDialogItem(hWin, WIN3_TBX1);
            TEXTBOX_SetAllAlpha(hItem, 200);
        }
        break;
    }
}

void Create_Window3(void)
{
    GUI_HWIN hWin;

    hWin = WINDOW_Create(10, 10, 220, 300, NULL, WINDOW3, WM_WS_MOVE, Window3_Cb);
    WINDOW_SetTitle(hWin, "Infomation");
}

void Window4_Cb(WM_MESSAGE *pMsg)
{
    int i;
    char s[25], *ps;
    WM_HWIN hWin, hItem;

    switch (pMsg->MsgId) {
    case WM_CREATED:
        hWin = WM_GetClientWindow(pMsg->hWin);
        hItem = LISTBOX_Create(0, 0, 214, 210, hWin, WIN4_LBX1, 0);
        for (i = 0; i < 2000; ++i) {
#ifdef _WIN32
            sprintf_s(s, sizeof(s), "Item%d", i + 1);
#else
            sprintf(s, "Item%d", i + 1);
#endif // _WIN32
            LISTBOX_AddList(hItem, s);
        }
        LISTBOX_AddList(hItem, "Check to return...");
        hItem = BUTTON_Create(70, 250, 60, 20, hWin, WIN4_BTN1, 0);
        BUTTON_SetTitle(hItem, "Exit");
        break;
    case WM_BUTTON_RELEASED:
        if (WM_GetDialogId(pMsg->hWinSrc) == WIN4_BTN1) {
            WM_DeleteWindow(pMsg->hWin);
        }
        break;
    case WM_LISTBOX_CHECK:
        ps = LISTBOX_GetSelText(pMsg->hWinSrc);
        if (!strcmp(ps, "Check to return...")) {
            WM_DeleteWindow(pMsg->hWin);
        }
        break;
    }
}

void Create_Window4(void)
{
    GUI_HWIN hWin;

    hWin = WINDOW_Create(10, 10, 220, 300, NULL, WINDOW4, WM_WS_MOVE, Window4_Cb);
    WINDOW_SetTitle(hWin, "LISTBOX Test");
}

void Window5_Cb(WM_MESSAGE *pMsg)
{
    static u_8 stop = 0;
    static int i, x[300], y1[300], y2[300];
    GUI_TIME tm;
    static GUI_TIME t;
    GUI_HWIN hWin, hItem, hData;

    switch (pMsg->MsgId) {
    case WM_CREATED:
        t = 0;
        WINDOW_SetTitle(pMsg->hWin, "GRPHA Test");
        GUI_SetWindowTimer(pMsg->hWin, 100);
        hWin = WM_GetClientWindow(pMsg->hWin);
        hItem = GRAPH_Create(0, 0, 214, 210, hWin, WIN5_GPH1, 0);
        GRAPH_SetScale(hItem, 0, -100, 100, 100, 50, 50);
        for (i = 0; i < 300; ++i) {
            x[i] = i;
            y1[i] = (int)(sin(i / 16.0) * 100.0);
            y2[i] = (int)(sin(i / 14.0) * 50.0);
        }
        hData = GRAPH_XY_DataCreate(x, y1, 300, 300, 0x0000ff00, 0);
        GRAPH_AttachData(hItem, hData);
        hData = GRAPH_TY_DataCreate(y2, 0, 300, 300, 0x00ff0000, 0);
        GRAPH_AttachData(hItem, hData);
        hItem = BUTTON_Create(30, 250, 60, 20, hWin, WIN5_BTN1, 0);
        BUTTON_SetTitle(hItem, "Stop");
        hItem = BUTTON_Create(100, 250, 60, 20, hWin, WIN5_BTN2, 0);
        BUTTON_SetTitle(hItem, "Exit");
        break;
    case WM_BUTTON_RELEASED:
        switch (WM_GetDialogId(pMsg->hWinSrc)) {
        case WIN5_BTN1:
            hItem = WM_GetDialogItem(pMsg->hWin, WIN5_BTN1);
            if (stop == 0) {
                stop = 1;
                BUTTON_SetTitle(hItem, "Run");
                GUI_SetWindowTimer(pMsg->hWin, 0);
            } else {
                stop = 0;
                BUTTON_SetTitle(hItem, "Stop");
                GUI_SetWindowTimer(pMsg->hWin, 100);
            }
            break;
        case WIN5_BTN2:
            WM_DeleteWindow(pMsg->hWin);
            break;
        }
        break;
    case WM_TIME_UPDATA:
        t +=  10;
        tm = t % 800;
        if (tm > 400) {
            tm = 800 - tm;
        }
        tm /= 10;
        for (i = 0; i < 300; ++i) {
            x[i] = i;
            y1[i] = (int)(sin((t + i) / 16.0) * 100.0);
            y2[i] = (int)(sin((t / 3 + i) / 14.0) * tm);
        }
        t++;
        hItem = WM_GetDialogItem(pMsg->hWin, WIN5_GPH1);
        hData = GRAPH_GethData(hItem, 1);
        GRAPH_XY_DataEdit(hData, x, y1, 300);
        hData = GRAPH_GethData(hItem, 2);
        GRAPH_TY_DataEdit(hData, y2, 20, 300);
        WM_Invalidate(hItem);
    }
}

void Create_Window5(void)
{
    WINDOW_Create(10, 10, 220, 300, NULL, WINDOW5, WM_WS_MOVE, Window5_Cb);
}

void Window6_Cb(WM_MESSAGE *pMsg)
{
    char Str[50];
    WM_HWIN hWin, hItem;

    switch (pMsg->MsgId) {
    case WM_CREATED:
        WINDOW_SetTitle(pMsg->hWin, "Benchmark Result");
        hWin = WM_GetClientWindow(pMsg->hWin);
        TEXTBOX_Create(0, 0, 140, 60, hWin, WIN6_TBX1, 0);
        hItem = BUTTON_Create(42, 65, 60, 20, hWin, WIN6_BTN1, 0);
        BUTTON_SetTitle(hItem, "OK!");
        break;
    case WM_BUTTON_RELEASED:
        if (WM_GetDialogId(pMsg->hWinSrc) == WIN6_BTN1) {
            WM_DeleteWindow(pMsg->hWin);
        }
        break;
    case WM_USER_MESSAGE:
#ifdef _WIN32
        sprintf_s(Str, sizeof(Str), "%d0 Pixels/s\nFPS : %d",
            (int)pMsg->Param, (int)(pMsg->Param / 320 / 24)); /* 放大10倍 */
#else
        sprintf(Str, "%d0 Pixels/s\n%d FPS",
            (int)pMsg->Param, (int)pMsg->Param / 320 / 24); /* 放大10倍 */
#endif // _WIN32
        hItem = WM_GetDialogItem(pMsg->hWin, WIN6_TBX1);
        TEXTBOX_SetText(hItem, Str);
        break;
    }
}

void Window7_Cb(WM_MESSAGE *pMsg)
{
    i_16 x0, y0;
    u_16 xSize, ySize;
    static unsigned long long PixelSum;
    static GUI_TIME dt, t0;
    GUI_COLOR Color;
    WM_HWIN hWin = pMsg->hWin;

    switch (pMsg->MsgId) {
    case WM_PAINT:
        PixelSum = 0;
        t0 = GUI_GetTime();
        do {
            Color = (rand() * rand()) & 0x00ffffff;
            x0 = rand() % 240;
            y0 = rand() % 320;
            xSize = rand() % (240 - x0);
            ySize = rand() % (320 - y0);
            GUI_FillRect(x0, y0, xSize, ySize, Color);
            GUI_DrawDevice(x0, y0, x0 + xSize + 1, y0 + ySize + 1);
            PixelSum += (u_32)xSize * (u_32)ySize;
            dt = GUI_GetTime() - t0;
        } while (dt < 5000);
        WM_PostMessage(hWin, WM_USER_MESSAGE, 0);
        break;
    case WM_USER_MESSAGE:
        WM_DeleteWindow(hWin);
        hWin = WINDOW_Create(10, 10, 150, 120,
            NULL, WINDOW7, WM_WS_MOVE, Window6_Cb);
        WM_SendMessage(hWin, WM_USER_MESSAGE,
            (GUI_PARAM)(PixelSum * 1000 / dt / 10));
        break;
    }
}

void Create_Window6(void)
{
    WM_CreateWindowAsChild(0, 0, 240, 320,
        NULL, 0, WINDOW6, Window7_Cb, 0);
    //WM_SetForegroundWindow(_pRootWin->hFirstChild);
}

void Window2_Cb(WM_MESSAGE *pMsg)
{
    WM_HWIN hItem, hWin = pMsg->hWin, hClient;

    switch (pMsg->MsgId) {
    case WM_CREATED:
        hClient = WM_GetClientWindow(hWin);
        hItem = BUTTON_Create(5, 5, 100, 20, hClient, WIN2_BTN1, 0);
        BUTTON_SetTitle(hItem, "Alpha Test");
        hItem = BUTTON_Create(5, 30, 100, 20, hClient, WIN2_BTN2, 0);
        BUTTON_SetTitle(hItem, "TEXTBOX Test");
        hItem = BUTTON_Create(5, 55, 100, 20, hClient, WIN2_BTN3, 0);
        BUTTON_SetTitle(hItem, "LISTBOX Test");
        hItem = BUTTON_Create(5, 80, 100, 20, hClient, WIN2_BTN4, 0);
        BUTTON_SetTitle(hItem, "GRAPH Test");
        hItem = BUTTON_Create(5, 105, 100, 20, hClient, WIN2_BTN5, 0);
        BUTTON_SetTitle(hItem, "Benchmark");
        break;
    case WM_BUTTON_RELEASED:
        /* 根据点击的按键创建窗口 */
        switch (WM_GetDialogId(pMsg->hWinSrc)) {
        case WIN2_BTN1:
            Create_Window1();
            break;
        case WIN2_BTN2:
            Create_Window3();
            break;
        case WIN2_BTN3:
            Create_Window4();
            break;
        case WIN2_BTN4:
            Create_Window5();
            break;
        case WIN2_BTN5:
            Create_Window6();
            break;
        }
        break;
    }
}

void Create_Window2(void)
{
    GUI_HWIN hWin;

    hWin = WINDOW_Create(30, 80, 120, 160, NULL, WINDOW2, WM_WS_MOVE, Window2_Cb);
    WINDOW_SetTitle(hWin, "S-GUI Demo");
}
