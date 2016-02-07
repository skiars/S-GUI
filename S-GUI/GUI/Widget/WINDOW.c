#include "WINDOW.h"

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

/* Window自绘函数 */
static void _WINDOW_Paint(WM_hWin hWin)
{
    i_16 x0, y0;
    u_16 xSize, ySize;
    GUI_COLOR Color;
    GUI_RECT Rect;
    WINDOW_Obj *pObj = hWin;

    Rect = ((WM_Obj*)hWin)->Rect;
    x0 = Rect.x0;
    y0 = Rect.y0;
    xSize = Rect.x1 - x0 + 1;
    ySize = Rect.y1 - y0 + 1;
    
    /* 绘制外框 */
    Color = pObj->Widget.Skin.EdgeColor[0];
    GUI_DrawRect(x0, y0, xSize, ySize, Color);
    /* 绘制中框 */
    Color = pObj->Widget.Skin.EdgeColor[1];
    GUI_VertLine(x0 + 1, y0  + pObj->CaptionHeight + 1,
                       ySize - pObj->CaptionHeight - 2, Color);
    GUI_VertLine(x0 + xSize - 2, y0  + pObj->CaptionHeight + 1,
                       ySize - pObj->CaptionHeight - 2, Color);
    GUI_HoriLine(x0 + 1, y0  + ySize - 2, xSize - 2, Color);
    /* 绘制内框 */
    Color = pObj->Widget.Skin.EdgeColor[2];
    GUI_DrawRect(x0 + 2, y0 + pObj->CaptionHeight + 1,
                 xSize - 4, ySize - pObj->CaptionHeight - 3, 
                 Color);

    /* 绘制标题栏 */
    Color = pObj->Widget.Skin.CaptionColor[0];
    GUI_FillRect(x0 + 1, y0 + 1, xSize - 2,
                 pObj->CaptionHeight / 2, Color);
    Color = pObj->Widget.Skin.CaptionColor[1];
    GUI_FillRect(x0 + 1, y0 + pObj->CaptionHeight / 2 + 1,
                 xSize - 2, pObj->CaptionHeight / 2, Color);

    /* 绘制内部背景 */
    Color = pObj->Widget.Skin.BackColor[0];
    GUI_FillRect(x0 + 3, y0 + pObj->CaptionHeight + 2,
                 xSize - 6, ySize - pObj->CaptionHeight - 5, Color);
    /* 绘制标题 */
    Color = pObj->Widget.Skin.FontColor[0];
    GUI_DspStringCurRectMiddle(x0 + 1, y0 + 1, xSize - 2,
                               pObj->CaptionHeight,
                               pObj->Title, Color, 
                               WIDGET_GetFont(pObj));
}

/* WINDOW控件自动回调函数 */
static void _WINDOW_Callback(WM_MESSAGE *pMsg)
{
    i_16 dX, dY;
    
    /* 检测是否为WINDOW控件 */
    if (WM_CheckWindowSign(pMsg->hWin, WIDGET_WINDOW)) {
        return;
    }
    switch (pMsg->MsgId) {
        case WM_PAINT :
            _WINDOW_Paint(pMsg->hWin);
            break;
        case WM_DELETE :
            GUI_fastfree(pMsg->hWin);
            break;
        case WM_TP_CHECKED :
            WM_SetActiveMainWindow(pMsg->hWin);
            break;
        case WM_TP_PRESS:
            dX = ((GUI_POINT*)pMsg->Param)[1].x;
            dY = ((GUI_POINT*)pMsg->Param)[1].y;
            WM_MoveWindow(pMsg->hWin, dX, dY);
            break;
        case WM_TP_LEAVE:
            dX = ((GUI_POINT*)pMsg->Param)[1].x;
            dY = ((GUI_POINT*)pMsg->Param)[1].y;
            WM_MoveWindow(pMsg->hWin, dX, dY);
            break;
        default : /* 执行用户回调函数 */
            ((WINDOW_Obj*)pMsg->hWin)->UserCb(pMsg);
    }
    
}

