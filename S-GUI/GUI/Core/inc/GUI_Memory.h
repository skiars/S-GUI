#ifndef __GUI_MEMORY_H
#define __GUI_MEMORY_H

#include "GUI_Typedef.h"

#if GUI_USE_MEMORY == 0
    #define GUI_DrawPixel(x, y, Color) \
                (GUI_Data->phy_info.SetPixel(x, y, Color))
    #define GUI_ReadPixel(x, y) \
                (GUI_Data->phy_info.GetPixel(x, y))
#else
    #define GUI_DrawPixel(x, y, Color) \
                (GUI_Data->lcdbuf[(y) * GUI_GetScreenWidth() + (x)] = (Color))
    #define GUI_ReadPixel(x, y) \
                (GUI_Data->lcdbuf[(y) * GUI_GetScreenWidth() + (x)])
#endif


void GUI_DrawFromMem(u_16 x,u_16 y,u_16 len,u_16 wid);
void GUI_ReadBack(u_16 x,u_16 y,u_16 len,u_16 wid,GUI_COLOR *pb);
void GUI_RecoverBack(u_16 x,u_16 y,u_16 len,u_16 wid,GUI_COLOR *pb);
void GUI_MemToMem(u_16 x,u_16 y,u_16 len,u_16 wid,GUI_COLOR *pb);

#endif /* __GUI_MEMORY_H */







