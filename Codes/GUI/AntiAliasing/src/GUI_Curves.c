#include "GUI_Curves.h"
#include "GUI_StrokeLine.h"
#include <math.h>

static int length(pos_t dx1, pos_t dy1, pos_t dx2, pos_t dy2)
{
    int len = (int)((sqrtf(dx1 * dx1 + dy1 * dy1)
        + sqrtf(dx1 * dx1 + dy1 * dy1)) / 10.0f + 0.5f);

    return len > 20 ? len : 20;
}

void curve2(GUI_Rasterizer * ras,
            pos_t x1, pos_t y1,
            pos_t x2, pos_t y2, 
            pos_t x3, pos_t y3)
{
    int u, u1;
    int step, step2;
    int bez;
    pos_t x, y;
    StrokeLine vc;

    step = length(x2 - x1, y2 - y1, x3 - x2, y3 - y2);

    step2 = step * step;

    vc.width = 10.0f;
    line_set_to(&vc, x3, y3);

    // the first point doesn't need to be painted.
    for (u = 1; u <= step; ++u) {
        u1 = step - u;
        bez = u * u;
        x = x1 * bez;
        y = y1 * bez;
        bez = 2 * u * u1;
        x += x2 * bez;
        y += y2 * bez;
        bez = u1 * u1;
        x += x3 * bez;
        y += y3 * bez;
        x /= step2;
        y /= step2;
        line_move_to(ras, &vc, x, y);
    }
    line_stoke_end(ras, &vc);
}