/*
 * 创建窗口控件
 * x0:WINDOW控件的最左像素(相对于父窗口)
 * y0:WINDOW控件的最右像素(相对于父窗口)
 * xSize:WINDOW控件的水平宽度
 * ySize:WINDOW控件的竖直高度
 * hParent:父窗口句柄
 * Id:窗口ID
 * Flag:窗口状态
 * cb:用户回调历程指针
 **/
WM_hWin WINDOW_Create(i_16 x0,
                      i_16 y0,
                      u_16 xSize,
                      u_16 ySize,
                      WM_hWin hParent,
                      u_16 Id,
                      u_8 Flag,
                      WM_CALLBACK *cb)
{
    WINDOW_Obj *pObj;
    
    pObj = WM_CreateWindowAsChild(x0, y0, xSize, ySize, hParent, Flag,
                                  WIDGET_WINDOW, Id, _WINDOW_Callback,
                                  sizeof(WINDOW_Obj) - sizeof(WM_Obj));
    if (pObj == NULL) {
        return NULL;
    }
    pObj->CaptionHeight = WINDOW_DEF_CAPHEIGHT;  /* 标题栏高度 */
    /* 设置用户区 */
    pObj->Widget.Win.UserRect.x0 = pObj->Widget.Win.Rect.x0 + 3;
    pObj->Widget.Win.UserRect.y0 = pObj->Widget.Win.Rect.y0 +
                                   pObj->CaptionHeight + 2;
    pObj->Widget.Win.UserRect.x1 = pObj->Widget.Win.Rect.x1 - 3;
    pObj->Widget.Win.UserRect.y1 = pObj->Widget.Win.Rect.y1 - 3;
    /* 配色 */
    pObj->Widget.Skin.CaptionColor[0] = WINDOW_CAPTION_UPC;  /* 标题栏上半部分 */
    pObj->Widget.Skin.CaptionColor[1] = WINDOW_CAPTION_DOWNC;/* 标题栏下半部分 */
    pObj->Widget.Skin.EdgeColor[0] = STD_WIN_RIM_OUTC;       /* 外线 */
    pObj->Widget.Skin.EdgeColor[1] = STD_WIN_RIM_MIDC;       /* 中线 */
    pObj->Widget.Skin.EdgeColor[2] = STD_WIN_RIM_INC;        /* 内线 */
    pObj->Widget.Skin.BackColor[0] = WINDOW_BODY_BKC;        /* 底色 */
    pObj->Widget.Skin.FontColor[0] = WINDOW_TITLE_COLOR;
    pObj->Widget.Skin.FontColor[1] = WINDOW_FONT_COLOR;
    pObj->UserCb = cb;
    WINDOW_SetTitle(pObj, ""); /* 设置初始字符串 */
    WINDOW_SetFont(pObj, GUI_DEF_FONT);
    
    return pObj;
}

/* WINDOW设置标题 */
u_8 WINDOW_SetTitle(WM_hWin hWin, const char *str)
{
    /* 检测是否为WINDOW控件 */
    if (WM_CheckWindowSign(hWin, WIDGET_WINDOW)) {
        return 1;
    }
    ((WINDOW_Obj*)hWin)->Title = (char*)str;
    return 0;
}

/* WINDOW设置字体 */
u_8 WINDOW_SetFont(WM_hWin hWin, GUI_FontType Font)
{
    /* 检测是否为WINDOW控件 */
    if (WM_CheckWindowSign(hWin, WIDGET_WINDOW)) {
        return 1;
    }
    WIDGET_SetFont(hWin, Font);
    return 0;
}

/* WINDOW设置为透明窗口 */
void WINDOW_SetAllAlpha(WM_hWin hWin, u_8 Alpha)
{
    GUI_RECT Rect;
    
    WIDGET_Alpha(hWin, WIDGET_ALL, 0, Alpha);
    Rect = WM_GetWindowAreaRect(hWin);
    WM_InvalidateRect(hWin, &Rect); /* 将窗口无效化 */
}
