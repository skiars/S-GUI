#include "GUI_Test.h"
#include "GUI.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <math.h>
#include "GUI_AntiAliasing.h"

#include "Bitmap.h"
#include "game.h"

#define WINDOW1      (WM_USER_ID + 0x0010)
#define WINDOW2      (WM_USER_ID + 0x0020)
#define WINDOW3      (WM_USER_ID + 0x0030)
#define WINDOW4      (WM_USER_ID + 0x0040)
#define WINDOW5      (WM_USER_ID + 0x0050)

#define WIN1_BTN1    (WINDOW1 + 0x0001)
#define WIN1_BTN2    (WINDOW1 + 0x0002)

#define WIN2_BTN1    (WINDOW2 + 0x0001)
#define WIN2_BTN2    (WINDOW2 + 0x0002)
#define WIN2_BTN3    (WINDOW3 + 0x0002)

void Create_Window1(void);

static char _Str[30];
static int _FpsVal; /* 帧率 */
static u_8 _CPUUsage; /* CPU占用率 */

static void _RootWinPaint(WM_HWIN hWin)
{
    /* 绘制背景 */
    GUI_SetForeground(0x00C0C0C0);
    //GUI_FillRect(0, 0, GUI_GetScreenWidth(), GUI_GetScreenHeight());
    GUI_DrawBitmap(0, 0, 480, 320, &bmpic_rootwin);
    GUI_DispString(10, 300, _Str);
}

static void _RootWinTimer(WM_HWIN hWin)
{
    GUI_RECT Rect = { 10, 300, 150, 320 };

#if _MSC_VER > 1500
    sprintf_s(_Str, sizeof(_Str), "FPS: %d, CPU: %d%%", _FpsVal, _CPUUsage);
#else
    sprintf(_Str, "FPS: %d, CPU: %d%%", _FpsVal, _CPUUsage);
#endif // _WIN32

    WM_InvalidateRect(gui_rootwin, &Rect);
    //WM_Invalidate(gui_rootwin);
    _FpsVal = 0; /* 帧率清零 */
    printf("Mem usage: %d bytes\n", GUI_GetMemUsage());
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
    GUI_SetFont(&GUI_FontUI17_4pp);
    while (1) {
        GUI_Delay(20);
        ++_FpsVal; /* 统计帧率 */
    }
}

void Window2_Cb(WM_MESSAGE *pMsg)
{
    WM_HWIN hWin, hItem;

    hWin = WM_GetClientWindow(pMsg->hWin);
    switch (pMsg->MsgId) {
    case WM_CREATED:
        hItem = BUTTON_Create(12, 20, 70, 20, hWin, WIN2_BTN2, 0);
        BUTTON_SetTitle(hItem, "Button 1");
        hItem = BUTTON_Create(108, 20, 70, 20, hWin, WIN2_BTN3, 0);
        BUTTON_SetTitle(hItem, "Button 2");
        hItem = BUTTON_Create(70, 70, 70, 20, hWin, WIN2_BTN1, 0);
        BUTTON_SetTitle(hItem, "Exit");
        break;
    case WM_BUTTON_RELEASED:
        if (WM_GetDialogId(pMsg->hWinSrc) == WIN2_BTN1) {
            WM_DeleteWindow(pMsg->hWin);
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

void Window4_Cb(WM_MESSAGE *pMsg)
{
    WM_HWIN hWin;
    int x, y;
    static int pos, status;
    static PosPoint p[3] = {
        10, 10,
        300, 100,
        10, 120
    };

    hWin = WM_GetClientWindow(pMsg->hWin);
    switch (pMsg->MsgId) {
    case WM_PAINT:
        if (pMsg->hWinSrc == hWin) {
            GUI_Rasterizer *ras = rasterizer_init();
            StrokeLine vc;

            ras->min_ex = 0;
            ras->max_ex = 400;
            ras->min_ey = 0;
            ras->max_ey = 300;

            curve2(ras, p[0].x, p[0].y,
                p[1].x, p[1].y, p[2].x, p[2].y);

            GUI_SetForeground(0x00);
            sweep_scanlines(ras);
            rasterizer_free(ras);

            ras = rasterizer_init();
            ras->min_ex = 0;
            ras->max_ex = 400;
            ras->min_ey = 0;
            ras->max_ey = 300;

            GUI_SetForeground(0x00556B2F);
            vc.width = 10.0f;
            line_set_to(&vc, p[0].x, p[0].y);
            line_move_to(ras, &vc, p[1].x, p[1].y);
            line_move_to(ras, &vc, p[2].x, p[2].y);
            //line_move_to(ras, &vc, p[0].x, p[0].y);
            line_stoke_end(ras, &vc);
            sweep_scanlines(ras);
            rasterizer_free(ras);

            /*GUI_SetForeground(0x00FF0000);
            GUI_SetPenSize(6);
            GUI_DrawPoint((int)p[0].x, (int)p[0].y);
            GUI_DrawPoint((int)p[1].x, (int)p[1].y);
            GUI_DrawPoint((int)p[2].x, (int)p[2].y);*/
        }
        break;
    case WM_CREATED:
        //GUI_TimerCreate(hWin, 0, 40, GUI_TMR_AUTO);
        break;
    case WM_TP_CHECKED:
        GUI_SetWinPos(WM_HandleToPtr(hWin)->rect.x0, WM_HandleToPtr(hWin)->rect.y0);
        x = (((GUI_POINT*)pMsg->Param)[0].x);
        y = (((GUI_POINT*)pMsg->Param)[0].y);
        GUI_ScreenToClient(&x, &y);
        for (pos = 0; pos < 3; ++pos) {
            if (x <= p[pos].x + 3 && x >= p[pos].x - 3 &&
                y <= p[pos].y + 3 && y >= p[pos].y - 3) {
                status = 1;
                break;
            } else {
                status = 0;
            }
        }
    case WM_TP_PRESS:
        if (status) {
            x = (((GUI_POINT*)pMsg->Param)[0].x);
            y = (((GUI_POINT*)pMsg->Param)[0].y);
            GUI_ScreenToClient(&x, &y);
            p[pos].x = (pos_t)x;
            p[pos].y = (pos_t)y;
            WM_Invalidate(hWin);
        }
        break;
    }
}

void Create_Window4(void)
{
    WM_HWIN hWin;

    hWin = WINDOW_Create(100, 100, 400,
        300, NULL, WINDOW4, WM_WS_MOVE, Window4_Cb);
    WINDOW_SetTitle(hWin, "AntiAliasing Test");
}

void Window1_Cb(WM_MESSAGE *pMsg)
{
    WM_HWIN hItem, hWin = pMsg->hWin, hClient;

    switch (pMsg->MsgId) {
    case WM_CREATED:
        hClient = WM_GetClientWindow(hWin);
        hItem = BUTTON_Create(5, 5, 100, 20, hClient, WIN1_BTN1, 0);
        BUTTON_SetTitle(hItem, "Button Test");
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
    Create_Window4();
}
