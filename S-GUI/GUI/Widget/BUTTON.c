#include "BUTTON.h"
#include "GUI.h"

/* 按键控件自绘函数 */
static void _BUTTON_Paint(WM_hWin hWin)
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
    GUI_DrawTailorRect(x0, y0, xSize, ySize, Color);
    if (pObj->Check) {
        Color = pObj->Widget.Skin.BackColor[1];
        FontColor = pObj->Widget.Skin.FontColor[1];
    } else {
        Color = pObj->Widget.Skin.BackColor[0];
        FontColor = pObj->Widget.Skin.FontColor[0];
    }
    /* 绘制按键内部 */
    GUI_FillTailorRect(x0 + 1, y0 + 1, xSize - 2, ySize - 2, Color);
    /* 绘制标题 */
    GUI_DspStringCurRectMiddle(x0 + 1, y0 + 1,
                               xSize - 2, ySize - 2,
                               pObj->Title, FontColor,
                               WIDGET_GetFont(pObj));
}

static void _BUTTON_Callback(WM_MESSAGE *pMsg)
{
    /* 检测是否为BUTTON控件 */
    if (WM_CheckWindowSign(pMsg->hWin, WIDGET_BUTTON)) {
        return;
    }
    switch (pMsg->MsgId) {
        case WM_PAINT :
            _BUTTON_Paint(pMsg->hWin);
            break;
        case WM_DELETE :
            GUI_fastfree(pMsg->hWin);
            break;
        case WM_TP_CHECKED :
            BUTTON_Check(pMsg->hWin, 1);
            WM_SetActiveMainWindow(pMsg->hWin);
            WM_PostMessageToParent(pMsg->hWin, WM_BUTTON_CLICKED, NULL);
            break;
        case WM_TP_REMOVED :
            BUTTON_Check(pMsg->hWin, 0);
            WM_PostMessageToParent(pMsg->hWin, WM_BUTTON_RELEASED, NULL);
            break;
        case WM_TP_LEAVE   :
            WM_PostMessageToParent(pMsg->hWin, WM_NUTTON_MOVED_OUT, NULL);
            break;
    }
}

/* 创建按键控件 */
WM_hWin BUTTON_Create(i_16 x0,
                      i_16 y0,
                      u_16 xSize,
                      u_16 ySize,
                      WM_hWin hParent,
                      u_16 Id,
                      u_8 Flag)
{
    BUTTON_Obj *pObj;
    
    pObj = WM_CreateWindowAsChild(x0, y0, xSize, ySize, hParent, Flag,
                                  WIDGET_BUTTON, Id, _BUTTON_Callback,
                                  sizeof(BUTTON_Obj) - sizeof(WM_Obj));
    if (pObj == NULL) {
        return NULL;
    }
    /* 设置用户区 */
    pObj->Widget.Win.UserRect.x0 = pObj->Widget.Win.Rect.x0 + 1;
    pObj->Widget.Win.UserRect.y0 = pObj->Widget.Win.Rect.y0 + 1;
    pObj->Widget.Win.UserRect.x1 = pObj->Widget.Win.Rect.x1 - 1;
    pObj->Widget.Win.UserRect.y1 = pObj->Widget.Win.Rect.y1 - 1;
    /* 配色 */
    pObj->Widget.Skin.EdgeColor[0] = 0x00FFFFFF;  //边线未按下
    pObj->Widget.Skin.EdgeColor[1] = 0x00FFFFFF;  //边线按下
    pObj->Widget.Skin.BackColor[0] = 0X0066FF99;
    pObj->Widget.Skin.BackColor[1] = 0X007171C6;
    pObj->Widget.Skin.FontColor[0] = 0X00367200;
    pObj->Widget.Skin.FontColor[1] = 0X00FFFFFF;
    pObj->Check = 0;                //没有按下
    BUTTON_SetTitle(pObj, "");      //设置初始字符串
    BUTTON_SetFont(pObj, GUI_DEF_FONT);
    
    return pObj;
}

u_8 BUTTON_SetTitle(WM_hWin hWin, const char *str)
{
    /* 检测是否为BUTTON控件 */
    if (WM_CheckWindowSign(hWin, WIDGET_BUTTON)) {
        return 1;
    }
    ((BUTTON_Obj*)hWin)->Title = (char*)str;
    return 0;
}

u_8 BUTTON_SetFont(WM_hWin hWin, GUI_FontType Font)
{
    /* 检测是否为BUTTON控件 */
    if (WM_CheckWindowSign(hWin, WIDGET_BUTTON)) {
        return 1;
    }
    WIDGET_SetFont(hWin, Font);
    return 0;
}

/* 按键按下API */
void BUTTON_Check(WM_hWin hWin, u_8 NewStatus)
{
    BUTTON_Obj *pObj = hWin;
    
    /* 检测是否为BUTTON控件 */
    if (WM_CheckWindowSign(hWin, WIDGET_BUTTON)) {
        return;
    }
    pObj->Check = NewStatus;
    WM_Invalidate(hWin);
}

void BUTTON_Test(void)
{
#if  0
    WM_MESSAGE Msg;
    Msg.hWin = BUTTON_Create(20,20,50,20,NULL,0,0);
    Msg.MsgId = WM_PAINT;
    _BUTTON_Callback(&Msg);
#endif
}
