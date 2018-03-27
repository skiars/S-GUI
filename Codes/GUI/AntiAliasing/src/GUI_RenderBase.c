#include "GUI_RenderBase.h"
#include "GUI_DrawBase.h"
#include "GUI.h"

static void base_blend_pixel(GUI_RenderBase *ren, int x, int y, unsigned alpha)
{
    GUI_COLOR color = GUI_AlphaBlend(ren->color,
        gui_gl_apis->getPixel(x, y), 255 - alpha);

    gui_gl_apis->setPixel(x, y, color);
}

static void base_fill_span(GUI_RenderBase *ren, int x0, int y0, int x1)
{
    gui_gl_apis->drawHLine(x0, y0, x1, ren->color);
}

static GUI_COLOR get_color(GUI_RenderBase *ren, int x, int y)
{
    return ren->color;
}

static copy_color_hspan(int x, int y, int len, const GUI_COLOR* colors)
{

}
