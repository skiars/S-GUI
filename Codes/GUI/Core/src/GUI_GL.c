/*****************************************************************************
 * S-GUI 2D图形库
 *****************************************************************************/

#include "GUI_GL.h"
#include "GUI.h"
#include "GUI_Math.h"
#include "GUI_GL_AA.h"
#include <math.h>

/* 全透明直接返回 */
#define RETURN_TRANSPARENT() \
    if (GUI_Context.FGColor >> 24 == 0xff) return;

#define FG_COLOR (GUI_Context.FGColor)
#define BG_COLOR (GUI_Context.BGColor)

static void _ClientToScreen(i_16 *x, i_16 *y)
{
    int xPos = GUI_Context.WinPos.x;
    int yPos = GUI_Context.WinPos.y;

    if (GUI_Context.AAEnable) {
        int factor = GUI_Context.AAFactor;

        xPos *= factor;
        yPos *= factor;
    }
	*x += xPos;
	*y += yPos;
}

static void _ClientToScreenRect(GUI_RECT *pRect)
{
    int xPos = GUI_Context.WinPos.x;
    int yPos = GUI_Context.WinPos.y;

    if (GUI_Context.AAEnable) {
        int factor = GUI_Context.AAFactor;

        xPos *= factor;
        yPos *= factor;
    }
	pRect->x0 += xPos;
	pRect->y0 += yPos;
	pRect->x1 += xPos;
	pRect->y1 += yPos;
}

/* 画直径为1的点 */
static void _DrawPoint(i_16 x, i_16 y)
{
    GUI_RECT r1;

    RETURN_TRANSPARENT();
    _ClientToScreen(&x, &y); /* 转换到屏幕坐标系 */
    GUI_Val2Rect(&r1, x, y, 1, 1);
    GUI_DrawAreaInit(&r1);
    while (GUI_GetNextArea()) { /* 遍历所有的显示区域 */
        GL_SetPixelClip(x, y);
    }
}

/* 画点 */
void GUI_DrawPoint(i_16 x,i_16 y)
{
    if (GUI_Context.PenSize > 1) {
        GUI_FillCircle(x, y, GUI_Context.PenSize / 2);
    } else {
        _DrawPoint(x, y);
    }
}

static void _VertLine(i_16 x0, i_16 y0, u_16 len)
{
    GUI_RECT r;

    RETURN_TRANSPARENT();
    _ClientToScreen(&x0, &y0); /* 转换到屏幕坐标系 */
    if (GUI_Val2Rect(&r, x0, y0, 1, len)) {
        GUI_DrawAreaInit(&r);
        while (GUI_GetNextArea()) { /* 遍历所有的显示区域 */
            GL_DrawVLine(x0, y0, r.y1);
        }
    }
}

/* 画垂直线
 * x0,y0:坐标
 * len:线长度
 * color:颜色
 **/
void GUI_VertLine(i_16 x0,i_16 y0,u_16 len)
{
    int w = GUI_Context.PenSize;
    
    if (w > 1) {
        GUI_FillRect(x0, y0, w, len);
    } else {
        _VertLine(x0, y0, len);
    }
}

static void _HoriLine(i_16 x0, i_16 y0, u_16 len)
{
    GUI_RECT r;

    RETURN_TRANSPARENT();
    _ClientToScreen(&x0, &y0); /* 转换到屏幕坐标系 */
    if (GUI_Val2Rect(&r, x0, y0, len, 1)) {
        GUI_DrawAreaInit(&r);
        while (GUI_GetNextArea()) { /* 遍历所有的显示区域 */
            GL_DrawHLine(x0, y0, r.x1);
        }
    }
}

/* 画水平线
 * x0,y0:坐标
 * len:线长度
 * color:颜色
 **/
void GUI_HoriLine(i_16 x0, i_16 y0, u_16 len)
{
    int w = GUI_Context.PenSize;

    if (w > 1) {
        GUI_FillRect(x0, y0, len, w);
    } else {
        _HoriLine(x0, y0, len);
    }
}

