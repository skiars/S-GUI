#include "GUI_Device.h"
#include "GUI.h"

GUI_GDEV        *GUI_GDev;       /* 当前使用的图形设备 */
GUI_GDEV         GUI_Screen;     /* 屏幕设备 */
GUI_GLAPI        GUI_glAPI;      /* 基本的绘图函数 */
static GUI_GDEV *GUI_GDevList;   /* 图形设备列表 */

#define HL_SetPixel GUI_GDev->SetPixel
#define HL_GetPixel GUI_GDev->GetPixel

/* 默认画点函数 */
static void _SetPixel(int x, int y, GUI_COLOR Color)
{

}

/* 默认读取像素函数 */
static GUI_COLOR _GetPixel(int x, int y)
{
    return 0;
}

/* 在设备上画水平线 */
static void _DrawHLine(int x0, int y0, int x1, GUI_COLOR Color)
{
    while (x0 <= x1) {
        HL_SetPixel(x0, y0, Color);
        ++x0;
    }
}

/* 在设备上画垂直线 */
static void _DrawVLine(int x0, int y0, int y1, GUI_COLOR Color)
{
    while (y0 <= y1) {
        HL_SetPixel(x0, y0, Color);
        ++y0;
    }
}

/* 在设备上填充区域 */
static void _FillRect(GUI_FLIPOUT *Cmd)
{
    int x0 = Cmd->x0, y0 = Cmd->y0, x1 = Cmd->x1, y1 = Cmd->y1;
    GUI_COLOR Color = Cmd->Color;

    while (y0 <= y1) {
        GUI_GDev->DrawHLine(x0, y0, x1, Color);
    }
}

/* 绘制查色表位图 */
static void _DrawLogBitmap(GUI_FLIPOUT *Cmd)
{
    int i, j;
    int x0 = Cmd->x0, y0 = Cmd->y0;
    const u_8 *pSrc = Cmd->pSrc;
    const GUI_COLOR *pLog = Cmd->pLog->pPalEntries;

    for (j = 0; j < (int)Cmd->ySize; ++j) {
        for (i = 0; i < (int)Cmd->xSize; ++i) {
            HL_SetPixel(x0 + i, y0 + j, pLog[*pSrc]);
            pSrc += 1;
        }
        pSrc += Cmd->Offset;
    }
}

/* 绘制位图 */
static void _DrawBitmap(GUI_FLIPOUT *Cmd)
{
    int i, j, pixBytes = 1;
    int x0 = Cmd->x0, y0 = Cmd->y0;
    const u_8 *pSrc = (const u_8 *)Cmd->pSrc;

    if (Cmd->SrcFormat == GUI_RGB565) {
        pixBytes = 2;
    } else if (Cmd->SrcFormat == GUI_RGB888) {
        pixBytes = 3;
    } else if (Cmd->SrcFormat == GUI_LOG) {
        _DrawLogBitmap(Cmd);
        return;
    }
    Cmd->Offset *= pixBytes;
    for (j = 0; j < (int)Cmd->ySize; ++j) {
        for (i = 0; i < (int)Cmd->xSize; ++i) {
            HL_SetPixel(x0 + i, y0 +j, GUI_RGB565To888(*(u_16 *)pSrc));
            pSrc += pixBytes;
        }
        pSrc += Cmd->Offset;
    }
}

/* 绘制像素 */
void _GL_SetPixel(int x, int y, GUI_COLOR Color)
{
    HL_SetPixel(x, y, Color);
}

/* 读取像素 */
GUI_COLOR _GL_GetPixel(int x, int y)
{
    return HL_GetPixel(x, y);
}

/* 裁剪绘制像素 */
static void _glSetPixelClip(int x, int y)
{
    CHECK_X(x);
    CHECK_Y(y);
    HL_SetPixel(x, y, GUI_Context.FGColor);
}

/* 绘制水平线 */
static void _glDrawHLine(int x0, int y0, int x1)
{
    CLIP_X(x0, x1);
    CHECK_Y(y0);
    if (x0 <= x1) {
        GUI_GDev->DrawHLine(x0, y0, x1, GUI_Context.FGColor);
    }
}

/* 绘制垂直线 */
void _GL_DrawVLine(int x0, int y0, int y1)
{
    CLIP_Y(y0, y1);
    CHECK_X(x0);
    if (y0 <= y1) {
        GUI_GDev->DrawVLine(x0, y0, y1, GUI_Context.FGColor);
    }
}

