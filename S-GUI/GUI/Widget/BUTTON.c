#include "BUTTON.h"
#include "GUI.h"

/* 按键控件自绘函数 */
static void _BUTTON_Paint(WM_HWIN hWin)
{
    i_16 x0, y0;
    u_16 xSize, ySize;
    GUI_COLOR Color, FontColor;
    GUI_RECT Rect;
    BUTTON_Obj *pObj = hWin;
    
    Rect = ((WM_Obj*)hWin)->Rect;
    x0 = Rect.x0;
    y0 = Rect.y0;
    xSize = Rect.x1 - x0 + 1;
    ySize = Rect.y1 - y0 + 1;
    Color = pObj->Widget.Skin.EdgeColor[0];
    /* 绘制边框 */
    GUI_DrawRect(x0, y0, xSize, ySize, Color);
    if (pObj->Check) {
        Color = pObj->Widget.Skin.BackColor[1];
        FontColor = pObj->Widget.Skin.FontColor[1];
    } else {
        Color = pObj->Widget.Skin.BackColor[0];
        FontColor = pObj->Widget.Skin.FontColor[0];
    }
    /* 绘制按键内部 */
    GUI_FillRect(x0 + 1, y0 + 1, xSize - 2, ySize - 2, Color);
    /* 绘制标题 */
    GUI_DspStringCurRectMiddle(x0 + 1, y0 + 1,
                               xSize - 2, ySize - 2,
                               pObj->Title, FontColor,
                               WIDGET_GetFont(pObj));
}

static void _BUTTON_Callback(WM_MESSAGE *pMsg)
{
    switch (pMsg->MsgId) {
        case WM_PAINT :
            _BUTTON_Paint(pMsg->hWin);
            break;
        case WM_TP_CHECKED :
            BUTTON_Check(pMsg->hWin, 1);
            WM_SetActiveMainWindow(pMsg->hWin); /* 设置祖先为活动窗口 */
            pMsg->MsgId = WM_BUTTON_CLICKED;
            WM_SendMessageToParent(pMsg->hWin, pMsg);
            break;
        case WM_TP_REMOVED :
            BUTTON_Check(pMsg->hWin, 0);
            pMsg->MsgId = WM_BUTTON_RELEASED;
            WM_SendMessageToParent(pMsg->hWin, pMsg);
            break;
        case WM_TP_LEAVE   :
            pMsg->MsgId = WM_NUTTON_MOVED_OUT;
            WM_SendMessageToParent(pMsg->hWin, pMsg);
            break;
        default:
            WM_DefaultProc(pMsg);
    }
}

/* 创建按键控件 */
WM_HWIN BUTTON_Create(i_16 x0,
                      i_16 y0,
                      u_16 xSize,
                      u_16 ySize,
                      WM_HWIN hParent,
                      u_16 Id,
                      u_8 Flag)
{
    BUTTON_Obj *pObj;
    
    pObj = WM_CreateWindowAsChild(x0, y0, xSize, ySize,
        hParent, Flag, WIDGET_BUTTON, Id, _BUTTON_Callback,
        sizeof(BUTTON_Obj) - sizeof(WM_Obj));
    if (pObj == NULL) {
        return NULL;
    }
    /* 配色 */
    pObj->Widget.Skin.EdgeColor[0] = 0x00FFFFFF;  /* 边线未按下 */
    pObj->Widget.Skin.EdgeColor[1] = 0x00FFFFFF;  /* 边线按下 */
    pObj->Widget.Skin.BackColor[0] = 0X0099FF66;
    pObj->Widget.Skin.BackColor[1] = 0X00C67171;
    pObj->Widget.Skin.FontColor[0] = 0X00007236;
    pObj->Widget.Skin.FontColor[1] = 0X00FFFFFF;
    pObj->Check = 0;                /* 没有按下 */
    BUTTON_SetTitle(pObj, "");      /* 设置初始字符串 */
    BUTTON_SetFont(pObj, GUI_DEF_FONT);
    
    return pObj;
}

GUI_RESULT BUTTON_SetTitle(WM_HWIN hWin, const char *str)
{
    /* 检测是否为BUTTON控件 */
    WIDGET_SignErrorReturn(hWin, WIDGET_BUTTON);
    ((BUTTON_Obj*)hWin)->Title = (char*)str;
    return GUI_OK;
}

GUI_RESULT BUTTON_SetFont(WM_HWIN hWin, GUI_FontType Font)
{
    /* 检测是否为BUTTON控件 */
    WIDGET_SignErrorReturn(hWin, WIDGET_BUTTON);
    WIDGET_SetFont(hWin, Font);
    return GUI_OK;
}

/* 按键按下API */
GUI_RESULT BUTTON_Check(WM_HWIN hWin, u_8 NewStatus)
{
    BUTTON_Obj *pObj = hWin;
    
    /* 检测是否为BUTTON控件 */
    WIDGET_SignErrorReturn(hWin, WIDGET_BUTTON);
    pObj->Check = NewStatus;
    WM_Invalidate(hWin);
    return GUI_OK;
}
