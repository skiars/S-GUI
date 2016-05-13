#include "game.h"
#include <stdlib.h>

#define GAME_WIN (WM_USER_ID + 0x90)

static GUI_COLOR FgColor = 0x00FF00FF;
static i_16 CubeX[40], UserX = 0;
static int CubeNum = 40, Star = 0, daid = 0;
static char StarStr[50] = "";
static u_32 KeyVal = 0;

void Game_Paint(WM_HWIN hWin)
{
    u_16 xSize, ySize, len, x0, y0;
    int i;
    GUI_RECT *pr;

    pr = &((WM_Obj *)hWin)->Rect;
    xSize = pr->x1 - pr->x0 + 1;
    ySize = pr->y1 - pr->y0 + 1;
    GUI_FillRect(pr->x0, pr->y0, xSize, ySize, 0xFFFFFF);
    GUI_DispStringCurRect(pr->x0, pr->y0, StarStr, 0x00000000, &GUI_FontASCII_8x16);
    for (i = 0; i < CubeNum; ++i) {
        len = i * 2 / 3 + 1;
        x0 = pr->x0 + xSize / 2 - len / 2 + (CubeX[i] + UserX) * (i + CubeNum / 2) / CubeNum;
        y0 = pr->y0 + 100 + i * (ySize - 100) / CubeNum;
        GUI_FillRect(x0, y0, len, len, FgColor);
        GUI_DrawRect(x0 - 1, y0 - 1, len + 2, len + 2, 0x00);
    }
    GUI_FillRect(pr->x0 + xSize / 2 - 5, pr->y1 - 10, 10, 10, 0x00);
}

void KeyPro(void)
{
    if (KeyVal == KEY_LEFT) {
        UserX += 10;
    } else if (KeyVal == KEY_RIGHT) {
        UserX -= 10;
    }
}

void GameCb(WM_MESSAGE *pMsg)
{
    int i;

    switch (pMsg->MsgId) {
    case WM_PAINT:
        Game_Paint(pMsg->hWin);
        break;
    case WM_TIME_UPDATA:
#ifdef _MSC_VER
        sprintf_s(StarStr, sizeof(StarStr), "Star:%d", Star);
#else
        sprintf(StarStr, "%d", Star);
#endif
        if (Star < 500) {
            FgColor = 0x00FF00FF;
        } else if (Star < 1000) {
            FgColor = 0x000000FF;
            GUI_SetWindowTimer(pMsg->hWin, 30);
        } else {
            FgColor = 0x0000FFFF;
            GUI_SetWindowTimer(pMsg->hWin, 20);
        }
        if (!daid) {
            ++Star;
        }
        for (i = CubeNum - 1; i > 0; --i) {
            CubeX[i] = CubeX[i - 1];
        }
        CubeX[0] = rand() % 1000 - 500 - UserX;
        if (CubeX[CubeNum - 1] == UserX) {
           // daid = 1;
        }
        KeyPro();
        WM_Invalidate(pMsg->hWin);
        break;
    case WM_KEYDOWN:
        switch (pMsg->Param) {
        case KEY_ESC:
            pMsg->MsgId = WM_QUIT;
            WM_SendMessageToParent(pMsg->hWin, pMsg);
            break;
        case KEY_LEFT:
            KeyVal = KEY_LEFT;
            break;
        case KEY_RIGHT:
            KeyVal = KEY_RIGHT;
            break;
        }
        break;
    case WM_KEYUP:
        KeyVal = 0;
        break;
    }
}

void GameWin_Cb(WM_MESSAGE *pMsg)
{
    int i;
    GUI_RECT *r;
    WM_HWIN hWin, hItem;

    switch (pMsg->MsgId) {
    case WM_CREATED:
        WINDOW_SetTitle(pMsg->hWin, "Cube Field");
        r = WM_GetClientRect(pMsg->hWin);
        hWin = WM_GetClientWindow(pMsg->hWin);
        hItem = WM_CreateWindowAsChild(0, 0, r->x1 - r->x0 + 1,
            r->y1 - r->y0 + 1, hWin, 0, GAME_WIN, GameCb, 0);
        GUI_SetWindowTimer(hItem, 50);
        for (i = 0; i < CubeNum / 2; ++i) {
            CubeX[i * 2] = -100;
            CubeX[i * 2 + 1] = 100;
        }
        UserX = 0;
        Star = 0;
        daid = 0;
        break;
    case WM_QUIT:
        WM_DeleteWindow(pMsg->hWin);
        break;
    }
}