/* 填充矩形 */
static void _glFillRect(int x0, int y0, int x1, int y1)
{
    GUI_FLIPOUT Cmd;

    CLIP_X(x0, x1);
    CLIP_Y(y0, y1);
    Cmd.x0 = x0;
    Cmd.y0 = y0;
    Cmd.x1 = x1;
    Cmd.y1 = y1;
    Cmd.Color = GUI_Context.FGColor;
    GUI_GDev->FillRect(&Cmd);
}

/* 绘制位图 */
void _GL_DrawBitmap(u_8 PixelFormat,
    const unsigned char *pPixel,
    int x0,
    int y0,
    int xSize,
    int ySize,
    int Offset,
    const LCD_LOGPALETTE *pLog)
{
    GUI_FLIPOUT Cmd;

    Cmd.SrcFormat = PixelFormat;
    Cmd.pSrc = pPixel;
    Cmd.x0 = x0;
    Cmd.y0 = y0;
    Cmd.xSize = xSize;
    Cmd.ySize = ySize;
    Cmd.Offset = Offset;
    Cmd.pLog = pLog;
    GUI_GDev->DrawBitmap(&Cmd);
}

/* LCD初始化 */
void GUI_DeviceInit(void)
{
    GUI_glAPI.SetPixelClip = _glSetPixelClip;
    GUI_glAPI.DrawHLine = _glDrawHLine;
    GUI_glAPI.FillRect = _glFillRect;
    GUI_GDevList = NULL;
    GUI_GDev = GUI_GetDevice(GUI_DEF_SCREEN);
    GUI_GDev->Width = 0;
    GUI_GDev->Height = 0;
    GUI_GDev->SetPixel = _SetPixel;
    GUI_GDev->GetPixel = _GetPixel;
    GUI_GDev->DrawHLine = _DrawHLine;
    GUI_GDev->DrawVLine = _DrawVLine;
    GUI_GDev->FillRect = _FillRect;
    GUI_GDev->DrawBitmap = _DrawBitmap;
    GUI_UserConfig(GUI_GDev); /* 执行用户的初始化函数 */
}

/* 选中某个图形设备 */
GUI_RESULT GUI_SelectDevice(unsigned char id)
{
    GUI_GDEV *pNode;

    GUI_LOCK();
    for (pNode = GUI_GDevList; pNode && pNode->Id != id; pNode = pNode->pNext);
    if (pNode) {
        GUI_GDev = pNode;
    }
    GUI_UNLOCK();
    if (pNode) {
        return GUI_OK;
    }
    return GUI_ERR;
}

/* 获取一个图形设备的指针,如果设备不存在就创建一个 */
GUI_GDEV * GUI_GetDevice(unsigned char id)
{
    GUI_GDEV *pNode;

    GUI_LOCK();
    pNode = GUI_GDevList;
    if (pNode == NULL) { /* 链表为空则创建 */
        pNode = GUI_Malloc(sizeof(GUI_GDEV));
        pNode->Id = id;
        pNode->pNext = NULL;
    } else {
        while (pNode->pNext && pNode->Id != id) { /* 寻找ID为id的设备 */
            pNode = pNode->pNext;
        }
        if (pNode->Id != id) { /* 不存在设备 */
            pNode->pNext = GUI_Malloc(sizeof(GUI_GDEV));
            pNode = pNode->pNext;
            pNode->Id = id;
            pNode->pNext = NULL;
        }
    }
    GUI_UNLOCK();
    return pNode;
}

/* 删除图形设备 */
void GUI_DeleteDevice(unsigned char id)
{
    GUI_GDEV *pNode;

    if (id == GUI_DEF_SCREEN) { /* 默认设备不可删除 */
        return;
    }
    GUI_LOCK();
    pNode = GUI_GDevList;
    if (pNode->Id == id) {
        GUI_GDevList = pNode->pNext;
        GUI_Free(pNode);
    } else if (pNode) { /* 链表不为空 */
        while (pNode->pNext && pNode->pNext->Id != id) { /* 到要删除的前一个链节 */
            pNode = pNode->pNext;
        }
        if (pNode->pNext && pNode->pNext->Id == id) {
            if (GUI_GDev->Id == id) { /* 使用默认设备 */
                GUI_GDev = GUI_GetDevice(GUI_DEF_SCREEN);
            }
            pNode->pNext = pNode->pNext->pNext;
            GUI_Free(pNode->pNext);
        }
    }
    GUI_UNLOCK();
}

/* 删除图形设备表, 此函数调用之后不能再使用S-GUI */
void GUI_DeleteDeviceList(void)
{
    GUI_GDEV *pNode, *pNext;

    GUI_LOCK();
    pNode = GUI_GDevList;
    while (pNode) {
        pNext = pNode->pNext;
        GUI_Free(pNode);
        pNode = pNext;
    }
    GUI_GDevList = NULL;
    GUI_UNLOCK();
}
