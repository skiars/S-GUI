#include "GUI_Color.h"

/* RGB888格式转换为RGB565格式 */
u_16 GUI_RGB888To565(u_32 RGB)
{
    u_16 R, G, B;
    R = (RGB >> 19) & 0x1F;
    G = (RGB >> 10) & 0x3F;
    B = (RGB >> 3) & 0x1F;
    return (R << 11) | (G << 5) | B;
}

/* RGB565格式转换为RGB888格式 */
u_32 GUI_RGB565To888(u_16 RGB)
{
    u_32 Color;
    Color = ((u_32)RGB & 0xF800) << 8;   /* R */
    Color |= ((u_32)RGB & 0x07E0) << 5;   /* G */
    Color |= ((u_32)RGB & 0x001F) << 3;   /* B */
    return Color;
}

u_16 GUI_ARGB8888To4444(u_32 ARGB)
{
    u_16 A, R, G, B;
    
    A = ARGB >> 28;
    R = (ARGB >> 20) & 0x0f;
    G = (ARGB >> 12) & 0x0f;
    B = (ARGB >> 4)  &  0x0f;
    return (A << 12) | (R << 8) |(G << 4) | B;
}

/*
* Alpha混合
* BkColor:背景颜色
* Color:目标颜色
* Alpha:透明度(0~256)
**/
GUI_COLOR GUI_AlphaBlend(GUI_COLOR Color, GUI_COLOR BkColor, u_16 Alpha)
{
    /* Calc Color seperations for FgColor first */
    GUI_COLOR R = (BkColor & 0xFF)    * Alpha;
    GUI_COLOR G = (BkColor & 0xFF00)  * Alpha;
    GUI_COLOR B = (BkColor & 0xFF0000)* Alpha;
    /* Add Color seperations for BkColor */
    Alpha = 256 - Alpha;
    R += (Color & 0xFF)     * Alpha;
    G += (Color & 0xFF00)   * Alpha;
    B += (Color & 0xFF0000) * Alpha;
    R = (R >> 8);
    G = (G >> 8) & 0xFF00;
    B = (B >> 8) & 0xFF0000;
    return R | G | B;
}
