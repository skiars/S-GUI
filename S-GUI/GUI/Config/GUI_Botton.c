/*****************************************************************************
 * 本文件是S-GUI移植时需要配置的底层驱动。
 * 这个文件在Windows操作系统上使用。
 *****************************************************************************/

#include "GUI_Botton.h"

/* 用户自己添加的头文件 */
#include <stdio.h>
#include "..\..\simulate_lcd.h"
#include "..\..\memalloc.h"

/* 用于Windows的互斥信号量 */
HANDLE GUI_hMutex;

/* GUI多任务初始化 */
void GUI_InitOS(void)
{
    GUI_hMutex = CreateMutex(NULL, FALSE, NULL); /* 创建互斥量 */
}

/* GUI上锁 */
void GUI_TaskLock(void)
{
    WaitForSingleObject(GUI_hMutex, INFINITE); /* 等待互斥量 */
}

/* GUI解锁 */
void GUI_TaskUnlock(void)
{
    ReleaseMutex(GUI_hMutex); /* 释放互斥量 */
}

/* GUI获取任务ID */
u_32 GUI_GetTaskId(void)
{
    return GetCurrentThreadId();
}

/* 获得GUI时间,单位ms */
GUI_TIME GUI_GetTime(void)
{
    return GetTickCount(); /* 返回Windows启动后的ms数 */
}

/* 延时,单位ms */
void _GUI_Delay_ms(GUI_TIME tms)
{
    Sleep(tms); /* Windows下的延时函数 */
}

/* GUI动态内存支持 */
void *GUI_malloc(u_32 size)
{
    return memalloc(size, sim_mem2);
}

void GUI_free(void *ptr)
{
    memfree(ptr, sim_mem2);
}

void *GUI_fastmalloc(u_32 size)
{
    return memalloc(size, sim_mem1);
}

void GUI_fastfree(void *ptr)
{
    memfree(ptr, sim_mem1);
}

/* GUI图形硬件物理层接口 */
static GUI_COLOR Phy_GetPixel(u_16 x, u_16 y);
static void Phy_SetPixel(u_16 x, u_16 y, GUI_COLOR Color);
static void Phy_DispArea(u_16 x, u_16 y, u_16 xSize, u_16 ySize,
    GUI_COLOR *pBuffer, u_16 rowlen);

/* 图形硬件初始化 */
void GUI_Phy_Init(GUI_PHY_INFO *phys)
{
    phys->xSize = (u_16)sim_getWidth();
    phys->ySize = (u_16)sim_getHeight();
    phys->GetPixel = Phy_GetPixel;
    phys->SetPixel = Phy_SetPixel;
    phys->DispArea = Phy_DispArea;
    sim_lcd_init();
}

/* 读取屏幕上的点 */
static GUI_COLOR Phy_GetPixel(u_16 x, u_16 y)
{
    return sim_readPix(x, y);
}

/* 写入屏幕上的一个点 */
static void Phy_SetPixel(u_16 x, u_16 y, GUI_COLOR Color)
{
    sim_drawPix(x, y, Color);
}

/* 从GUI的内部缓冲区写入到屏幕（写入一个区域） */
static void Phy_DispArea(u_16 x,
    u_16 y,
    u_16 xSize,
    u_16 ySize,
    GUI_COLOR *pBuffer,
    u_16 rowlen)
{
    sim_dispArea(x, y, xSize, ySize, pBuffer, rowlen);
}

/* GUI调试输出 */
#if GUI_DEBUG_MODE
void _GUI_DebugOut(const char *s)
{
    /* 代码仅Win32适用 */
    AllocConsole();
    unsigned long lgsize;
    HANDLE hOutput = GetStdHandle(STD_OUTPUT_HANDLE);
    WriteFile(hOutput, s, strlen(s), &lgsize, 0);
    while (1);
}
#endif
