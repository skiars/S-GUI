#include "WIDGET.h"
#include "GUI.h"

/* 设置窗口的字体 */
void WIDGET_SetFont(GUI_HWIN hWin, GUI_FONT *Font)
{
    if (hWin != NULL) {
        ((WIDGET*)hWin)->Skin.Font = Font;
    }
}

/* 获取窗口的字体 */
GUI_FONT *WIDGET_GetFont(GUI_HWIN hWin)
{
    return ((WIDGET*)hWin)->Skin.Font;
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
