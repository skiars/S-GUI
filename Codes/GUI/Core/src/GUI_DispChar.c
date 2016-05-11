#include "GUI_DispChar.h"
#include "GUI.h"
#include "string.h"
#include "GUI_Font.h"

/* 获取字符信息 */
static GUI_CHARINFO * _GetCharInfo(const u_16 Char)
{
    GUI_FONT_PROP *pProp = GUI_Context.Font->pProp;

    while (pProp->pNext) {
        if (Char >= pProp->FirstChar && Char <= pProp->LastChar) {
            return &pProp->pCharInfo[Char];
        }
        pProp = pProp->pNext;
    }
    return NULL;
}

/* 获取当前字体字符的高度 */
u_16 GUI_GetCharHeight(void)
{
    return GUI_Context.Font->CharHeight;
}

/* 获取当前字体字符的宽度 */
u_16 GUI_GetCharWidth(const u_16 Char)
{
    GUI_CHARINFO *pInfo = _GetCharInfo(Char);

    if (pInfo) {
        return pInfo->XSize;
    }
    return 0;
}

u_16 GUI_DispChar(i_16 x, i_16 y, const char *ch, GUI_COLOR Color, GUI_FONT Font)
{
    return 0;
}

/* 在当前的显示区域下显示一串字符串 */
void GUI_DispStringCurRect(i_16 x0,
                           i_16 y0,
                           const char *str,
                           GUI_COLOR Color,
                           GUI_FONT *Font)
{
    
}

/* 在当前的显示区域下显示一串字符串,显示N个字符 */
void GUI_DispStringCurRectN(i_16 x0,
                           i_16 y0,
                           const char *str,
                           GUI_COLOR Color,
                           GUI_FONT *Font,
                           u_16 n)
{
    
}

/* 在当前的显示区域下的指定区域居中显示字符串 */
void GUI_DspStringCurRectMiddle(i_16 x0,
                                i_16 y0,
                                i_16 xSize,
                                i_16 ySize,
                                const char *str,
                                GUI_COLOR Color,
                                GUI_FONT *Font)
{
    
}

/* 在指定的区域显示一串字符串 */
void GUI_DispStringAt(i_16 x0,
                      i_16 y0,
                      u_16 xSize,
                      u_16 ySize,
                      const char *str,
                      GUI_COLOR Color,
                      GUI_FONT *Font)
{

}
