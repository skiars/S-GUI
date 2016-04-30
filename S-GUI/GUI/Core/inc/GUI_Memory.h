#ifndef __GUI_MEMORY_H
#define __GUI_MEMORY_H

#include "GUI_Typedef.h"

    #define GUI_DrawPixel(x, y, Color) \
                (GUI_Data->phy_info.SetPixel(x, y, Color))
    #define GUI_ReadPixel(x, y) \
                (GUI_Data->phy_info.GetPixel(x, y))

void GUI_DrawFromMem(u_16 x,u_16 y,u_16 len,u_16 wid);
void GUI_ReadBack(u_16 x,u_16 y,u_16 len,u_16 wid,GUI_COLOR *pb);
void GUI_RecoverBack(u_16 x,u_16 y,u_16 len,u_16 wid,GUI_COLOR *pb);
void GUI_MemToMem(u_16 x,u_16 y,u_16 len,u_16 wid,GUI_COLOR *pb);
void GUI_DrawDevice(i_16 x0, i_16 y0, i_16 x1, i_16 y1);

#endif /* __GUI_MEMORY_H */







