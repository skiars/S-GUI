#include "GUI_Device.h"
#include "GUI.h"

GUI_DEVICE GUI_LCD;

/* 在设备上画水平线 */
static void _DrawHLine(i_16 x0, i_16 y0, u_16 len, GUI_COLOR Color)
{
    i_16 x1 = x0 + len - 1;

    CHECK_Y(y0);
    CLIP_X(x0, x1);
    len = x1 >= x0 ? x1 - x0 + 1 : 0;
    while (len--) {
        GL_SetPixel(x0 + len, y0, Color);
    }
}

/* 在设备上填充区域 */
static void _FillRect(i_16 x0, i_16 y0, i_16 x1, i_16 y1, GUI_COLOR Color)
{

}

/* LCD初始化 */
void GUI_DeviceInit(void)
{
    GUI_LCD.xSize = 0;
    GUI_LCD.ySize = 0;
    GUI_LCD.DrawHLine = _DrawHLine;
    GUI_LCD.FillRect = _FillRect;
    GUI_Phy_Init(&GUI_LCD); /* 执行用户的初始化函数 */
}
