#ifndef __WINDOW_H
#define __WINDOW_H

#include "WIDGET.h"

typedef struct {
    WIDGET Widget;
    WM_HWIN hClient;        /* 客户区句柄 */
    WM_HWIN hFocus;         /* 窗口当前焦点 */
    WM_HWIN hBtn;           /* 关闭按钮 */
    char *Title;
    u_8  CaptionHeight;     /* 标题栏高度 */
    WM_CALLBACK *UserCb;    /* 用户功能回调函数 */
}WINDOW_Obj;

WM_HWIN WINDOW_Create(int x0, int y0,  int xSize, int ySize,
    WM_HWIN hParent, int Id, u_8 Style, WM_CALLBACK *cb);
GUI_RESULT WINDOW_SetTitle(WM_HWIN hWin, const char *str);
GUI_RESULT WINDOW_SetFont(WM_HWIN hWin, GUI_FONT *Font);

#endif
