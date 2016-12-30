#include "GUI_Surface.h"
#include "GUI.h"

GUI_SURFACE * GUI_SurfaceCreate(int x0,
    int y0,
    int Width,
    int Height)
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

void GUI_FlipSurfaceAt(int xPos, int yPos)
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
