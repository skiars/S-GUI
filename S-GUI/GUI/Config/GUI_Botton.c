/*****************************************************************************
 * 本文件是S-GUI移植时需要配置的底层驱动。
 * 这个文件在Windows操作系统上使用。
 *****************************************************************************/

#include "GUI_Botton.h"

/* 用户自己添加的头文件 */
#include <stdio.h>
#include "..\..\sdlscreen.h"
#include "SDL.h"

/* 用于Windows的互斥信号量 */
SDL_mutex *GUI_Mutex;

/* GUI多任务初始化 */
void GUI_InitOS(void)
{
    GUI_Mutex = SDL_CreateMutex();
}

/* GUI上锁 */
void GUI_TaskLock(void)
{
    SDL_LockMutex(GUI_Mutex);
}

/* GUI解锁 */
void GUI_TaskUnlock(void)
{
    SDL_UnlockMutex(GUI_Mutex);
}

/* GUI获取任务ID */
u_32 GUI_GetTaskId(void)
{
    return SDL_GetThreadID(NULL);
}

/* 获得GUI时间,单位ms */
GUI_TIME GUI_GetTime(void)
{
    return SDL_GetTicks();
}

/* 延时,单位ms */
void _GUI_Delay_ms(GUI_TIME tms)
{
    SDL_Delay(tms);
}

/* 内存堆空间分配 */
void * _GUI_GetHeapBuffer(int Page, u_32 *Size)
{
    static char heap0[1024 * 256];
    static char heap1[1024 * 1024];

    if (Page == 0) {
        *Size = sizeof(heap0);
        return heap0;
    } else if (Page == 1) {
        *Size = sizeof(heap1);
        return heap1;
    }
    return NULL;
}

/* GUI图形硬件物理层接口 */
static GUI_COLOR Phy_GetPixel(u_16 x, u_16 y);
static void Phy_SetPixel(u_16 x, u_16 y, GUI_COLOR Color);

/* 图形硬件初始化 */
void GUI_Phy_Init(GUI_PHY_INFO *phys)
{
    phys->xSize = HAL_SCREEN_W;
    phys->ySize = HAL_SCREEN_H;
    phys->GetPixel = Phy_GetPixel;
    phys->SetPixel = Phy_SetPixel;
}

/* 读取屏幕上的点 */
static GUI_COLOR Phy_GetPixel(u_16 x, u_16 y)
{
    return HAL_GetPixel(x, y);
}

/* 写入屏幕上的一个点 */
static void Phy_SetPixel(u_16 x, u_16 y, GUI_COLOR Color)
{
    HAL_SetPixel(x, y, Color);
}

/* 填充矩形 */
void LCD_FillRect(i_16 x0, i_16 y0, i_16 x1, i_16 y1, GUI_COLOR Color)
{
    HAL_FillRect(x0, y0, x1, y1, Color);
}


void LCD_DrawBitmap(u_32 ColorFormat, const unsigned char *pPixel,
    i_16 x0, i_16 y0, u_16 xSize, u_16 ySize, u_16 Offset)
{
    if (ColorFormat == GUI_RGB888) {
        HAL_DrawBitmap(HAL_RGB888, pPixel, x0, y0, xSize, ySize, Offset);
    }
}

/* GUI调试输出 */
#if GUI_DEBUG_MODE
void _GUI_DebugOut(const char *s)
{
    printf(s);
    while (1);
}
#endif
