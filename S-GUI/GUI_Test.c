#include "GUI_Test.h"
#include "GUI.h"
#include <stdio.h>
#include <string.h>

#define WINDOW1      (WM_USER_ID + 3)
#define WINDOW2      (WM_USER_ID + 2)
#define WINDOW3      (WM_USER_ID + 4)
#define WINDOW4      (WM_USER_ID + 5)

#define WIN2_BTN1    0x0001
#define WIN2_BTN2    0x0002
#define WIN2_BTN3    0x0003

#define WIN1_BTN1    0x0001
#define WIN1_BTN2    0x0002
#define WIN1_BTN3    0x0003

#define WIN3_TBX1    0x0001
#define WIN3_BTN1    0x0002
#define WIN3_BTN2    0x0003

#define WIN4_LBX1    0x0001

//GUI_hWin ListBox_AutoCreate(u16 x0, u16 y0, u16 xSize, u16 ySize, u8 cmd);
//void win1_callback (WM_MESSAGE *pMsg);

//GUI_hWin Window1(void);
//GUI_hWin Window2(void);

/*
static GUI_WIDGET_CREATE_INFO test_win_1[]=
{
    ListBox_AutoCreate, 0, WIN1_LBOX0, 34, 45, 100, 200, 0,
    ListBox_AutoCreate, 0, WIN1_LBOX1, 0, 60, 100, 200, 0,
};
*/

void GUI_2D_Test(void);
void Create_Window2(void);

//GUI²âÊÔ
void GUI_Test(void)
{
    GUI_Init();

    Create_Window2();
    while(1) {
        GUI_Delay(20);
    }
}

/*
GUI_hWin ListBox_AutoCreate(u16 x0, u16 y0, u16 xSize, u16 ySize, u8 cmd)
{
    return LISTBOX_Creat(x0, y0, xSize, ySize, cmd,50);
}

void win1_callback (WM_MESSAGE *pMsg)
{
    u16 i;
    u8 str[50];
    GUI_hWin hItem;
    if(pMsg -> MsgId == WM_INIT_DIALOG)
    {
        hItem = WM_GetDialogItem(pMsg -> hWin,WIN1_LBOX0);
        for(i=1;i<=14;i++)
        {
            sprintf((char *)str,"Item%d",i);
            LISTBOX_AddString((LISTBOX_Handle)hItem,(PData)str);
        }
        LISTBOX_Draw((LISTBOX_Handle)hItem);

        hItem = WM_GetDialogItem(pMsg -> hWin,WIN1_LBOX1);
        for(i=1;i<=14;i++)
        {
            sprintf((char *)str,"Item%d",i);
            LISTBOX_AddString((LISTBOX_Handle)hItem,(PData)str);
        }
        LISTBOX_Draw((LISTBOX_Handle)hItem);
    }
    else if(pMsg -> MsgId ==  WM_KEY)
    {
        if(GUI_GetKey() == GUI_KEY_UP) {
            hItem = WM_GetDialogItem(pMsg -> hWin,WIN1_LBOX1);
            LISTBOX_Up((LISTBOX_Handle)hItem);
        }
    }
}

GUI_hWin Window1(void)
{
    return GUI_CreateDialogBox(test_win_1,GUI_COUNTOF(test_win_1), win1_callback, 0, 0);
}

GUI_hWin Window2(void)
{
    return GUI_CreateDialogBox(test_win_1,GUI_COUNTOF(test_win_1), win1_callback, 25, 50);
}
*/

void Window1_Cb(WM_MESSAGE *pMsg)
{
    static u_8 Alpha;
    WM_hWin hWin;
    switch (pMsg->MsgId) {
        case WM_BUTTON_RELEASED :
            if (WM_GetDialogId(pMsg->hWinSrc) == WIN1_BTN1) {
                WM_DeleteWindow(pMsg->hWin);
                Alpha = 0;
            } else if (WM_GetDialogId(pMsg->hWinSrc) == WIN1_BTN2) {
                hWin = pMsg->hWin;
                WINDOW_SetAllAlpha(hWin, Alpha);
                if (Alpha < 200 ) Alpha += 10;
            } else if (WM_GetDialogId(pMsg->hWinSrc) == WIN1_BTN3) {
                hWin = pMsg->hWin;
                WINDOW_SetAllAlpha(hWin, Alpha);
                if (Alpha >= 10) Alpha -= 10;
            }
            break;
        case WM_KEY_CHECKED :
                GUI_GetKey();
                WM_DeleteWindow(pMsg->hWin);
                Alpha = 0;
            break;
    }
}

void Create_Window1(void)
{
    GUI_hWin hWin, hWin2, hWin3;

    hWin = WINDOW_Create(7,10,200,140,NULL, WINDOW1,0, Window1_Cb);
    if (hWin == NULL) return;
    WINDOW_SetTitle(hWin, "Hello World!");
    hWin2 = BUTTON_Create(12,20,72,30,hWin,WIN1_BTN2,0);
    BUTTON_SetTitle(hWin2, "Alpha+");
    hWin2 = BUTTON_Create(108,20,72,30,hWin,WIN1_BTN3,0);
    BUTTON_SetTitle(hWin2, "Alpha-");
    hWin2 = hWin;
    hWin3 = BUTTON_Create(70,70,60,30,hWin,WIN1_BTN1,0);
    BUTTON_SetTitle(hWin3, "Exit");
}

