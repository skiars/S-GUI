#include "GUI_Test.h"
#include "GUI.h"
#include <stdio.h>
#include <string.h>
#include <math.h>

#include "Bitmap.h"

#define WINDOW2      (WM_USER_ID + 0x0010)
#define WINDOW1      (WM_USER_ID + 0x0020)
#define WINDOW3      (WM_USER_ID + 0x0030)
#define WINDOW4      (WM_USER_ID + 0x0040)
#define WINDOW5      (WM_USER_ID + 0x0050)

#define WIN2_BTN1    (WINDOW2 + 0x0001)
#define WIN2_BTN2    (WINDOW2 + 0x0002)
#define WIN2_BTN3    (WINDOW2 + 0x0003)
#define WIN2_BTN4    (WINDOW2 + 0x0004)

#define WIN1_BTN1    (WINDOW1 + 0x0001)
#define WIN1_BTN2    (WINDOW1 + 0x0002)
#define WIN1_BTN3    (WINDOW1 + 0x0003)

#define WIN3_TBX1    (WINDOW3 + 0x0001)
#define WIN3_BTN1    (WINDOW3 + 0x0002)
#define WIN3_BTN2    (WINDOW3 + 0x0003)

#define WIN4_LBX1    (WINDOW4 + 0x0001)
#define Win4_BTN1    (WINDOW4 + 0x0002)

#define WIN5_GPH1    (WINDOW5 + 0x0001)
#define WIN5_BTN1    (WINDOW5 + 0x0002)
#define WIN5_BTN2    (WINDOW5 + 0x0003)

#define RWIN_TBX1    WM_USER_ID + 0x0060

void GUI_2D_Test(void);
void Create_Window2(void);

static char _Str[15] = "";
static u_16 _FpsVal; /* 帧率 */

static void _RootWinPaint(WM_HWIN hWin)
{
    static i_16 y1 = 300;
    /* 绘制背景 */
    GUI_FillRect(0, 0, 480, 320, 0x00ffffff);
    GUI_DrawBitmap(0, 0, 640, 320, &bmpic_rootwin);
    GUI_DispStringCurRect(10, 300, _Str, 0x00000000, Font_ASCII_8X16);
    GUI_DrawLine(100, 100, 200, 5, 0);
}

static void _RootWinTimer(WM_HWIN hWin)
{
    GUI_RECT Rect = { 10, 300, 100, 320 };

    sprintf_s(_Str, sizeof(_Str), "FPS: %d", _FpsVal);
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
    case WM_KEYUP:
        WM_DeleteWindow(pMsg->hWin);
        Alpha = 0;
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
    case WM_KEYUP:
        WINDOW_SetAllAlpha(pMsg->hWin, 30);
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
    WM_HWIN hWin, hItem;

    switch (pMsg->MsgId) {
    case WM_CREATED:
        hWin = WM_GetClientWindow(pMsg->hWin);
        hItem = LISTBOX_Create(0, 0, 214, 210, hWin, WIN4_LBX1, 0);
        LISTBOX_ScrollDisplay(hItem);
        //WIDGET_SetFont(hWin2, Font_ASCII_8X16);
        LISTBOX_AddList(hItem, "This is a Listbox test...Scroll...asdfg-ASDFGqwertyuiopQWERTYUIOP");
        LISTBOX_AddList(hItem, "Hello");
        LISTBOX_AddList(hItem, "abcdefg");
        LISTBOX_AddList(hItem, "ABCDEFG");
        LISTBOX_AddList(hItem, "hijklmn");
        LISTBOX_AddList(hItem, "HIJKLMN");
        LISTBOX_AddList(hItem, "dgs");
        LISTBOX_AddList(hItem, "saff");
        LISTBOX_AddList(hItem, "csafe");
        LISTBOX_AddList(hItem, "srwetw");
        LISTBOX_AddList(hItem, "vfdgreg");
        LISTBOX_AddList(hItem, "vfdgreg");
        LISTBOX_AddList(hItem, "vfdgreg");
        LISTBOX_AddList(hItem, "Alpha Test");
        LISTBOX_AddList(hItem, "ListBox Test");
        //LISTBOX_SetSelFromStr(hWin2, "saff");
        hItem = BUTTON_Create(70, 250, 60, 20, hWin, Win4_BTN1, 0);
        BUTTON_SetTitle(hItem, "Exit");
        break;
    case WM_BUTTON_RELEASED:
        if (WM_GetDialogId(pMsg->hWinSrc) == Win4_BTN1) {
            WM_DeleteWindow(pMsg->hWin);
        }
        break;
    case WM_KEYUP:
        hItem = WM_GetDialogItem(pMsg->hWin, WIN4_LBX1);
        LISTBOX_ItemDown(hItem);
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
        hData = GRAPH_XY_DataCreate(x, y2, 300, 300, 0x00ff0000, 0);
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
        GRAPH_XY_DataEdit(hData, x, y2, 300);
        WM_Invalidate(hItem);
    }
}

void Create_Window5(void)
{
    GUI_HWIN hWin;

    hWin = WINDOW_Create(10, 10, 220, 300, NULL, WINDOW5, WM_WS_MOVE, Window5_Cb);
    if (hWin == NULL) {
        return;
    }
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
        }
        break;
    case WM_KEYUP:
        Create_Window1();
        break;
    }
}

void Create_Window2(void)
{
    GUI_HWIN hWin;

    hWin = WINDOW_Create(30, 80, 180, 220, NULL, WINDOW2, WM_WS_MOVE, Window2_Cb);
    WINDOW_SetTitle(hWin, "S-GUI Demo");
}