static void _DrawRect(i_16 x0, i_16 y0, i_16 x1, i_16 y1)
{
    GL_DrawHLine(x0, y0, x1);
    GL_DrawHLine(x0, y1, x1);
    GL_DrawVLine(x0, y0 + 1, y1 - 1);
    GL_DrawVLine(x1, y0 + 1, y1 - 1);
}

/* 画矩形框 */
void GUI_DrawRect(i_16 x0, i_16 y0, u_16 xSize, u_16 ySize)
{
    GUI_RECT r;

    _ClientToScreen(&x0, &y0); /* 转换到屏幕坐标系 */
    if (GUI_Val2Rect(&r, x0, y0, xSize, ySize)) {
        GUI_DrawAreaInit(&r);
        while (GUI_GetNextArea()) {
            _DrawRect(x0, y0, r.x1, r.y1);
        }
    }
}

/* 填充矩形 */
void GUI_FillRect(i_16 x0, i_16 y0, u_16 xSize, u_16 ySize)
{
    GUI_RECT r;
    
    RETURN_TRANSPARENT();
    _ClientToScreen(&x0, &y0); /* 转换到屏幕坐标系 */
    /* 将矩形坐标转换为结构体 */
    if (GUI_Val2Rect(&r, x0, y0, xSize, ySize)) {
        GUI_DrawAreaInit(&r);
        while (GUI_GetNextArea()) { /* 遍历所有的显示区域 */
            GL_FillRect(x0, y0, r.x1, r.y1);
        }
    }
}

/* 画任意直线, 文件内部调用 */
static void _DrawLine(i_16 x0, i_16 y0, i_16 x1, i_16 y1)
{
    int dx, dy, p, diff, addx, addy, i;

    if (x0 < x1) {
        dx = x1 - x0;
        addx = 1;
    } else {
        dx = x0 - x1;
        addx = -1;
    }
    if (y0 < y1) {
        dy = y1 - y0;
        addy = 1;
    } else {
        dy = y0 - y1;
        addy = -1;
    }
    if (dx >= dy) {
        dy <<= 1;
        p = dy - dx;
        diff = p - dx;
        for (i = 0; i <= dx; ++i) {
            GL_SetPixelClip(x0, y0);
            x0 += addx;
            if (p < 0) {
                p += dy;
            } else {
                y0 += addy;
                p += diff;
            }
        }
    } else {
        dx <<= 1;
        p = dx - dy;
        diff = p - dy;
        for (i = 0; i <= dy; ++i) {
            GL_SetPixelClip(x0, y0);
            y0 += addy;
            if (p < 0) {
                p += dx;
            } else {
                x0 += addx;
                p += diff;
            }
        }
    }
}

const i_16 aTan[] = {
	0,       /* atan(0/16) */
	41,      /* atan(1/16) */
	81,      /* atan(2/16) */
	121,     /* atan(3/16) */
	160,     /* atan(4/16) */
	197,     /* atan(5/16) */
	234,     /* atan(6/16) */
	269,     /* atan(7/16) */
	302,     /* atan(8/16) */
	334,     /* atan(9/16) */
	364,     /* atan(10/16) */
	393,     /* atan(11/16) */
	419,     /* atan(12/16) */
	445,     /* atan(13/16) */
	469,     /* atan(14/16) */
	491,     /* atan(15/16) */
	512      /* atan(1) = 45?= 512/1024 */
};

static int _atan0_45(int q)
{
	int r;
	int i, Faktor;
	/* Now angle is reduced to 0?<= <= 90?==>  0 <= <= 256*/
	q >>= 2;    /* make sure we do not exceed 16 bits in calculations */
	i = q >> 4;
	Faktor = (1 << 4) - (q&((1 << 4) - 1));
	r = aTan[i] * Faktor;
	if (Faktor != (1 << 4)) {
		r += aTan[i + 1] * ((1 << 4) - Faktor);
	}
	r = (r + (1 << 3)) / (1 << 4);   /* divide  incl. rounding */
	return r;
}

