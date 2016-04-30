#include "GUI_Memory.h"
#include "GUI.h"

/* GUI内存绘图支持 */

/* 将屏幕缓冲区的数据显示到屏幕 */
void GUI_DrawFromMem(u_16 x, u_16 y, u_16 len, u_16 wid)
{

}

void GUI_DrawBlock(u_16 x, u_16 y, u_16 len, u_16 wid, GUI_COLOR *data)
{

}

/* 读取屏幕缓冲区的数据 */
void GUI_ReadBack(u_16 x, u_16 y, u_16 len, u_16 wid, GUI_COLOR *pb)
{

}

/* 恢复屏幕缓冲区的数据，pb指向数据源 */
void GUI_RecoverBack(u_16 x, u_16 y, u_16 len, u_16 wid, GUI_COLOR *pb)
{

}

void GUI_MemToMem(u_16 x, u_16 y, u_16 len, u_16 wid, GUI_COLOR *pb)
{

}

/* GUI显示 */
void GUI_DrawDevice(i_16 x0, i_16 y0, i_16 x1, i_16 y1)
{
    GUI_DrawFromMem(x0, y0, x1 - x0 + 1, y1 - y0 + 1);
}
