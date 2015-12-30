#include "TEXTBOX.h"
#include "GUI.h"

void TEXTBOX_LineFeedDisp( const char *str, GUI_COLOR Color, GUI_FontType Font, GUI_RECT *Rect);

/* 自绘函数 */
static void __Paint(WM_hWin hWin)
{
    i_16 x0, y0;
    u_16 xSize, ySize;
    GUI_COLOR Color;
    GUI_RECT Rect;
    TEXTBOX_Obj *pObj = hWin;

    Rect = ((WM_Obj*)hWin)->Rect;
    x0 = Rect.x0;
    y0 = Rect.y0;
    xSize = Rect.x1 - x0 + 1;
    ySize = Rect.y1 - y0 + 1;
    /* 绘制内部背景 */
    Color = pObj->Widget.Skin.BackColor[0];
    GUI_FillTailorRect(x0, y0, xSize, ySize, Color);
    /* 绘制文本内容 */
    Color = pObj->Widget.Skin.FontColor[0];
    TEXTBOX_LineFeedDisp(pObj->Text, Color,  WIDGET_GetFont(pObj), &Rect);
}

/* WINDOW控件自动回调函数 */
static void __Callback(WM_MESSAGE *pMsg)
{
    /* 检测是否为WINDOW控件 */
    if (WM_CheckWindowSign(pMsg->hWin, WIDGET_TEXTBOX)) {
        return;
    }
    switch (pMsg->MsgId) {
        case WM_PAINT :
            __Paint(pMsg->hWin);
            break;
        case WM_DELETE :
            GUI_fastfree(pMsg->hWin);
            break;
        case WM_TP_CHECKED :
            WM_SetActiveMainWindow(pMsg->hWin);
            break;
        case WM_TP_PRESS:
            break;
        case WM_TP_LEAVE:
            break;
    }
}

/*
 * 创建文本框
 * x0:TEXTBOX控件的最左像素(相对于父窗口)
 * y0:TEXTBOX控件的最右像素(相对于父窗口)
 * xSize:TEXTBOX控件的水平宽度
 * ySize:TEXTBOX控件的竖直高度
 * hParent:父窗口句柄
 * Id:窗口ID
 * Flag:窗口状态
 * cb:用户回调历程指针
 **/
WM_hWin TEXTBOX_Create(i_16 x0,
                      i_16 y0,
                      u_16 xSize,
                      u_16 ySize,
                      WM_hWin hParent,
                      u_16 Id,
                      u_8 Flag)
{
    TEXTBOX_Obj *pObj;
    
    pObj = WM_CreateWindowAsChild(x0, y0, xSize, ySize, hParent, Flag,
                                  WIDGET_TEXTBOX, Id, __Callback,
                                  sizeof(TEXTBOX_Obj) - sizeof(WM_Obj));
    if (pObj == NULL) {
        return NULL;
    }
    /* 设置用户区 */
    pObj->Widget.Win.UserRect.x0 = pObj->Widget.Win.Rect.x0;
    pObj->Widget.Win.UserRect.y0 = pObj->Widget.Win.Rect.y0;
    pObj->Widget.Win.UserRect.x1 = pObj->Widget.Win.Rect.x1;
    pObj->Widget.Win.UserRect.y1 = pObj->Widget.Win.Rect.y1;
    /* 配色 */
    pObj->Widget.Skin.BackColor[0] = TEXTBOX_DEF_BKC;
    pObj->Widget.Skin.FontColor[0] = TEXTBOX_TEXT_COLOR;
    TEXTBOX_SetText(pObj, "");      /* 设置初始字符串 */
    TEXTBOX_SetFont(pObj, GUI_DEF_FONT);
    
    return pObj;
}

/* TEXTBOX设置标题 */
u_8 TEXTBOX_SetText(WM_hWin hWin, const char *str)
{
    GUI_RECT Rect;

    /* 检测是否为TEXTBOX控件 */
    if (WM_CheckWindowSign(hWin, WIDGET_TEXTBOX)) {
        return 1;
    }
    ((TEXTBOX_Obj*)hWin)->Text = (char*)str;
    Rect = WM_GetWindowAreaRect(hWin);
    hWin = WM_GetParentHandle(hWin);
    WM_InvalidateRect(hWin, &Rect);  /* 整个窗口失效 */
    return 0;
}

/* TEXTBOX设置字体 */
u_8 TEXTBOX_SetFont(WM_hWin hWin, GUI_FontType Font)
{
    /* 检测是否为TEXTBOX控件 */
    if (WM_CheckWindowSign(hWin, WIDGET_TEXTBOX)) {
        return 1;
    }
    WIDGET_SetFont(hWin, Font);
    return 0;
}

/* TEXTBOX设置为透明窗口 */
void TEXTBOX_SetAllAlpha(WM_hWin hWin, u_8 Alpha)
{
    GUI_RECT Rect;
    
    WIDGET_Alpha(hWin, WIDGET_ALL, 0, Alpha);
    Rect = WM_GetWindowAreaRect(hWin);
    hWin = WM_GetParentHandle(hWin);
    WM_InvalidateRect(hWin, &Rect); /* 整个窗口失效 */
}

/* 自动换行显示 */
void TEXTBOX_LineFeedDisp( const char *str, GUI_COLOR Color, GUI_FontType Font, GUI_RECT *Rect)
{
    u_16 pix, ch_num, x_pix;
    i_16 x0, x1, y0;
    
    x0 = Rect->x0;
    y0 = Rect->y0;
    x1 = Rect->x1;
    x_pix = x1 - x0 + 1;
    while (*str) {
        pix = GUI_SkipWord(str, Font, x_pix, &ch_num);
        if (x0 + pix > x1 || *str == '\n') {
            x0 = Rect->x0;
            y0 += Font->CharHeight;
        }
        if (*str == '\n') {
            str += ch_num;
            continue;
        }
        GUI_DispStringCurRectN(x0, y0 , str, Color,  Font, ch_num);
        str += ch_num;
        x0 += pix;
        
    }
}
