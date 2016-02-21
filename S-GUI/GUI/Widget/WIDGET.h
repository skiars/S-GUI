#ifndef __WIDGET_H
#define __WIDGET_H

#include "GUI_WM.h"
#include "GUI_Font.h"

/* 窗口标志定义 */
#define WIDGET_CLIENT          0x00
#define WIDGET_ROOTWIN         0x01
#define WIDGET_BUTTON          0x02
#define WIDGET_LISTBOX         0x03
#define WIDGET_WINDOW          0x04
#define WIDGET_TEXTBOX         0x05
#define WIDGET_SCROLLBAR       0x06
#define WIDGET_ICONVIEW        0x07
#define WIDGET_GRAPH           0x08

#define WIDGET_DRAW_EDGE1      0x0000
#define WIDGET_DRAW_EDGE2      0x0001
#define WIDGET_DRAW_EDGE3      0x0002

#define WIDGET_DRAW_BACK1      0x0010
#define WIDGET_DRAW_BACK2      0x0011

#define WIDGET_DRAW_FONT1      0x0020
#define WIDGET_DRAW_FONT2      0x0021

/* WIDGET皮肤位置定义 */
#define WIDGET_ALL             0x00
#define WIDGET_EDGE            0x01
#define WIDGET_BACK            0x02
#define WIDGET_CAPTION         0x03
#define WIDGET_FONT            0x04

/* 检查控件标识符是否正确 */
#define WIDGET_SignErrorReturn(h, s) \
    { if (WM_CheckWindowSign(h, s) == GUI_ERR) return GUI_ERR; }
#define WIDGET_SignErrorReturnVoid(h, s) \
    { if (WM_CheckWindowSign(h, s) == GUI_ERR) return; }

typedef struct {
    GUI_COLOR      EdgeColor[3];    /* 边框颜色 */
    GUI_COLOR      BackColor[2];    /* 内部背景颜色 */
    GUI_COLOR      CaptionColor[2]; /* 标题颜色 */
    GUI_FontType   Font;            /* 字体 */
    GUI_COLOR      FontColor[2];    /* 字体颜色 */
}WIDGET_SKIN;      /* 窗口控件皮肤 */

typedef struct {
    WM_Obj Win;
    WIDGET_SKIN Skin;
}WIDGET;

void WIDGET_SetTransWindow(GUI_HWIN hWin);
void WIDGET_Alpha(GUI_HWIN hWin, u_8 Part, u_8 Id, u_8 Alpha);
GUI_FontType WIDGET_GetFont(GUI_HWIN hWin);
void WIDGET_SetFont(GUI_HWIN hWin, GUI_FontType Font);
GUI_COLOR WIDGET_GetBackColor(GUI_HWIN hWin, u_8 n);
GUI_COLOR WIDGET_GetEdgeColor(GUI_HWIN hWin, u_8 n);
GUI_COLOR WIDGET_GetCaptionColor(GUI_HWIN hWin, u_8 n);
GUI_COLOR WIDGET_GetFontColor(GUI_HWIN hWin, u_8 n);

#endif