static int _atan2(i_32 x, i_32 y)
{
	u_8 q = 0;
	int angle;
	/* first make sure we are in angle between 0 and 45?*/
	if (x<0) {
		q = 1;
		x = -x;
	}
	if (y<0) {
		q |= (1 << 1);
		y = -y;
	}
	if (y>x) {
		int t = y;
		y = x;
		x = t;
		q |= (1 << 2);
	}
	y <<= 10;
	y += (x / 2);
	y /= x;
	angle = _atan0_45(y);
	if (q&(1 << 2)) { /* y/x reverse ? */
		angle = GUI_90DEG - angle;
	}
	if (q & 1) {  /* xreverse ? */
		angle = GUI_180DEG - angle;
	}
	if (q&(1 << 1)) { /* y-reverse ? */
		angle = GUI_360DEG - angle;
	}
	return angle;
}

static void _CalcOrto(int xDiff, int yDiff, i_32 r, int *px, int *py)
{
	i_32 x, y;
	int Angle = _atan2(xDiff, yDiff);
	Angle += GUI_90DEG;
	x = (r*(i_32)GUI_cos(Angle));
	y = (r*(i_32)GUI_sin(Angle));
	x = (x<0) ? -((-x + 512) >> 10) : ((x + 512) >> 10);
	y = (y<0) ? -((-y + 512) >> 10) : ((y + 512) >> 10);
	*px = x;
	*py = y;
}

/* 画任意直线, 线宽大于1 */
static void _DrawLineW(i_16 x0, i_16 y0, i_16 x1, i_16 y1)
{
	GUI_POINT Poly[4];
	int w = GUI_Context.PenSize;
	int xOff, yOff;
	int xOffP, yOffP, xOffM, yOffM;

	_CalcOrto(x0 - x1, y0 - y1, w - 1, &xOff, &yOff);
	/* Do rounding for offsets */
	if (xOff >0) {
		xOffP = (xOff + 1) / 2;
		xOffM = xOff / 2;
	} else {
		xOffP = xOff / 2;
		xOffM = (xOff - 1) / 2;
	}
	if (yOff >0) {
		yOffP = (yOff + 1) / 2;
		yOffM = yOff / 2;
	} else {
		yOffP = yOff / 2;
		yOffM = (yOff - 1) / 2;
	}
	Poly[0].x = x0 + xOffP;
	Poly[0].y = y0 + yOffP;
	Poly[1].x = x0 - xOffM;
	Poly[1].y = y0 - yOffM;
	Poly[2].x = x1 - xOffM;
	Poly[2].y = y1 - yOffM;
	Poly[3].x = x1 + xOffP;
	Poly[3].y = y1 + yOffP;
	GUI_FillPolygon(Poly, 4);
}

/* 画任意直线, 公共代码 */
void GUI_DrawLine(i_16 x0, i_16 y0, i_16 x1, i_16 y1)
{
    int w = GUI_Context.PenSize;
    GUI_RECT r;

	if (w > 1) {
		_DrawLineW(x0, y0, x1, y1);
		return;
	}
    r.x0 = x0 < x1 ? x0 : x1;
    r.y0 = y0 < y1 ? y0 : y1;
    r.x1 = x0 > x1 ? x0 : x1;
    r.y1 = y0 > y1 ? y0 : y1;
    if (x0 == x1) { /* 竖直线 */
        GUI_VertLine(r.x0, r.y0, r.y1 - r.y0 + 1);
        return;
    }
    if (y0 == y1) { /* 水平线 */
        GUI_HoriLine(r.x0, r.y0, r.x1 - r.x0 + 1);
        return;
    }
	_ClientToScreenRect(&r);
    _ClientToScreen(&x0, &y0);
    _ClientToScreen(&x1, &y1);
    GUI_DrawAreaInit(&r);
    while (GUI_GetNextArea()) {
        _DrawLine(x0, y0, x1, y1);
    }
}

/* 画8点 */
static void _Draw8Point(int x0, int y0, int x, int y)
{
    
    GL_SetPixelClip(x0 + x, y0 - y);
    GL_SetPixelClip(x0 - y, y0 - x);
    GL_SetPixelClip(x0 - x, y0 + y);
    GL_SetPixelClip(x0 + y, y0 + x);
    if (x && x != y) {
        GL_SetPixelClip(x0 - x, y0 - y);
        GL_SetPixelClip(x0 - y, y0 + x);
        GL_SetPixelClip(x0 + x, y0 + y);
        GL_SetPixelClip(x0 + y, y0 - x);
    }
}

