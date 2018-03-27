#include "GUI_Rasterizer.h"
#include "GUI_DrawBase.h"
#include "GUI.h"

enum {
    poly_subpixel_shift = 8,
    poly_subpixel_scale = 1 << poly_subpixel_shift,
    poly_subpixel_mask  = poly_subpixel_scale - 1
};

enum {
    cell_block_shift   = 12,
    cell_block_size    = 1 << cell_block_shift,
    cell_block_mask    = cell_block_size - 1,
    cell_block_pool    = 256,
    cell_block_limit   = 1024
};

static void record_curr_cell(GUI_Rasterizer *ras)
{
    TCell  **pcell, *cell;
    int  x = ras->ex;

    if (ras->ey < ras->min_ey || ras->ey > ras->max_ey) {
        return;
    }
    pcell = &ras->ycells[ras->ey - ras->min_ey];
    for (;;) {
        cell = *pcell;
        if (!cell || cell->x > x) {
            break;
        }
        if (cell->x == x) {
            /* update old cell */
            cell->area += ras->area;
            cell->cover += ras->cover;
            return;
        }
        pcell = &cell->next;
    }

    /* insert new cell */
    cell = ras->cells + ras->num_cells++;
    cell->x = x;
    cell->area = ras->area;
    cell->cover = ras->cover;

    cell->next = *pcell;
    *pcell = cell;
}

static void set_curr_cell(GUI_Rasterizer *ras, int ex, int ey)
{
    if (ex < ras->min_ex) {
        ex = ras->min_ex - 1;
    }
    if (ex > ras->max_ex) {
        ex = ras->max_ex + 1;
    }
    if (ras->ex != ex || ras->ey != ey) {
        record_curr_cell(ras);
        ras->area = 0;
        ras->cover = 0;
        ras->ex = ex;
        ras->ey = ey;
    }
}

static void render_hline(GUI_Rasterizer *ras, int ey,
                                              int x1, int y1,
                                              int x2, int y2)
{
    int ex1 = x1 >> poly_subpixel_shift;
    int ex2 = x2 >> poly_subpixel_shift;
    int fx1 = x1 & poly_subpixel_mask;
    int fx2 = x2 & poly_subpixel_mask;

    int delta, p, first, dx;
    int incr, lift, mod, rem;

    //trivial case. Happens often
    if (y1 == y2) {
        set_curr_cell(ras, ex2, ey);
        return;
    }

    //everything is located in a single cell.  That is easy!
    if (ex1 == ex2) {
        delta = y2 - y1;
        ras->cover += delta;
        ras->area += (fx1 + fx2) * delta;
        return;
    }

    //ok, we'll have to render a run of adjacent cells on the same
    //hline...
    p = (poly_subpixel_scale - fx1) * (y2 - y1);
    first = poly_subpixel_scale;
    incr = 1;

    dx = x2 - x1;

    if (dx < 0) {
        p = fx1 * (y2 - y1);
        first = 0;
        incr = -1;
        dx = -dx;
    }

    delta = p / dx;
    mod = p % dx;

    if (mod < 0) {
        delta--;
        mod += dx;
    }

    ras->cover += delta;
    ras->area += (fx1 + first) * delta;

    ex1 += incr;
    set_curr_cell(ras, ex1, ey);
    y1 += delta;

    if (ex1 != ex2) {
        p = poly_subpixel_scale * (y2 - y1 + delta);
        lift = p / dx;
        rem = p % dx;

        if (rem < 0) {
            lift--;
            rem += dx;
        }

        mod -= dx;

        while (ex1 != ex2) {
            delta = lift;
            mod += rem;
            if (mod >= 0) {
                mod -= dx;
                delta++;
            }

            ras->cover += delta;
            ras->area += poly_subpixel_scale * delta;
            y1 += delta;
            ex1 += incr;
            set_curr_cell(ras, ex1, ey);
        }
    }
    delta = y2 - y1;
    ras->cover += delta;
    ras->area += (fx2 + poly_subpixel_scale - first) * delta;
}

