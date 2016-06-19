#include "GUI_Device.h"
#include "GUI.h"

GUI_DEVICE GUI_GDev;

/* 默认画点函数 */
static void _SetPixel(u_16 x, u_16 y, GUI_COLOR Color)
{

}

/* 默认读取像素函数 */
static GUI_COLOR _GetPixel(u_16 x, u_16 y)
{
    return 0;
}

/* 在设备上画水平线 */
static void _DrawHLine(i_16 x0, i_16 y0, i_16 x1, GUI_COLOR Color)
{
    while (x0 <= x1) {
        GL_SetPixel(x0++, y0, Color);
    }
}

/* 填充透明矩形 */
static void __FillRectAlpha(i_16 x0, i_16 y0, u_16 x1, u_16 y1, GUI_COLOR Color)
{
    i_16 x, y;
    u_16 Alpha = Color >> 24;
    GUI_COLOR tColor;

    for (y = y0; y <= y1; y++) {
        for (x = x0; x <= x1; x++) {
            tColor = GUI_AlphaBlend(Color, GL_GetPixel(x, y), Alpha);
            GL_SetPixel(x, y, tColor);
        }
    }
}

/* 填充非透明矩形 */
static void __FillRect(i_16 x0, i_16 y0, u_16 x1, u_16 y1, GUI_COLOR Color)
{
    i_16 y;

    for (y = y0; y <= y1; y++) {
        GUI_GDev.DrawHLine(x0, y, x1, Color);
    }
}

/* 在设备上填充区域 */
static void _FillRect(i_16 x0, i_16 y0, i_16 x1, i_16 y1, GUI_COLOR Color)
{
    if (Color >> 24) {
        __FillRectAlpha(x0, y0, x1, y1, Color);
    } else {
        __FillRect(x0, y0, x1, y1, Color);
    }
}

/* 绘制位图 */
static void __DrawBitmap(u_8 cFormat, const unsigned char *pData,
        i_16 x0, i_16 y0, u_16 xSize, u_16 ySize, u_32 Offset)
{
    int i, j, pixBytes;
    
    if (cFormat == GUI_RGB565) {
        pixBytes = 2;
    } else if (cFormat == GUI_RGB888) {
        pixBytes = 3;
    }
    Offset *= pixBytes;
    for (j = 0; j < ySize; ++j) {
        for (i = 0; i < xSize; ++i) {
            GL_SetPixel(x0 + i, y0 + j, GUI_RGB565To888(*((u_16 *)pData)));
            pData += pixBytes;
        }
        pData += Offset;
    }
}

/* LCD初始化 */
void GUI_DeviceInit(void)
{
    GUI_GDev.xSize = 0;
    GUI_GDev.ySize = 0;
    GUI_GDev.SetPixel = _SetPixel;
    GUI_GDev.GetPixel = _GetPixel;
    GUI_GDev.DrawHLine = _DrawHLine;
    GUI_GDev.FillRect = _FillRect;
    GUI_GDev.DrawBitmap = __DrawBitmap;
    GUI_UserConfig(&GUI_GDev); /* 执行用户的初始化函数 */
}

/* 绘制水平线 */
void GL_DrawHLine(i_16 x0, i_16 y0, i_16 x1, GUI_COLOR Color)
{
    CLIP_X(x0, x1);
    CHECK_Y(y0);
    if (x0 <= x1) {
        GUI_GDev.DrawHLine(x0, y0, x1, Color);
    }
}