static void _DrawCircle(int x0, int y0, int r)
{
    int x, y = r, d = -(r >> 1);

    for (x = 0; x <= y; ++x) {
        if (d < 0) {
            d += x;
        } else {
            d += x - y;
            --y;
        }
        _Draw8Point(x0, y0, x, y);
    }
}

/* 画8点, 画线宽不为1的圆时调用 */
static void _Draw8PointW(int x0, int y0, int x, int y, int y1)
{
    if (y1 - x < 0) {
        y1 = x;
    }
    GL_DrawVLine(x0 + x, y0 - y, y0 - y1);
    GL_DrawHLine(x0 - y, y0 - x, x0 - y1);
    GL_DrawVLine(x0 - x, y0 + y1, y0 + y);
    GL_DrawHLine(x0 + y1, y0 + x, x0 + y);
    if (x) {
        if (y1 == x) {
            y1 = x + 1;
        }
        GL_DrawVLine(x0 - x, y0 - y, y0 - y1);
        GL_DrawHLine(x0 - y, y0 + x, x0 - y1);
        GL_DrawVLine(x0 + x, y0 + y1, y0 + y);
        GL_DrawHLine(x0 + y1, y0 - x, x0 + y);
    }
}

/* 画线宽不为1的圆 */
static void _DrawCircleW(int x0, int y0, int r)
{
    int x, y = r, d = -(r >> 1);
    int r1 = r - GUI_Context.PenSize + 1;
    int y1 = r1, d1 = -(r1 >> 1);

    for (x = 0; x <= y; ++x) {
        if (d1 < 0) {
            d1 += x;
        } else {
            d1 += x - y1;
            --y1;
        }
        if (d < 0) {
            d += x;
        } else {
            d += x - y;
            --y;
        }
        _Draw8PointW(x0, y0, x, y, y1);
    }
}

/* 画圆 */
void GUI_DrawCircle(i_16 x0, i_16 y0, u_16 r)
{
    int w = GUI_Context.PenSize;
    GUI_RECT Rect;
    void (*circle)(int, int, int);

    if (w > 1) {
        w >>= 1;
        if (r <= w) {
            GUI_FillCircle(x0, y0, r);
            return;
        }
        circle = _DrawCircleW;
        r += w;
    } else {
        circle = _DrawCircle;
    }
    _ClientToScreen(&x0, &y0); /* 转换到屏幕坐标系 */
    Rect.x0 = x0 - r;
    Rect.x1 = x0 + r;
    Rect.y0 = y0 - r;
    Rect.y1 = y0 + r;
    GUI_DrawAreaInit(&Rect);
    while (GUI_GetNextArea()) { /* 遍历所有的显示区域 */
        circle(x0, y0, r);
    }
}

/* 填充圆 */
static void _FillCircle(int x0, int y0, int r)
{
    i_32 i;
    int imax = (r * 707) / 1000 + 1;
    i_32 sqmax = r * r + r / 2;
    int x = r;

    GL_DrawHLine(x0 - r, y0, x0 + r);
    for (i = 1; i <= imax; ++i) {
        if (i * i + x * x > sqmax) {
            /* draw lines from outside */
            if (x > imax) {
                GL_DrawHLine(x0 - i + 1, y0 + x, x0 + i - 1);
                GL_DrawHLine(x0 - i + 1, y0 - x, x0 + i - 1);
            }
            --x;
        }
        /* draw lines from inside (center) */
        GL_DrawHLine(x0 - x, y0 + i, x0 + x);
        GL_DrawHLine(x0 - x, y0 - i, x0 + x);
    }
}

