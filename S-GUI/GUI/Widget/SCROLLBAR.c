#include "SCROLLBAR.h"
#include "GUI.h"

/* 按键控件自绘函数 */
static void __Paint(WM_hWin hWin)
{
    i_16 x0, y0;
    u_16 xSize, ySize, ScrLen;
    GUI_COLOR Color;
    GUI_RECT Rect;
    SCROLLBAR_Obj *pObj = hWin;

    Rect = WM_GetTaliorInvalidRect(hWin);  //获取需要显示的区域
    Rect = ((WM_Obj*)hWin)->Rect;
    x0 = Rect.x0;
    y0 = Rect.y0;
    xSize = Rect.x1 - x0 + 1;
    ySize = Rect.y1 - y0 + 1;
    
    /* 绘制背景 */
    Color = pObj->Widget.Skin.BackColor[0];
    GUI_FillTailorRect(x0, y0, xSize, ySize, Color);
    /* 绘制滑块 */
    ScrLen = ySize / pObj->Totality;
    if(ScrLen < pObj->MinThick)
    {
        ScrLen = pObj->MinThick;
    }
    y0 += pObj->Loation * (ySize - ScrLen) / (pObj->Totality - 1);
    Color = pObj->Widget.Skin.BackColor[1];
    GUI_FillTailorRect(x0, y0, xSize, ScrLen, Color);
}

static void __Callback(WM_MESSAGE *pMsg)
{
    /* 检测是否为SCROLLBAR控件 */
    if (WM_CheckWindowSign(pMsg->hWin, WIDGET_SCROLLBAR)) {
        return;
    }
    switch (pMsg->MsgId) {
        case WM_PAINT :
            __Paint(pMsg->hWin);
            break;
        case WM_DELETE :
            GUI_fastfree(pMsg->hWin);
            break;
    }
}

/* 创建按键控件 */
WM_hWin SCROLLBAR_Create(i_16 x0,
                      i_16 y0,
                      u_16 xSize,
                      u_16 ySize,
                      WM_hWin hParent,
                      u_16 Id,
                      u_8 Flag)
{
    SCROLLBAR_Obj *pObj;
    
    pObj = WM_CreateWindowAsChild(x0, y0, xSize, ySize, hParent, Flag,
                                  WIDGET_SCROLLBAR, Id, __Callback,
                                  sizeof(SCROLLBAR_Obj) - sizeof(WM_Obj));
    if (pObj == NULL) {
        return NULL;
    }
    /* 设置用户区 */
    pObj->Widget.Win.UserRect.x0 = pObj->Widget.Win.Rect.x0;
    pObj->Widget.Win.UserRect.y0 = pObj->Widget.Win.Rect.y0;
    pObj->Widget.Win.UserRect.x1 = pObj->Widget.Win.Rect.x0;
    pObj->Widget.Win.UserRect.y1 = pObj->Widget.Win.Rect.y0;
    /* 配色 */
    pObj->Widget.Skin.BackColor[0] = SCLB_DFT_BKCOLOR;
    pObj->Widget.Skin.BackColor[1] = SCLB_DFT_BTNCOLOR;
    pObj->MinThick = SCLB_MIN_THICK;
    SCROLLBAR_SetTotality(pObj, 100);
    SCROLLBAR_SetLoation(pObj, 0);
    return pObj;
}

void SCROLLBAR_SetTotality(GUI_hWin hWin, u_16 Totality)
{
    SCROLLBAR_Obj *pObj = hWin;
    
    /* 检测是否为SCROLLBAR控件 */
    if (WM_CheckWindowSign(hWin, WIDGET_SCROLLBAR)) {
        return;
    }
    pObj->Totality = Totality;
    WM_Invalidate(hWin);
}

void SCROLLBAR_SetLoation(GUI_hWin hWin, u_16 Loation)
{
    SCROLLBAR_Obj *pObj = hWin;
    
    /* 检测是否为SCROLLBAR控件 */
    if (WM_CheckWindowSign(hWin, WIDGET_SCROLLBAR)) {
        return;
    }
    pObj->Loation = Loation;
    WM_Invalidate(hWin);
}
