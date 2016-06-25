#include "GUI_Device.h"
#include "GUI.h"

GUI_DRIVER  GUI_GDev;       /* 当前使用的驱动 */
GUI_GDEV    GUI_Screen;     /* 屏幕设备 */
GUI_GDEV   *GUI_CurDevice;  /* 当前的图形设备 */
GUI_DRIVER *GUI_CurDriver;  /* 当前的图形设备的驱动库 */
GUI_GLAPI   GUI_glAPI;      /* 基本的绘图函数 */

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
    if (Color >> 24) {
        u_16 Alpha = Color >> 24;
        GUI_COLOR tColor;

        while (x0 <= x1) {
            tColor = GUI_AlphaBlend(Color, GL_GetPixel(x0, y0), Alpha);
            GL_SetPixel(x0, y0, tColor);
            ++x0;
        }
    } else {
        while (x0 <= x1) {
            GL_SetPixel(x0++, y0, Color);
        }
    }
}

/* 在设备上画垂直线 */
static void _DrawVLine(i_16 x0, i_16 y0, i_16 y1, GUI_COLOR Color)
{
    if (Color >> 24) {
        u_16 Alpha = Color >> 24;
        GUI_COLOR tColor;

        while (y0 <= y1) {
            tColor = GUI_AlphaBlend(Color, GL_GetPixel(x0, y0), Alpha);
            GL_SetPixel(x0, y0, tColor);
            ++y0;
        }
    } else {
        while (y0 <= y1) {
            GL_SetPixel(x0, y0++, Color);
        }
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


/* 裁剪像素 */
static void _SetPixelClip(i_16 x, i_16 y)
{
	GUI_COLOR Color = GUI_Context.FGColor;

	CHECK_X(x);
	CHECK_Y(y);
	if (Color >> 24) {
		Color = GUI_AlphaBlend(Color, GL_GetPixel(x, y), Color >> 24);
	}
	GL_SetPixel(x, y, Color);
}

/* 绘制水平线 */
static void _glDrawHLine(i_16 x0, i_16 y0, i_16 x1)
{
	CLIP_X(x0, x1);
	CHECK_Y(y0);
	if (x0 <= x1) {
		GUI_GDev.DrawHLine(x0, y0, x1, GUI_Context.FGColor);
	}
}

/* 绘制垂直线 */
static void _glDrawVLine(i_16 x0, i_16 y0, i_16 y1)
{
	CLIP_Y(y0, y1);
	CHECK_X(x0);
	if (y0 <= y1) {
		GUI_GDev.DrawVLine(x0, y0, y1, GUI_Context.FGColor);
	}
}

static void _glFillRect(i_16 x0, i_16 y0, i_16 x1, i_16 y1)
{
	CLIP_X(x0, x1);
	CLIP_Y(y0, y1);
	GUI_GDev.FillRect(x0, y0, x1, y1, GUI_Context.FGColor);
}

/* LCD初始化 */
void GUI_DeviceInit(void)
{
	GUI_glAPI.SetPixel = _SetPixelClip;
	GUI_glAPI.DrawHLine = _glDrawHLine;
	GUI_glAPI.DrawVLine = _glDrawVLine;
	GUI_glAPI.FillRect =  _glFillRect;
    GUI_GDev.xSize = 0;
    GUI_GDev.ySize = 0;
    GUI_GDev.SetPixel = _SetPixel;
    GUI_GDev.GetPixel = _GetPixel;
    GUI_GDev.DrawHLine = _DrawHLine;
    GUI_GDev.DrawVLine = _DrawVLine;
    GUI_GDev.FillRect = _FillRect;
    GUI_GDev.DrawBitmap = __DrawBitmap;
    GUI_UserConfig(&GUI_GDev); /* 执行用户的初始化函数 */
}

/* 内存映射型图形设备的显示偏移位置设置 */
void _SetPosition(u_16 x, u_16 y)
{
    GUI_CurDevice->Position = (void *)((int)GUI_CurDevice->FrameBuffer
        + ((int)y * (int)GUI_CurDevice->Width + (int)x)
        * GUI_CurDevice->BytesPerPixel);
}

/* 在当前位置下写入像素 */
void _WriteCurPosition(GUI_COLOR ColorIndex)
{
    switch (GUI_CurDevice->BytesPerPixel) {
    case 1: /* 8bpp */
        *(u_8 *)GUI_CurDevice->Position = (u_8)ColorIndex;
        break;
    case 2: /* 16bpp */
        *(u_16 *)GUI_CurDevice->Position = (u_16)ColorIndex;
        break;
    case 3: /* 24bpp */
        *(u_8 *)GUI_CurDevice->Position
            = (u_8)((ColorIndex >> 16) & 0xFF);
        *(u_8 *)((u_32)GUI_CurDevice->Position + 1)
            = (u_8)((ColorIndex >> 8) & 0xFF);
        *(u_8 *)((u_32)GUI_CurDevice->Position + 2)
            = (u_8)(ColorIndex & 0xFF);
        break;
    case 4: /* 32bpp */
        *(u_32 *)GUI_CurDevice->Position = (u_32)ColorIndex;
        break;
    }
    GUI_CurDevice->Position = (void *)((u_32)GUI_CurDevice->Position
        + GUI_CurDevice->BytesPerPixel);
}

/* 显示像素偏移偏移 */
void _DispOffset(int OffsetPixels)
{
    GUI_CurDevice->Position = (void *)((u_32)GUI_CurDevice->Position
        + OffsetPixels * GUI_CurDevice->BytesPerPixel);
}

/* 设置帧缓冲地址 */
void GUI_SetFrameBuffer(GUI_GDEV *gDevice, void *Ptr)
{
    gDevice->FrameBuffer = Ptr;
}

/* 设置像素颜色格式 */
GUI_RESULT GUI_SetPixelFormat(GUI_GDEV *gDevice, int PixelFormat)
{
    if (PixelFormat == GUI_ARGB8888) {
        gDevice->BytesPerPixel = 4;
    } else if (PixelFormat == GUI_RGB888) {
        gDevice->BytesPerPixel = 3;
    } else if (PixelFormat == GUI_RGB565) {
        gDevice->BytesPerPixel = 2;
    } else if (PixelFormat == GUI_L8) {
        gDevice->BytesPerPixel = 1;
    } else {
        return GUI_ERR;
    }
    gDevice->PixelFormat = PixelFormat;
    return GUI_OK;
}