/* 填充圆 */
void GUI_FillCircle(i_16 x0, i_16 y0, i_16 r)
{
    GUI_RECT Rect;

    _ClientToScreen(&x0, &y0); /* 转换到屏幕坐标系 */
    Rect.x0 = x0 - r;
    Rect.x1 = x0 + r;
    Rect.y0 = y0 - r;
    Rect.y1 = y0 + r;
    GUI_DrawAreaInit(&Rect);
    while (GUI_GetNextArea()) { /* 遍历所有的显示区域 */
        _FillCircle(x0, y0, r);
    }
}

/* 获取多边形的边界范围 */
void GUI_GetPolyArea(GUI_RECT *r, GUI_POINT *Points, int cnt)
{
    i_16 t;
    int i;

    r->x0 = Points->x;
    r->x1 = r->x0;
    r->y0 = Points->y;
    r->y1 = r->y0;
    for (i = 0; i < cnt; ++i) {
        t = Points[i].x;
        if (t > r->x1) {
            r->x1 = t;
        }
        if (t < r->x0) {
            r->x0 = t;
        }
        t = Points[i].y;
        if (t > r->y1) {
            r->y1 = t;
        }
        if (t < r->y0) {
            r->y0 = t;
        }
    }
}

/* 绘制多边形, 内部调用 */
static void _DrawPolygon(GUI_POINT *Points, int cnt)
{
    int i;
    i_16 xPos = GUI_Context.WinPos.x;
    i_16 yPos = GUI_Context.WinPos.y;
    GUI_POINT *p1, *p2;

    if (GUI_Context.AAEnable) {
        int factor = GUI_Context.AAFactor;

        xPos *= factor;
        yPos *= factor;
    }
    for (i = 0; i < cnt - 1; ++i) {
        p1 = Points + i;
        p2 = Points + i + 1;
        _DrawLine(p1->x + xPos, p1->y + yPos, p2->x + xPos, p2->y + yPos);
    }
    p1 = Points + i;
    p2 = Points;
    _DrawLine(p1->x + xPos, p1->y + yPos, p2->x + xPos, p2->y + yPos);
}

/* 绘制多边形 */
void GUI_DrawPolygon(GUI_POINT *Points, int cnt)
{
    GUI_RECT r;

    GUI_GetPolyArea(&r, Points, cnt);
    _ClientToScreenRect(&r);
    GUI_DrawAreaInit(&r);
    while (GUI_GetNextArea()) { /* 遍历所有的显示区域 */
        _DrawPolygon(Points, cnt);
    }
}

/* 填充多边形, 内部调用 */
static void _FillPolygon(GUI_POINT *Points, int cnt)
{
    i_16 xPos = GUI_Context.WinPos.x;
    i_16 yPos = GUI_Context.WinPos.y;
    int yMax = Points->y, yMin = yMax;
    int y, i, j, nodes;
    static int nodeX[GUI_MAX_POLY_CORNERS];
    GUI_POINT *pi, *pj;

    if (GUI_Context.AAEnable) {
        int factor = GUI_Context.AAFactor;

        xPos *= factor;
        yPos *= factor;
    }
    /* 找出最上面的行和最下面的行 */
    for (i = 0; i < cnt; ++i) {
        y = Points[i].y;
        if (y > yMax) {
            yMax = y;
        }
        if (y < yMin) {
            yMin = y;
        }
    }
    for (y = yMin; y <= yMax; ++y) {
        /* 计算多变形与扫描线的交点 */
        nodes = 0;
        j = cnt - 1;
        for (i = 0; i < cnt; ++i) {
            pi = Points + i; /* 边的起点 */
            pj = Points + j; /* 边的终点 */
            if (pi->y == y && pj->y == y) { /* 水平边直接填充 */
                if (pi->x < pj->x) {
                    GL_DrawHLine(pi->x + xPos, y + yPos, pj->x + xPos);
                } else {
                    GL_DrawHLine(pj->x + xPos, y + yPos, pi->x + xPos);
                }
            } else if (pi->y < y && pj->y >= y || pj->y < y && pi->y >= y) {
                nodeX[nodes++] = pi->x + ((y - pi->y)
                    * (pj->x - pi->x)) / (pj->y - pi->y);
            }
            j = i;
        }
        /* 交点从左到右排序 */
        i = 0;
        while (i < nodes - 1) {
            if (nodeX[i] > nodeX[i + 1]) {
                j = nodeX[i];
                nodeX[i] = nodeX[i + 1];
                nodeX[i + 1] = j;
                if (i) {
                    --i;
                }
            } else {
                ++i;
            }
        }
        /* 填充线绘制 */
        for (i = 0; i < nodes; i += 2) {
            GL_DrawHLine(nodeX[i] + xPos, y + yPos, nodeX[i + 1] + xPos);
        }
    }
}

