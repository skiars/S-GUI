#include "GUI_Test.h"
#include "GUI.h"
#include <stdio.h>
#include <string.h>

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

#define RWIN_TBX1    WM_USER_ID + 0x0060

void GUI_2D_Test(void);
void Create_Window2(void);

static char _Str[15] = "";
static u_16 _FpsVal; /* 帧率 */

static void _RootWinPaint(WM_HWIN hWin)
{
    /* 绘制背景 */
    GUI_DrawBitmap(0, 0, 640, 320, &bmpic_rootwin);
    GUI_DispStringCurRect(10, 300, _Str, 0x00000000, Font_ASCII_8X16);
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
        hItem = BUTTON_Create(12, 20, 72, 30, hWin, WIN1_BTN2, 0);
        BUTTON_SetTitle(hItem, "Alpha+");
        hItem = BUTTON_Create(108, 20, 72, 30, hWin, WIN1_BTN3, 0);
        BUTTON_SetTitle(hItem, "Alpha-");
        hItem = BUTTON_Create(70, 70, 60, 30, hWin, WIN1_BTN1, 0);
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
    case WM_KEY_CHECKED:
        GUI_GetKey();
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
    case WM_KEY_CHECKED:
        GUI_GetKey();
        WINDOW_SetAllAlpha(pMsg->hWin, 30);
        break;
    }
}

void Create_Window3(void)
{
    GUI_HWIN hWin, hWin2, hWin3;

    hWin = WINDOW_Create(10, 10, 220, 300, NULL, WINDOW3, WM_WS_MOVE, Window3_Cb);
    if (hWin == NULL) {
        return;
    }
    WINDOW_SetTitle(hWin, "Infomation");
    hWin = WM_GetClientWindow(hWin);
    hWin2 = TEXTBOX_Create(0, 0, 214, 210, hWin, WIN3_TBX1, 0);
    //WIDGET_SetFont(hWin2, Font_ASCII_8X16);
    TEXTBOX_SetText(hWin2, "This is a Small Graphical User Interface.\n"
        "It\'s author is Guan Wenliang.\n"
        "This is a demonstration...");
    hWin3 = BUTTON_Create(100, 220, 60, 30, hWin, WIN3_BTN1, 0);
    BUTTON_SetTitle(hWin3, "Exit");
    hWin3 = BUTTON_Create(30, 220, 60, 30, hWin, WIN3_BTN2, 0);
    BUTTON_SetTitle(hWin3, "Alpha");
}

void Window4_Cb(WM_MESSAGE *pMsg)
{
    WM_HWIN hItem;
    switch (pMsg->MsgId) {
    case WM_BUTTON_RELEASED:
        if (WM_GetDialogId(pMsg->hWinSrc) == Win4_BTN1) {
            WM_DeleteWindow(pMsg->hWin);
        }
        break;
    case WM_KEY_CHECKED:
        GUI_GetKey();
        hItem = WM_GetDialogItem(pMsg->hWin, WIN4_LBX1);
        LISTBOX_ItemDown(hItem);
        break;
    }
}

void Create_Window4(void)
{
    GUI_HWIN hWin, hWin2, hWin3;

    hWin = WINDOW_Create(10, 10, 220, 300, NULL, WINDOW4, WM_WS_MOVE, Window4_Cb);
    if (hWin == NULL) return;
    WINDOW_SetTitle(hWin, "LISTBOX Test");
    hWin = WM_GetClientWindow(hWin);
    hWin2 = LISTBOX_Create(0, 0, 214, 210, hWin, WIN4_LBX1, 0, 100);
    LISTBOX_ScrollDisplay(hWin2);
    //WIDGET_SetFont(hWin2, Font_ASCII_8X16);
    LISTBOX_AddList(hWin2, "This is a Listbox test...Scroll...asdfg-ASDFGqwertyuiopQWERTYUIOP");
    LISTBOX_AddList(hWin2, "Hello");
    LISTBOX_AddList(hWin2, "abcdefg");
    LISTBOX_AddList(hWin2, "ABCDEFG");
    LISTBOX_AddList(hWin2, "hijklmn");
    LISTBOX_AddList(hWin2, "HIJKLMN");
    LISTBOX_AddList(hWin2, "dgs");
    LISTBOX_AddList(hWin2, "saff");
    LISTBOX_AddList(hWin2, "csafe");
    LISTBOX_AddList(hWin2, "srwetw");
    LISTBOX_AddList(hWin2, "vfdgreg");
    LISTBOX_AddList(hWin2, "vfdgreg");
    LISTBOX_AddList(hWin2, "vfdgreg");
    LISTBOX_AddList(hWin2, "Alpha Test");
    LISTBOX_AddList(hWin2, "ListBox Test");
    //LISTBOX_SetSelFromStr(hWin2, "saff");
    hWin3 = BUTTON_Create(70, 220, 60, 30, hWin, Win4_BTN1, 0);
    BUTTON_SetTitle(hWin3, "Exit");
}

void Window5_Cb(WM_MESSAGE *pMsg)
{
    switch (pMsg->MsgId) {
    case WM_BUTTON_RELEASED:
        if (WM_GetDialogId(pMsg->hWinSrc) == WIN5_BTN1) {
            WM_DeleteWindow(pMsg->hWin);
        }
        break;
    case WM_KEY_CHECKED:
        break;
    }
}

void Create_Window5(void)
{
    i_16 yd[10] = { 15, 10, 15, 38, 58, 99, 85, 32, 56, 78 };
    i_16 xd[10] = { 10, 20, 30, 40, 50, 60, 70, 80, 90, 100 };
    GUI_HWIN hWin, hWin2, hWin3;

    hWin = WINDOW_Create(10, 10, 220, 300, NULL, WINDOW5, 0, Window5_Cb);
    if (hWin == NULL) {
        return;
    }
    WINDOW_SetTitle(hWin, "GRPHA Test");
    hWin = WM_GetClientWindow(hWin);
    hWin2 = GRAPH_Create(0, 0, 214, 210, hWin, WIN5_GPH1, 0);
    GRAPH_SetData(hWin2, xd, yd, 10);

    hWin3 = BUTTON_Create(70, 220, 60, 30, hWin, WIN5_BTN1, 0);
    BUTTON_SetTitle(hWin3, "Exit");
}

void Window2_Cb(WM_MESSAGE *pMsg)
{
    WM_HWIN hItem, hWin = pMsg->hWin, hClient;

    switch (pMsg->MsgId) {
    case WM_CREATED:
        hClient = WM_GetClientWindow(hWin);
        hItem = BUTTON_Create(5, 5, 112, 35, hClient, WIN2_BTN1, 0);
        BUTTON_SetTitle(hItem, "Alpha Test");
        hItem = BUTTON_Create(5, 45, 150, 35, hClient, WIN2_BTN2, 0);
        BUTTON_SetTitle(hItem, "TEXTBOX Test");
        hItem = BUTTON_Create(5, 85, 150, 35, hClient, WIN2_BTN3, 0);
        BUTTON_SetTitle(hItem, "LISTBOX Test");
        hItem = BUTTON_Create(5, 125, 150, 35, hClient, WIN2_BTN4, 0);
        BUTTON_SetTitle(hItem, "GRAPH Test");
        BUTTON_SetFont(hItem, Font_ASCII_2PP);
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
    case WM_KEY_CHECKED:
        GUI_CleanKeyBuffer();
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
