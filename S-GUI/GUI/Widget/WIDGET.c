#include "WIDGET.h"
#include "GUI.h"

/* 将一个窗口的属性设置为透明 */
void WIDGET_SetTransWindow(GUI_HWIN hWin)
{
    if (hWin) {
        ((WM_Obj*)hWin)->Status |= WM_WS_TRANS;
    }
}

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
    WIDGET_SetTransWindow(hWin);
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
GUI_FontType WIDGET_GetFont(GUI_HWIN hWin)
{
    return ((WIDGET*)hWin)->Skin.Font;
}

/* 设置窗口的字体 */
void WIDGET_SetFont(GUI_HWIN hWin, GUI_FontType Font)
{
    if (hWin != NULL) {
        ((WIDGET*)hWin)->Skin.Font = Font;
    }
}

GUI_COLOR WIDGET_GetBackColor(GUI_HWIN hWin, u_8 n)
{
    WIDGET *pObj = hWin;
    return pObj->Skin.BackColor[n];
}

GUI_COLOR WIDGET_GetEdgeColor(GUI_HWIN hWin, u_8 n)
{
    WIDGET *pObj = hWin;
    return pObj->Skin.EdgeColor[n];
}

GUI_COLOR WIDGET_GetCaptionColor(GUI_HWIN hWin, u_8 n)
{
    WIDGET *pObj = hWin;
    return pObj->Skin.CaptionColor[n];
}

GUI_COLOR WIDGET_GetFontColor(GUI_HWIN hWin, u_8 n)
{
    WIDGET *pObj = hWin;
    return pObj->Skin.FontColor[n];
}
