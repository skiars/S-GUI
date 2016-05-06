#include "SCROLLBAR.h"
#include "GUI.h"

/* 滚动条尺寸设定 */
#define SCLB_MIN_THICK         20          /* 最小的厚度(即高度) */
#define SCLB_DFT_BTNCOLOR      0X00121314  /* 滑块颜色 */
#define SCLB_DFT_BKCOLOR       0X002E323D  /* 背景颜色 */

/* 按键控件自绘函数 */
static void __Paint(WM_HWIN hWin)
{
    i_16 x0, y0;
    u_16 xSize, ySize, ScrLen;
    GUI_COLOR Color;
    GUI_RECT Rect;
    SCROLLBAR_Obj *pObj = hWin;

    Rect = WM_GetTaliorInvalidRect(hWin); /* 获取需要显示的区域 */
    Rect = ((WM_Obj*)hWin)->Rect;
    x0 = Rect.x0;
    y0 = Rect.y0;
    xSize = Rect.x1 - x0 + 1;
    ySize = Rect.y1 - y0 + 1;
    
    /* 绘制背景 */
    Color = pObj->Widget.Skin.BackColor[0];
    GUI_FillRect(x0, y0, xSize, ySize, Color);
    /* 绘制滑块 */
    ScrLen = ySize / pObj->Totality;
    if(ScrLen < pObj->MinThick)
    {
        ScrLen = pObj->MinThick;
    }
    y0 += pObj->Loation * (ySize - ScrLen) / (pObj->Totality - 1);
    Color = pObj->Widget.Skin.BackColor[1];
    GUI_FillRect(x0, y0, xSize, ScrLen, Color);
}

static void __Callback(WM_MESSAGE *pMsg)
{
    switch (pMsg->MsgId) {
        case WM_PAINT :
            __Paint(pMsg->hWin);
            break;
    }
}

/* 创建SCROLLBAR控件 */
WM_HWIN SCROLLBAR_Create(i_16 x0,
    i_16 y0,
    u_16 xSize,
    u_16 ySize,
    WM_HWIN hParent,
    u_16 Id,
    u_8 Style)
{
    SCROLLBAR_Obj *pObj;
    
    pObj = WM_CreateWindowAsChild(x0, y0, xSize, ySize, hParent,
        Style, Id, __Callback, sizeof(SCROLLBAR_Obj) - sizeof(WM_Obj));
    if (pObj == NULL) {
        return NULL;
    }
    /* 配色 */
    pObj->Widget.Skin.BackColor[0] = SCLB_DFT_BKCOLOR;
    pObj->Widget.Skin.BackColor[1] = SCLB_DFT_BTNCOLOR;
    pObj->MinThick = SCLB_MIN_THICK;
    SCROLLBAR_SetTotality(pObj, 100);
    SCROLLBAR_SetLoation(pObj, 0);
    return pObj;
}

GUI_RESULT SCROLLBAR_SetTotality(GUI_HWIN hWin, u_16 Totality)
{
    SCROLLBAR_Obj *pObj = hWin;
    
    pObj->Totality = Totality;
    WM_Invalidate(hWin);
    return GUI_OK;
}

GUI_RESULT SCROLLBAR_SetLoation(GUI_HWIN hWin, u_16 Loation)
{
    SCROLLBAR_Obj *pObj = hWin;

    pObj->Loation = Loation;
    WM_Invalidate(hWin);
    return GUI_OK;
}
