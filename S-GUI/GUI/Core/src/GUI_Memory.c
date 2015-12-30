#include "GUI_Memory.h"
#include "GUI.h"

/* GUI内存绘图支持 */

/* 将屏幕缓冲区的数据显示到屏幕 */
void GUI_DrawFromMem(u_16 x, u_16 y, u_16 len, u_16 wid)
{
#if GUI_USE_MEMORY
    GUI_Data->phy_info.DispArea(x, y, len, wid,
                                GUI_Data->lcdbuf, GUI_GetScreenWidth());
#endif
}

void GUI_DrawBlock(u_16 x, u_16 y, u_16 len, u_16 wid, GUI_COLOR *data)
{
#if GUI_USE_MEMORY
    u_16 i, j, lcd_w = GUI_GetScreenWidth();
    GUI_COLOR *plcd, *ptd;

    ptd = data;
    plcd = GUI_Data->lcdbuf;
    plcd += y * lcd_w;
    for (i = 0; i < wid; i++) {
        for (j = 0; j < len; j++) {
            plcd[x + j] = ptd[j];
        }
        ptd += len;
        plcd += lcd_w;
    }
#endif
}

/* 读取屏幕缓冲区的数据 */
void GUI_ReadBack(u_16 x, u_16 y, u_16 len, u_16 wid, GUI_COLOR *pb)
{
#if GUI_USE_MEMORY
    u_16 i, j, lcd_w = GUI_GetScreenWidth();
    GUI_COLOR *plcd;
    plcd = GUI_Data->lcdbuf;
    plcd += y*lcd_w;
    for (i = 0; i < wid; i++)
    {
        for (j = 0; j < len; j++)
        {
            pb[j] = plcd[x + j];
        }
        pb += len;
        plcd += lcd_w;
    }
#endif
}

/* 恢复屏幕缓冲区的数据，pb指向数据源 */
void GUI_RecoverBack(u_16 x, u_16 y, u_16 len, u_16 wid, GUI_COLOR *pb)
{
#if GUI_USE_MEMORY
    u_16 i, j, lcd_w = GUI_GetScreenWidth();
    GUI_COLOR *plcd;
    plcd = GUI_Data->lcdbuf;
    plcd += y * lcd_w;
    for (i = 0; i < wid; i++) {
        for (j = 0; j < len; j++) {
            plcd[x + j] = pb[x + j];
        }
        pb += len;
        plcd += lcd_w;
    }
#endif
}

void GUI_MemToMem(u_16 x, u_16 y, u_16 len, u_16 wid, GUI_COLOR *pb)
{
#if GUI_USE_MEMORY
    u_16 i, j, lcd_w = GUI_GetScreenWidth();
    GUI_COLOR *plcd;
    plcd = GUI_Data->lcdbuf;
    pb += y * lcd_w;
    plcd += y * lcd_w;
    for (i = 0; i < wid; i++) {
        for (j = 0; j < len; j++) {
            plcd[x + j] = pb[x + j];
        }
        pb += lcd_w;
        plcd += lcd_w;
    }
#endif
}