void Window3_Cb(WM_MESSAGE *pMsg)
{
    u_16 Id;
    WM_hWin hItem;
    
    switch (pMsg->MsgId) {
        case WM_BUTTON_RELEASED :
            Id = WM_GetDialogId(pMsg->hWinSrc);
            if (Id == WIN3_BTN1) {
                WM_DeleteWindow(pMsg->hWin);
            } else if (Id == WIN3_BTN2) {
                WINDOW_SetAllAlpha(pMsg->hWin, 30);
                hItem = WM_GetDialogItem(pMsg->hWin, WIN4_LBX1);
                WINDOW_SetAllAlpha(hItem, 80);
            }
            break;
        case WM_KEY_CHECKED :
                GUI_GetKey();
                WINDOW_SetAllAlpha(pMsg->hWin, 30);
            break;
    }
}

void Create_Window3(void)
{
    GUI_hWin hWin, hWin2, hWin3;

    hWin = WINDOW_Create(10,10,220,300,NULL, WINDOW3,0, Window3_Cb);
    if (hWin == NULL) return;
    WINDOW_SetTitle(hWin, "Infomation");
    hWin2 = TEXTBOX_Create(0,0,214,210,hWin,WIN3_TBX1,WM_WINDOW_UN_DRAW);
    //WIDGET_SetFont(hWin2, Font_ASCII_8X16);
    TEXTBOX_SetText(hWin2, "This is a Small Graphical User Interface.\n"
                           "It\'s author is Guan Wenliang.\n"
                           "This is a demonstration...");
    hWin3 = BUTTON_Create(100,220,60,30,hWin,WIN3_BTN1,0);
    BUTTON_SetTitle(hWin3, "Exit");
    hWin3 = BUTTON_Create(30,220,60,30,hWin,WIN3_BTN2,0);
    BUTTON_SetTitle(hWin3, "Alpha");
}

void Window4_Cb(WM_MESSAGE *pMsg)
{
    WM_hWin hItem;
    switch (pMsg->MsgId) {
        case WM_BUTTON_RELEASED :
            if (WM_GetDialogId(pMsg->hWinSrc) == WIN3_BTN1) {
                WM_DeleteWindow(pMsg->hWin);
            }
            break;
        case WM_KEY_CHECKED :
            GUI_GetKey();
            hItem = WM_GetDialogItem(pMsg->hWin, WIN4_LBX1);
            LISTBOX_ItemDown(hItem);
            break;
    }
}

void Create_Window4(void)
{
    GUI_hWin hWin, hWin2, hWin3;

    hWin = WINDOW_Create(10,10,220,300,NULL, WINDOW4,0, Window4_Cb);
    if (hWin == NULL) return;
    WINDOW_SetTitle(hWin, "LISTBOX Test");
    hWin2 = LISTBOX_Create(0,0,214,210,hWin,WIN4_LBX1, 0, 100);
    //WIDGET_SetFont(hWin2, Font_ASCII_8X16);
    LISTBOX_addlist(hWin2, "This is a Listbox test...Scroll...asdfg-ASDFGqwertyuiopQWERTYUIOP");
    LISTBOX_addlist(hWin2, "Hello");
    LISTBOX_addlist(hWin2, "abcdefg");
    LISTBOX_addlist(hWin2, "ABCDEFG");
    LISTBOX_addlist(hWin2, "hijklmn");
    LISTBOX_addlist(hWin2, "HIJKLMN");
    LISTBOX_addlist(hWin2, "dgs");
    LISTBOX_addlist(hWin2, "saff");
    LISTBOX_addlist(hWin2, "csafe");
    LISTBOX_addlist(hWin2, "srwetw");
    LISTBOX_addlist(hWin2, "vfdgreg");
    LISTBOX_addlist(hWin2, "vfdgreg");
    LISTBOX_addlist(hWin2, "vfdgreg");
    LISTBOX_addlist(hWin2, "Alpha Test");
    //LISTBOX_SetSelFromStr(hWin2, "saff");
    hWin3 = BUTTON_Create(70,220,60,30,hWin,WIN3_BTN1,0);
    BUTTON_SetTitle(hWin3, "Exit");
}

void Window2_Cb(WM_MESSAGE *pMsg)
{
    switch (pMsg->MsgId) {
        case WM_BUTTON_RELEASED :
            if (WM_GetDialogId(pMsg->hWinSrc) == WIN2_BTN1) {
                Create_Window1();
            } else if (WM_GetDialogId(pMsg->hWinSrc) == WIN2_BTN2) {
                Create_Window3();
            } else if (WM_GetDialogId(pMsg->hWinSrc) == WIN2_BTN3) {
                Create_Window4();
            }
            break;
        case WM_KEY_CHECKED :
                GUI_CleanKeyBuffer();
                Create_Window1();
            break;
    }
}

void Create_Window2(void)
{
    GUI_hWin hWin, hWin1;

    hWin = WINDOW_Create(30,80,180,170,NULL,WINDOW2,0, Window2_Cb);
    WINDOW_SetTitle(hWin, "S-GUI Demo");
    hWin1 = BUTTON_Create(5, 5, 112, 35,hWin,WIN2_BTN1,0);
    BUTTON_SetTitle(hWin1, "Alpha Test");
    hWin1 = BUTTON_Create(5, 45, 150, 35,hWin,WIN2_BTN2,0);
    BUTTON_SetTitle(hWin1, "TEXTBOX Test");
    hWin1 = BUTTON_Create(5, 85, 150, 35,hWin,WIN2_BTN3,0);
    BUTTON_SetTitle(hWin1, "LISTBOX Test");
    BUTTON_SetFont(hWin1, Font_ASCII_2PP);
}
