/*****************************************************************************
* S-GUI抗锯齿图形库.
*****************************************************************************/
#include "GUI_GL_AA.h"
#include "GUI.h"
#include <math.h>

#define GUI_AA_LINEBUFFER_SIZE 640

static u_8 _lineBuffer[GUI_AA_LINEBUFFER_SIZE];
static int _x0, _x1, _y, _xStart, _xEnd;
static GUI_GLAPI _glAPI;

static void _glDrawHLine(int x0, int y0, int x1, GUI_COLOR Color)
{
    if (x0 <= x1) {
        GUI_GDev->DrawHLine(x0, y0, x1, Color);
    }
}

/*
@ 按亮度级别输出像素
@ light: 0-255, 越大亮度越大
*/
void _lightPixel(int x, int y, u_8 light)
{
    GL_SetPixel(x, y, GUI_AlphaBlend(
        GL_GetPixel(x, y), GUI_Context.FGColor,
        (int)light * 255 / GUI_Context.AAFactor / GUI_Context.AAFactor));
}

static void _CleanLine(void)
{
    GUI_Memset(_lineBuffer, 0, _x1 - _x0 + 1);
    _y = -16383;  /* Invalidate */
    _xStart = 16383;
    _xEnd = -16383;
}

static void _flushLine(void)
{
    int i, j, iEnd = _xEnd - _x0;
    int iMax = GUI_Context.AAFactor * GUI_Context.AAFactor;
    int intens;

    for (i = _xStart - _x0; i < iEnd; ++i) {
        intens = *(_lineBuffer + i);
        if (intens >= iMax) {
            for (j = i; j < iEnd; ++j) {
                if (*(_lineBuffer + j + 1) != iMax) {
                    break;
                }
            }
            if (j != i) {
                /* 绘制水平线 */
                _glDrawHLine(_x0 + i, _y, _x0 + j, GUI_Context.FGColor);
                i = j;
            } else {
                /* 绘制单个像素 */
                GL_SetPixel(_x0 + i, _y, GUI_Context.FGColor);
            }
        } else {
            _lightPixel(_x0 + i, _y, (u_8)intens);
        }
    }
    _CleanLine();
}

/* 绘制水平线 */
static void _DrawHLine(int x0, int y, int x1)
{
    int factor = GUI_Context.AAFactor;
    int x0Real, x1Real, dx;
    u_8 *pl;

    CLIP_X(x0, x1);
    CHECK_Y(y);
    if (x1 < x0) {
        return;
    }
    x0Real = x0 / factor;
    x1Real = x1 / factor;
    /* Clip (should not be necessary ... Just to be on the safe side ! */
    if (x0Real < _x0) {
        x0 = _x0 * GUI_Context.AAFactor;
    }
    if (x1Real > _x1) {
        x1 = (_x1 + 1)*GUI_Context.AAFactor - 1;
    }
    dx = x1Real - x0Real + 1;
    if (y / factor != _y) {
        _flushLine(); /*  */
        _y = y / factor;
    }
    if (x0Real < _xStart) {
        _xStart = x0Real;
    }
    if (x1Real > _xEnd) {
        _xEnd = x1Real + 1;
    }
    /* 写入到缓冲中 */
    pl = _lineBuffer + x0Real - _x0;
    if (dx == 1) {
        *pl += x1 - x0 + 1;
    } else {
        /* 第一像素 */
        *pl++ += (x0Real + 1) * factor - x0;
        /* 中间像素 */
        while (--dx > 1) {
            *pl++ += factor;
        }
        /* 最后像素 */
        *pl += x1 - x1Real * factor;
    }
}

static void _SetPixel(int x, int y)
{
    CHECK_X(x);
    CHECK_Y(y);
    _DrawHLine(x, y, x);
}

static void _FillRect(int x0, int y0, int x1, int y1)
{
    while (y0 <= y1) {
        _DrawHLine(x0, y0++, x1);
    }
}

/*  */
void GUI_AA_Init(int x0, int x1)
{
    int xPos = GUI_Context.WinPos.x * GUI_Context.AAFactor;

    x0 += xPos;
    x1 += xPos;
    if (x0 < 0) {
        x0 = 0;
    }
    /* Is there anything to do at all ??? */
    if (x1 < x0) {
        x0 = x1;
    }
    GUI_Context.AAEnable = 1;
    _glAPI = GUI_glAPI;
    GUI_glAPI.SetPixelClip = _SetPixel;
    GUI_glAPI.DrawHLine = _DrawHLine;
    GUI_glAPI.FillRect = _FillRect;
    _x0 = x0 / GUI_Context.AAFactor - 1;
    _x1 = x1 / GUI_Context.AAFactor + 1;
    if (_x1 - _x0 > GUI_AA_LINEBUFFER_SIZE - 1) {
        _x1 = _x0 + GUI_AA_LINEBUFFER_SIZE - 1;
    }
    _CleanLine();
}

/*  */
void GUI_AA_Exit(void)
{
    GUI_Context.AAEnable = 0;
    _flushLine();
    GUI_glAPI = _glAPI;
}

void GUI_DrawLineAA(int x0, int y0, int x1, int y1)
{
    int w = GUI_Context.PenSize, xMin, xMax;

    if (x0 < x1) {
        xMin = x0;
        xMax = x1;
    } else {
        xMin = x1;
        xMax = x0;
    }
    xMin -= w;
    xMax += w;
    GUI_AA_Init(xMin, xMax);
    GUI_DrawLine(x0, y0, x1, y1);
    GUI_AA_Exit();
}

void GUI_FillPolygonAA(GUI_POINT *Points, int cnt)
{
    GUI_RECT r;

    GUI_GetPolyArea(&r, Points, cnt);
    GUI_AA_Init(r.x0, r.x1);
    GUI_FillPolygon(Points, cnt);
    GUI_AA_Exit();
}

void GUI_FillCircleAA(int x0, int y0, int r)
{
    GUI_AA_Init(x0 - r, x0 + r);
    GUI_FillCircle(x0, y0, r);
    GUI_AA_Exit();
}

void GUI_DrawLinesAA(int x, int y, GUI_POINT *Points, int cnt)
{
    GUI_RECT r;

    GUI_GetPolyArea(&r, Points, cnt);
    GUI_MoveRect(&r, x, y);
    GUI_AA_Init(r.x0, r.x1);
    GUI_DrawLines(x, y, Points, cnt);
    GUI_AA_Exit();
}
