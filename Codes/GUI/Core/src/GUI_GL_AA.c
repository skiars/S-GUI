/*****************************************************************************
* S-GUI抗锯齿图形库.
*****************************************************************************/
#include "GUI_GL_AA.h"
#include "GUI.h"
#include <math.h>

#define GUI_AA_LINEBUFFER_SIZE 640

static u_8 _lineBuffer[GUI_AA_LINEBUFFER_SIZE];
static int _x0, _x1, _y, _xStart, _xEnd;

static void(*_setPixel)(i_16, i_16);
static void(*_drawHLine)(i_16, i_16, i_16);
static void(*_drawVLine)(i_16, i_16, i_16);
static void(*_fillRect)(i_16, i_16, i_16, i_16);


/*
@ 按亮度级别输出像素
@ light: 0-255, 越大亮度越大
*/
void _lightPixel(i_16 x, i_16 y, u_8 light)
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
                GUI_GDev.DrawHLine(_x0 + i, _y, _x0 + j,
                    GUI_Context.FGColor);
                i = j;
            } else {
                /* 绘制单个像素 */
                GUI_GDev.SetPixel(_x0 + i, _y, GUI_Context.FGColor);
            }
        } else {
            _lightPixel(_x0 + i, _y, intens);
        }
    }
    _CleanLine();
}

/* 绘制水平线 */
static void _DrawHLine(i_16 x0, i_16 y, i_16 x1)
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

static void _SetPixel(i_16 x, i_16 y)
{
    CHECK_X(x);
    CHECK_Y(y);
    _DrawHLine(x, y, x);
}

static void _DrawVLine(i_16 x, i_16 y0, i_16 y1)
{
    CLIP_Y(y0, y1);
    CHECK_X(x);
    while (y0 <= y1) {
        _SetPixel(x, y0++);
    }
}

static void _FillRect(i_16 x0, i_16 y0, i_16 x1, i_16 y1)
{
    while (y0 <= y1) {
        _DrawHLine(x0, y0++, x1);
    }
}

/*  */
void GUI_AA_Init(i_16 x0, i_16 x1)
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
    _setPixel = GUI_glAPI.SetPixel;
    GUI_glAPI.SetPixel = _SetPixel;
    _drawHLine = GUI_glAPI.DrawHLine;
    GUI_glAPI.DrawHLine = _DrawHLine;
    _drawVLine = GUI_glAPI.DrawVLine;
    GUI_glAPI.DrawVLine = _DrawVLine;
    _fillRect = GUI_glAPI.FillRect;
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
    GUI_glAPI.DrawHLine = _drawHLine;
    GUI_glAPI.DrawVLine = _drawVLine;
    GUI_glAPI.SetPixel = _setPixel;
    GUI_glAPI.FillRect = _fillRect;
}

void GUI_DrawLineAA(i_16 x0, i_16 y0, i_16 x1, i_16 y1)
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

void GUI_DrawLinesAA(i_16 x, i_16 y, GUI_POINT *Points, int cnt)
{
    GUI_RECT r;

    GUI_GetPolyArea(&r, Points, cnt);
    GUI_MoveRect(&r, x, y);
    GUI_AA_Init(r.x0, r.x1);
    GUI_DrawLines(x, y, Points, cnt);
    GUI_AA_Exit();
}

#define M_PI 3.14159265358979323846f

void GUI_2DTestAA(void)
{
    int x0, y0, x1, y1;
    static float angle = 0.0f, a;

    GUI_SetPenSize(2);
    GUI_Context.AAFactor = 8;
    GUI_SetFGColor(0x00000000);
    //for (angle = 0.0; angle <= 360.0; angle += 5.0)
    {
        a = angle * M_PI / 180.0f;
        /* 旋转图像 */
        x0 = (int)(((210.0f - 200.0f) * cosf(a) + 200.0f));
        y0 = (int)(((210.0f - 200.0f) * sinf(a) + 160.0f));
        x1 = (int)(((350.0f - 200.0f) * cosf(a) + 200.0f));
        y1 = (int)(((350.0f - 200.0f) * sinf(a) + 160.0f));
        GUI_DrawLine(x0, y0, x1, y1);
    }
    angle += 0.3f;
    //GUI_DrawLineAA(-50 * 4, 200 * 4, 190 * 4, 220 * 4);
}