/* 填充多变形 */
void GUI_FillPolygon(GUI_POINT *Points, int cnt)
{
    GUI_RECT r;

    if (cnt > GUI_MAX_POLY_CORNERS) {
        return;
    }
    GUI_GetPolyArea(&r, Points, cnt);
    _ClientToScreenRect(&r);
    GUI_DrawAreaInit(&r);
    while (GUI_GetNextArea()) { /* 遍历所有的显示区域 */
        _FillPolygon(Points, cnt);
    }
}

/* 绘制折线, 内部调用 */
static void _DrawLines(i_16 x, i_16 y, GUI_POINT *Points, int cnt)
{
    int i;
    i_16 xPos = GUI_Context.WinPos.x;
    i_16 yPos = GUI_Context.WinPos.y;
    GUI_POINT *p1, *p2;
    void(*draw)(i_16, i_16, i_16, i_16);

    if (GUI_Context.AAEnable) {
        int factor = GUI_Context.AAFactor;

        xPos *= factor;
        yPos *= factor;
    }
    xPos += x;
    yPos += y;
    if (GUI_Context.PenSize > 1) {
        draw = _DrawLineW;
    } else {
        draw = _DrawLine;
    }
    for (i = 0; i < cnt - 1; ++i) {
        p1 = Points + i;
        p2 = Points + i + 1;
        draw(p1->x + xPos, p1->y + yPos, p2->x + xPos, p2->y + yPos);
    }
}

/* 绘制折线 */
void GUI_DrawLines(i_16 x, i_16 y, GUI_POINT *Points, int cnt)
{
    GUI_RECT r;

    GUI_GetPolyArea(&r, Points, cnt);
    GUI_MoveRect(&r, x, y);
    _ClientToScreenRect(&r);
    GUI_DrawAreaInit(&r);
    while (GUI_GetNextArea()) { /* 遍历所有的显示区域 */
        _DrawLines(x, y, Points, cnt);
    }
}

static void _waveTest(void)
{
    int i;
    static GUI_POINT Points[300];

    for (i = 0; i < 300; ++i) {
        Points[i].x = i * 4;
        Points[i].y = (int)sinf(i / 3.0f) * 50 * 4;
    }
    GUI_Context.AAFactor = 4;
    GUI_SetPenSize(10);
    GUI_DrawLines((i_16)100, (i_16)200, Points, 300);
}

void GUI_2DTest(float angle)
{
	void GUI_2DTestAA(void);
    int i, nodes;
    float s = 1.0;
    GUI_POINT Point[7];
    GUI_POINT points[] = {
        { 100, 10 },
        { 140, 60 },
        { 115, 50 },
        { 115, 100 },
        { 85, 100 },
        { 85, 50 },
        { 60, 60 }
    };

    nodes = GUI_COUNTOF(points);
    /* 旋转图像 */
    for (i = 0; i < nodes; ++i) {
        Point[i].x = (int)((points[i].x - 100.0f * s) * cosf(angle)
			- (points[i].y - 100.0f * s) * sinf(angle) + 100.0f * s);
        Point[i].y = (int)((points[i].x - 100.0f * s) * sinf(angle)
			+ (points[i].y - 100.0f * s) * cosf(angle) + 100.0f * s);
    }
    GUI_SetFGColor(0x00803050);
	//GUI_Context.AAFactor = 4;
    GUI_FillPolygon(Point, nodes);
    GUI_SetFGColor(0x50000000);
    GUI_DrawPolygon(Point, nodes);
	//GUI_2DTestAA();
    //_waveTest();
}
