#ifndef __WINDOW_H
#define __WINDOW_H

#include "GUI.h"

typedef struct {
    WIDGET Widget;
    char *Title;
    u_8  CaptionHeight;     /* 标题栏高度 */
    WM_CALLBACK *UserCb;    /* 用户功能回调函数 */
}WINDOW_Obj;

WM_hWin WINDOW_Create(i_16 x0, i_16 y0,  u_16 xSize, u_16 ySize,
                      WM_hWin hParent, u_16 Id, u_8 Flag, WM_CALLBACK *cb);
GUI_RESULT WINDOW_SetTitle(WM_hWin hWin, const char *str);
GUI_RESULT WINDOW_SetFont(WM_hWin hWin, GUI_FontType Font);
GUI_RESULT WINDOW_SetAllAlpha(WM_hWin hWin, u_8 Alpha);

#endif