// render a outline
void ras_render_line(GUI_Rasterizer *ras, int x1, int y1, int x2, int y2)
{
    int dx = x2 - x1;
    int dy = y2 - y1;
    int ex1 = x1 >> poly_subpixel_shift;
    int ey1 = y1 >> poly_subpixel_shift;
    int ey2 = y2 >> poly_subpixel_shift;
    int fy1 = y1 & poly_subpixel_mask;
    int fy2 = y2 & poly_subpixel_mask;
    int x_from, x_to;
    int p, rem, mod, lift, delta, first, incr;

    set_curr_cell(ras, ex1, ey1);

    //everything is on a single hline
    if (ey1 == ey2) {
        render_hline(ras, ey1, x1, fy1, x2, fy2);
        return;
    }
    //Vertical line - we have to calculate start and end cells,
    //and then - the common values of the area and coverage for
    //all cells of the line. We know exactly there's only one 
    //cell, so, we don't have to call render_hline().
    incr = 1;
    if (dx == 0) {
        int ex = x1 >> poly_subpixel_shift;
        int two_fx = (x1 - (ex << poly_subpixel_shift)) << 1;
        int area;

        first = poly_subpixel_scale;
        if (dy < 0) {
            first = 0;
            incr = -1;
        }

        x_from = x1;

        //render_hline(ey1, x_from, fy1, x_from, first);
        delta = first - fy1;
        ras->cover += delta;
        ras->area += two_fx * delta;

        ey1 += incr;
        set_curr_cell(ras, ex, ey1);

        delta = first + first - poly_subpixel_scale;
        area = two_fx * delta;
        while (ey1 != ey2) {
            //render_hline(ey1, x_from, poly_subpixel_scale - first, x_from, first);
            ras->cover = delta;
            ras->area = area;
            ey1 += incr;
            set_curr_cell(ras, ex, ey1);
        }
        //render_hline(ey1, x_from, poly_subpixel_scale - first, x_from, fy2);
        delta = fy2 - poly_subpixel_scale + first;
        ras->cover += delta;
        ras->area += two_fx * delta;
        return;
    }

    //ok, we have to render several hlines
    p = (poly_subpixel_scale - fy1) * dx;
    first = poly_subpixel_scale;

    if (dy < 0) {
        p = fy1 * dx;
        first = 0;
        incr = -1;
        dy = -dy;
    }

    delta = p / dy;
    mod = p % dy;

    if (mod < 0) {
        delta--;
        mod += dy;
    }

    x_from = x1 + delta;
    render_hline(ras, ey1, x1, fy1, x_from, first);

    ey1 += incr;
    set_curr_cell(ras, x_from >> poly_subpixel_shift, ey1);

    if (ey1 != ey2) {
        p = poly_subpixel_scale * dx;
        lift = p / dy;
        rem = p % dy;

        if (rem < 0) {
            lift--;
            rem += dy;
        }
        mod -= dy;

        while (ey1 != ey2) {
            delta = lift;
            mod += rem;
            if (mod >= 0) {
                mod -= dy;
                delta++;
            }

            x_to = x_from + delta;
            render_hline(ras, ey1, x_from, poly_subpixel_scale - first, x_to, first);
            x_from = x_to;

            ey1 += incr;
            set_curr_cell(ras, x_from >> poly_subpixel_shift, ey1);
        }
    }
    render_hline(ras, ey1, x_from, poly_subpixel_scale - first, x2, fy2);
}

// 计算透明度
static unsigned calculate_alpha(int area)
{
    int cover = area >> (poly_subpixel_shift + 1);

    if (cover < 0) {
        cover = -cover;
    }
    if (cover > poly_subpixel_mask) {
        cover = poly_subpixel_mask;
    }
    return cover;
}

// 渲染扫描线
void sweep_scanlines(GUI_Rasterizer *ras)
{
    int y;

    record_curr_cell(ras); // 保存最后一个cell
    for (y = ras->min_ey; y < ras->max_ey; ++y) {
        TCell* cell = ras->ycells[y - ras->min_ey];
        int cover = 0;

        while (cell) {
            int x = cell->x;
            int area = cell->area;
            unsigned alpha;

            cover += cell->cover;
            cell = cell->next;
            if (area) {
                alpha = calculate_alpha((cover << (poly_subpixel_shift + 1)) - area);
                if (alpha && x >= ras->min_ex && x <= ras->max_ex) {
                    ras->blend_pixel(x, y, alpha);
                }
                x++;
            }
            if (cover && cell && cell->x > x) { // 填充内部
                alpha = calculate_alpha(cover << (poly_subpixel_shift + 1));
                ras->add_span(x, y, cell->x - 1, alpha);
            }
        }
    }
}

void ras_move_to(GUI_Rasterizer *ras, int x, int y)
{
    record_curr_cell(ras); // 保存最后一个cell
    ras->x = x;
    ras->y = y;
    ras->ex = x >> poly_subpixel_shift;
    ras->ey = y >> poly_subpixel_shift;
    ras->cover = 0;
    ras->area = 0;
}

// 添加一条边
void ras_line_to(GUI_Rasterizer *ras, int to_x, int to_y)
{
    ras_render_line(ras, ras->x, ras->y, to_x, to_y);
    ras->x = to_x;
    ras->y = to_y;
}

static void blend_pixel(int x, int y, unsigned alpha)
{
    GUI_COLOR color = GUI_AlphaBlend(GUI_CurrentForeground(), gui_gl_apis->getPixel(x, y), 255 - alpha);
    gui_gl_apis->setPixel(x, y, color);
}

static void fill_span(int x0, int y0, int x1)
{
    GUI_COLOR color = GUI_CurrentForeground();

    gui_gl_apis->drawHLine(x0, y0, x1, color);
}

static void add_span(int x0, int y0, int x1, int alpha)
{
    if (alpha == 0) {
        return;
    }
    if (alpha == 0xFF) {
        fill_span(x0, y0, x1);
        return;
    }
    while (x0 <= x1) {
        blend_pixel(x0++, y0, alpha);
    }
}

GUI_Rasterizer *rasterizer_init(void)
{
    int ycount = 400;
    GUI_Rasterizer *ras = (GUI_Rasterizer *)GUI_Malloc(sizeof(GUI_Rasterizer));

    ras->cells = (TCell *)GUI_Malloc(sizeof(TCell) * 3000);
    ras->ycells = (TCell **)GUI_Malloc(sizeof(TCell *) * ycount);
    while (ycount--) {
        ras->ycells[ycount] = NULL;
    }
    ras->num_cells = 0;
    ras->blend_pixel = blend_pixel;
    ras->add_span = add_span;
    ras->ey = 0;
    ras->ex = 0;
    ras->min_ex = 0;
    ras->max_ex = 255;
    ras->min_ey = 0;
    ras->max_ey = 255;
    ras->cover = 0;
    ras->area = 0;
    return ras;
}

void rasterizer_free(GUI_Rasterizer *ras)
{
    GUI_Free(ras->cells);
    GUI_Free(ras->ycells);
    GUI_Free(ras);
}
