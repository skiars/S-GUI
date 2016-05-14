#include "GUI_DispChar.h"
#include "GUI.h"
#include "string.h"
#include "GUI_Font.h"

/* 获取字符信息 */
static GUI_CHARINFO * _GetCharInfo(const u_16 Char)
{
    GUI_FONT_PROP *pProp = GUI_Context.Font->pProp;

    while (pProp) {
        if (Char >= pProp->FirstChar && Char <= pProp->LastChar) {
            return &pProp->pCharInfo[Char - pProp->FirstChar];
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
u_16 GUI_GetCharWidth(u_16 Char)
{
    GUI_CHARINFO *pInfo = _GetCharInfo(Char);

    if (pInfo) {
        return pInfo->XSize;
    }
    return 0;
}

/* 获取当前字符串长度像素值, 会被'\r', '\n', '\0'截断 */
int GUI_GetStringWidth(const char *Str)
{
    int Width = 0;

    while (*Str && *Str != '\n' && *Str != '\r') {
        Width += GUI_GetCharWidth(*Str++);
    }
    return Width;
}

/* 字符显示透明度混合函数 */
static void _CharSetPixel(i_16 x, i_16 y, u_8 light, u_8 pp)
{
    GUI_COLOR FontColor = GUI_Context.FontColor;

    pp = (1 << pp) - 1;
    if (light == pp) { /* 不必计算透明度 */
        GL_SetPixel(x, y, FontColor);
    } else { /* 抗锯齿字体计算透明度 */
        GUI_COLOR BGColor = GL_GetPixel(x, y);
        GUI_COLOR R = ((FontColor >> 16) & 0xFF) * light / pp;
        GUI_COLOR G = ((FontColor >>  8) & 0xFF) * light / pp;
        GUI_COLOR B = (FontColor         & 0xFF) * light / pp;

        light = pp - light;
        R += ((BGColor >> 16) & 0xFF) * light / pp;
        G += ((BGColor >>  8) & 0xFF) * light / pp;
        B += (BGColor         & 0xFF) * light / pp;
        GL_SetPixel(x, y, R << 16 | G << 8 | B);
    }
}

/* 显示字符 */
static void _DispChar(i_16 x,    /* 显示位置 */
    i_16 y,                      /* 显示位置 */
    u_16 Width,                  /* 位图高度 */
    u_16 Height,                 /* 位图宽度 */
    const unsigned char *pData,  /* 位图数据 */
    u_8 PixBits,                 /* 每像素比特数 */
    u_16 BytesPerLine)           /* 每行的字节数 */
{
    unsigned char data, light, bitCnt;
    u_8 BytePixels = 8 / PixBits, x0Shift, shift = 8 - PixBits;
    i_16 x0 = x, y0 = y, x1 = x + Width - 1, y1 = y + Height - 1;
    u_16 xSize, ySize, i, j;
    const unsigned char *ptab;

    /* 裁剪至显示区域 */
    CLIP_X(x0, x1);
    CLIP_Y(y0, y1);
    xSize = x1 - x0 + 1;
    ySize = y1 - y0 + 1;
    pData += (y0 - y) * BytesPerLine + (x0 - x) / BytePixels;
    x0Shift = PixBits * ((x0 - x) & (BytePixels - 1));
    for (j = 0; j < ySize; ++j, pData += BytesPerLine) {
        ptab = pData;
        data = *ptab << x0Shift;
        bitCnt = 8 - x0Shift;
        for (i = x0; i <= x1; ++i) {
            light = data >> shift;
            data <<= PixBits;
            bitCnt -= PixBits;
            if (!bitCnt) {
                data = *(++ptab);
                bitCnt = 8;
            }
            if (light) { /* 显示 */
                _CharSetPixel(i, y0 + j, light, PixBits);
            }
        }
    }
}

/* 显示一个字符 */
u_16 GUI_DispChar(i_16 x, i_16 y, u_16 Char)
{
    u_8 PixBits = GUI_Context.Font->FontType;
    u_16 Width, Height = GUI_GetCharHeight();
    GUI_CHARINFO *cInfo = _GetCharInfo(Char);

    if (cInfo) {
        GUI_RECT r;
        u_8 BytesPerLine = cInfo->BytesPerLine;
        const unsigned char *pData = cInfo->pData;

        Width = cInfo->XSize;
        GUI_Val2Rect(&r, x, y, Width, Height);
        GUI_DrawAreaInit(&r);
        while (GUI_GetNextArea()) { /* 遍历所有的显示区域 */
            _DispChar(x, y, Width, Height, pData, PixBits, BytesPerLine);
        }
        return Width;
    }
    return 0; /* 字体中没有该字符 */
}

/* 在指定矩形区域内显示一个字符 */
u_16 GUI_DispCharInRect(GUI_RECT *pRect, u_16 Char)
{
    u_8 PixBits = GUI_Context.Font->FontType;
    u_16 Width, Height = GUI_GetCharHeight();
    GUI_CHARINFO *cInfo = _GetCharInfo(Char);

    if (cInfo) {
        GUI_RECT r;
        u_8 BytesPerLine = cInfo->BytesPerLine;
        const unsigned char *pData = cInfo->pData;

        Width = cInfo->XSize;
        GUI_Val2Rect(&r, pRect->x0, pRect->y0, Width, Height);
        if (GUI_RectOverlay(&r, pRect, &r)) {
            GUI_DrawAreaInit(&r);
            while (GUI_GetNextArea()) { /* 遍历所有的显示区域 */
                _DispChar(r.x0, r.y0, Width,
                    Height, pData, PixBits, BytesPerLine);
            }
            return Width;
        }
    }
    return 0; /* 字体中没有该字符或者不在显示范围内 */
}

/* 在当前的显示区域下显示一串字符串 */
void GUI_DispString(i_16 x0, i_16 y0, const char *Str)
{
    while (*Str) {
        x0 += GUI_DispChar(x0, y0, *Str++);
    }
}

/* 在指定矩形区域内显示一个字符串 */
void GUI_DispStringInRect(GUI_RECT *pRect, const char *Str, u_8 Align)
{
    GUI_RECT r;
    u_16 Height = GUI_GetCharHeight();
    int Width = GUI_GetStringWidth(Str);

    /* 计算水平对齐方式 */
    r.x1 = pRect->x1;
    switch (Align & (GUI_ALIGN_LEFT | GUI_ALIGN_RIGHT | GUI_ALIGN_HCENTER)) {
    case GUI_ALIGN_RIGHT: /* 右对齐 */
        r.x0 = pRect->x1 - Width;
        break;
    case GUI_ALIGN_HCENTER: /* 水平居中 */
        if (pRect->x1 - pRect->x0 + 1 > Width) {
            r.x0 = pRect->x0 + (pRect->x1 - pRect->x0 + 1 - Width) / 2;
            break;
        }
    default: /* 默认左对齐 */
        r.x0 = pRect->x0;
        break;
    }
    /* 计算垂直对齐方式 */
    r.y1 = pRect->y1;
    switch (Align & (GUI_ALIGN_TOP | GUI_ALIGN_BOTTOM | GUI_ALIGN_VCENTER)) {
    case GUI_ALIGN_BOTTOM: /* 低对齐 */
        r.y0 = pRect->y1 - Height;
        break;
    case GUI_ALIGN_VCENTER: /* 垂直居中 */
        if (pRect->y1 - pRect->y0 + 1 > Height) {
            r.y0 = pRect->y0 + (pRect->y1 - pRect->y0 + 1 - Height) / 2;
            break;
        }
    default: /* 默认顶对齐 */
        r.y0 = pRect->y0;
        break;
    }
    while (*Str && *Str != '\n' && *Str != '\r') {
        r.x0 += GUI_DispCharInRect(&r, *Str++);
    }
}
