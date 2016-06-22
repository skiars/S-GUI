#include "WIDGET.h"
#include "GUI.h"

/* 设置窗口的透明度 */
void WIDGET_Alpha(GUI_HWIN hWin, u_8 Part, u_8 Id, u_8 Alpha)
{
    u_8 i;
    WIDGET_SKIN *pSkin;
    GUI_COLOR A = (GUI_COLOR)Alpha << 24;

    if (hWin == NULL) {
        return;
    }
    GUI_LOCK();
    WM_SetTransWindow(hWin, 1);
    pSkin = &((WIDGET*)hWin)->Skin;
    switch (Part) {
        case WIDGET_EDGE :
            pSkin->EdgeColor[Id] &= 0x00FFFFFF;
            pSkin->EdgeColor[Id] |= A;
            break;
        case WIDGET_BACK :
            pSkin->BackColor[Id] &= 0x00FFFFFF;
            pSkin->BackColor[Id] = A;
            break;
        case WIDGET_CAPTION :
            pSkin->CaptionColor[Id] &= 0x00FFFFFF;
            pSkin->CaptionColor[Id] = A;
            break;
        case WIDGET_FONT :
            pSkin->FontColor[Id] &= 0x00FFFFFF;
            pSkin->FontColor[Id] = A;
            break;
        case WIDGET_ALL :  /* 字体不会自动透明 */
            for (i = 0; i < 3; i ++) {
                pSkin->EdgeColor[i] &= 0x00FFFFFF;
                pSkin->EdgeColor[i] |= A;
            }
            for (i = 0; i < 2; i ++) {
                pSkin->BackColor[i] &= 0x00FFFFFF;
                pSkin->BackColor[i] |= A;
            }
            for (i = 0; i < 2; i ++) {
                pSkin->CaptionColor[i] &= 0x00FFFFFF;
                pSkin->CaptionColor[i] |= A;
            }
            break;
    }
    GUI_UNLOCK();
}

/* 获取窗口的字体 */
GUI_FONT *WIDGET_GetFont(GUI_HWIN hWin)
{
    return ((WIDGET*)hWin)->Skin.Font;
}

/* 设置窗口的字体 */
void WIDGET_SetFont(GUI_HWIN hWin, GUI_FONT *Font)
{
    if (hWin != NULL) {
        ((WIDGET*)hWin)->Skin.Font = Font;
    }
}

/* 设置背景色 */
GUI_COLOR WIDGET_GetBackColor(GUI_HWIN hWin, u_8 n)
{
    WIDGET *pObj = hWin;
    return pObj->Skin.BackColor[n];
}

/* 设置边线颜色 */
GUI_COLOR WIDGET_GetEdgeColor(GUI_HWIN hWin, u_8 n)
{
    WIDGET *pObj = hWin;
    return pObj->Skin.EdgeColor[n];
}

/* 获取标题栏颜色 */
GUI_COLOR WIDGET_GetCaptionColor(GUI_HWIN hWin, u_8 n)
{
    WIDGET *pObj = hWin;
    return pObj->Skin.CaptionColor[n];
}

/* 获取字体颜色 */
GUI_COLOR WIDGET_GetFontColor(GUI_HWIN hWin, u_8 n)
{
    WIDGET *pObj = hWin;
    return pObj->Skin.FontColor[n];
}

GUI_RESULT WIDGET_SetPaintFunction(GUI_HWIN hWin, WIDGET_PAINT *Paint)
{
    if (Paint) {
        ((WIDGET *)hWin)->Skin.Paint = Paint;
        return GUI_OK;
    }
    return GUI_ERR;
}

void WIDGET_Paint(GUI_HWIN hWin)
{
    ((WIDGET *)hWin)->Skin.Paint(hWin);
}
