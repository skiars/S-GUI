#include "GUI_Surface.h"
#include "GUI.h"

GUI_SURFACE * GUI_SurfaceCreate(i_16 x0,
    i_16 y0,
    u_16 Width,
    u_16 Height)
{
    return NULL;
}

void GUI_SelectSurface(GUI_SURFACE *pSrfc)
{
    GUI_Context.Surface = pSrfc;
}

void GUI_FlipSurface(void)
{
#if 0
    GUI_SURFACE *pSrfc;

    if (GUI_Context.Surface) {
        pSrfc = GUI_Context.Surface;
        GUI_Context.Surface = NULL;
        /* Draw bitmap... */
    }
#endif
}

void GUI_FlipSurfaceAt(i_16 xPos, i_16 yPos)
{
#if 0
    GUI_SURFACE *pSrfc;

    if (GUI_Context.Surface) {
        pSrfc = GUI_Context.Surface;
        GUI_Context.Surface = NULL;
        /* Draw bitmap... */
    }
#endif
}
