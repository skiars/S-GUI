#ifndef __WINDOW_H
#define __WINDOW_H

#include "GUI.h"

#define WINDOW_DEF_CAPHEIGHT   32
/* 默认的窗体caption背景色caption的颜色. */
/* 以及窗体背景色. */
#define WINDOW_CAPTION_UPC          0x00524C3C  /* 窗口CAPTION上部分填充色 */
#define WINDOW_CAPTION_DOWNC        0x00423E33  /* 窗口CAPTION下部分填充色 */
#define WINDOW_TITLE_COLOR          0x00CDCD96  /* 窗口标题颜色 */
#define WINDOW_BODY_BKC             0x00232329  /* 窗口底色 */
#define WINDOW_FONT_COLOR           0x00FFFFFF  /* 窗口字体颜色 */

/* 标准窗体边框颜色定义 */
#define STD_WIN_RIM_OUTC            0x0033302A  /* 外线颜色 */
#define STD_WIN_RIM_MIDC            0x006D6654  /* 中线颜色 */
#define STD_WIN_RIM_INC             0x00423D30  /* 内线颜色 */

typedef struct {
    WIDGET Widget;
    char *Title;
    u_8  CaptionHeight;     /* 标题栏高度 */
    WM_CALLBACK *UserCb;    /* 用户功能回调函数 */
}WINDOW_Obj;

WM_hWin WINDOW_Create(i_16 x0, i_16 y0,  u_16 xSize, u_16 ySize,
                      WM_hWin hParent, u_16 Id, u_8 Flag, WM_CALLBACK *cb);
u_8 WINDOW_SetTitle(WM_hWin hWin, const char *str);
u_8 WINDOW_SetFont(WM_hWin hWin, GUI_FontType Font);
void WINDOW_SetAllAlpha(WM_hWin hWin, u_8 Alpha);

#endif

