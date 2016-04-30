#include "TEXTBOX.h"
#include "GUI.h"

#define TEXTBOX_DEF_BKC             0x00292323  /* 背景颜色 */
#define TEXTBOX_TEXT_COLOR          0x00FFFFFF  /* 字体颜色 */

static void TEXTBOX_LineFeedDisp( const char *str, GUI_COLOR Color, GUI_FONT Font, GUI_RECT *Rect);

/* 自绘函数 */
static void __Paint(WM_HWIN hWin)
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
    GUI_FillRect(x0, y0, xSize, ySize, Color);
    /* 绘制文本内容 */
    Color = pObj->Widget.Skin.FontColor[0];
    TEXTBOX_LineFeedDisp(pObj->Text, Color,  WIDGET_GetFont(pObj), &Rect);
}

/* TEXTBOX控件自动回调函数 */
static void __Callback(WM_MESSAGE *pMsg)
{
    /* 检测是否为TEXTBOX控件 */
    WIDGET_SignErrorReturnVoid(pMsg->hWin, WIDGET_TEXTBOX);
    switch (pMsg->MsgId) {
        case WM_PAINT :
            __Paint(pMsg->hWin);
            break;
        case WM_DELETE:
            GUI_fastfree(((TEXTBOX_Obj*)pMsg->hWin)->Text);
            break;
        case WM_TP_CHECKED :
            WM_SetForegroundWindow(pMsg->hWin);
            break;
        case WM_TP_PRESS:
            break;
        case WM_TP_LEAVE:
            break;
        default:
            WM_DefaultProc(pMsg);
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
WM_HWIN TEXTBOX_Create(i_16 x0,
                      i_16 y0,
                      u_16 xSize,
                      u_16 ySize,
                      WM_HWIN hParent,
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
    /* 配色 */
    pObj->Widget.Skin.BackColor[0] = TEXTBOX_DEF_BKC;
    pObj->Widget.Skin.FontColor[0] = TEXTBOX_TEXT_COLOR;
    pObj->Text = NULL;
    TEXTBOX_SetFont(pObj, GUI_DEF_FONT);
    
    return pObj;
}

/* TEXTBOX设置内容 */
GUI_RESULT TEXTBOX_SetText(WM_HWIN hWin, const char *str)
{
    GUI_RECT Rect;

    /* 检测是否为TEXTBOX控件 */
    WIDGET_SignErrorReturn(hWin, WIDGET_TEXTBOX);
    GUI_fastfree(((TEXTBOX_Obj*)hWin)->Text);
    ((TEXTBOX_Obj*)hWin)->Text = GUI_fastmalloc(GUI_Strlen(str) + 1);
    GUI_Strcpy(((TEXTBOX_Obj*)hWin)->Text, str);
    Rect = WM_GetWindowAreaRect(hWin);
    hWin = WM_GetParentHandle(hWin);
    WM_InvalidateRect(hWin, &Rect);  /* 整个窗口失效 */
    return GUI_OK;
}

/* TEXTBOX设置字体 */
GUI_RESULT TEXTBOX_SetFont(WM_HWIN hWin, GUI_FONT Font)
{
    /* 检测是否为TEXTBOX控件 */
    WIDGET_SignErrorReturn(hWin, WIDGET_TEXTBOX);
    WIDGET_SetFont(hWin, Font);
    return GUI_OK;
}

/* TEXTBOX设置为透明窗口 */
GUI_RESULT TEXTBOX_SetAllAlpha(WM_HWIN hWin, u_8 Alpha)
{
    GUI_RECT Rect;
    
    /* 检测是否为TEXTBOX控件 */
    WIDGET_SignErrorReturn(hWin, WIDGET_TEXTBOX);
    WIDGET_Alpha(hWin, WIDGET_ALL, 0, Alpha);
    Rect = WM_GetWindowAreaRect(hWin);
    hWin = WM_GetParentHandle(hWin);
    WM_InvalidateRect(hWin, &Rect); /* 整个窗口失效 */
    return GUI_OK;
}

/* 自动换行显示 */
static void TEXTBOX_LineFeedDisp( const char *str, GUI_COLOR Color, GUI_FONT Font, GUI_RECT *Rect)
{
    u_16 pix, ch_num, x_pix;
    i_16 x0, x1, y0;
    
    x0 = Rect->x0;
    y0 = Rect->y0;
    x1 = Rect->x1;
    x_pix = x1 - x0 + 1;
    while (str && *str) {
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
