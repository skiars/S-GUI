#include "GUI_StrokeLine.h"
#include <math.h>
#include "GUI.h"

enum {
    cmd_start,
    cmd_outline,
    cmd_end
};

enum path_commands_e {
    path_cmd_stop = 0,           //----path_cmd_stop    
    path_cmd_move_to = 1,        //----path_cmd_move_to 
    path_cmd_line_to = 2,        //----path_cmd_line_to 
    path_cmd_curve3 = 3,         //----path_cmd_curve3  
    path_cmd_curve4 = 4,         //----path_cmd_curve4  
    path_cmd_curveN = 5,         //----path_cmd_curveN
    path_cmd_catrom = 6,         //----path_cmd_catrom
    path_cmd_ubspline = 7,       //----path_cmd_ubspline
    path_cmd_end_poly = 0x0F,    //----path_cmd_end_poly
    path_cmd_mask = 0x0F         //----path_cmd_mask    
};

enum path_flags_e {
    path_flags_none = 0,         //----path_flags_none 
    path_flags_ccw = 0x10,       //----path_flags_ccw  
    path_flags_cw = 0x20,        //----path_flags_cw   
    path_flags_close = 0x40,     //----path_flags_close
    path_flags_mask = 0xF0       //----path_flags_mask 
};

static inline pos_t calc_distance(pos_t x1, pos_t y1, pos_t x2, pos_t y2)
{
    double dx = x2 - x1;
    double dy = y2 - y1;
    return sqrtf(dx * dx + dy * dy);
}

static inline pos_t cross_product(pos_t x1, pos_t y1,
                                  pos_t x2, pos_t y2,
                                  pos_t x, pos_t y)
{
    return (x - x2) * (y2 - y1) - (y - y2) * (x2 - x1);
}

// Calculate the intersection of two straight lines.
// l1: P1P2 --> (x1, y1) to (x2, y2).
// l2: P3P4 --> (x3, y3) to (x4, y4).
// return intersection point: *x, *y.
static inline int calc_intersection(pos_t x1, pos_t y1, pos_t x2, pos_t y2,
                                    pos_t x3, pos_t y3, pos_t x4, pos_t y4,
                                    pos_t* x, pos_t* y)
{
    pos_t num = (y1 - y3) * (x4 - x3) - (x1 - x3) * (y4 - y3);
    pos_t den = (x2 - x1) * (y4 - y3) - (y2 - y1) * (x4 - x3);
    if (fabsf(den) < 0.0001) {
        return 0;
    }
    *x = x1 + (x2 - x1) * num / den;
    *y = y1 + (y2 - y1) * num / den;
    return 1;
}

static void render_outline(GUI_Rasterizer * ras,
    pos_t x1, pos_t y1,
    pos_t x2, pos_t y2)
{
    ras_render_line(ras,
        (int)(x1 * 256), (int)(y1 * 256),
        (int)(x2 * 256), (int)(y2 * 256));
}

// add the intersection of two lines.
static void calc_join(StrokeLine *vc,
                      Vertex *v1,
                      Vertex *v2,
                      Vertex *v3,
                      pos_t len1,
                      pos_t len2)
{
    pos_t dx1 = vc->width * (v2->y - v1->y) / len1;
    pos_t dy1 = vc->width * (v2->x - v1->x) / len1;
    pos_t dx2 = vc->width * (v3->y - v2->y) / len2;
    pos_t dy2 = vc->width * (v3->x - v2->x) / len2;
    pos_t xi, yi;
    pos_t x1, y1, x2, y2, x3, y3;
    pos_t cp, limit;

    limit = len1 < len2 ? len1 : len2;
    if (len1 && len2) {
        cp = cross_product(v1->x, v1->y, v2->x, v2->y, v3->x, v3->y);
        if (cp > 0.0) {
            calc_intersection(v1->x + dx1, v1->y - dy1,
                v2->x + dx1, v2->y - dy1,
                v2->x + dx2, v2->y - dy2,
                v3->x + dx2, v3->y - dy2,
                &xi, &yi);

            if (calc_distance(v2->x, v2->y, xi, yi) >= limit) {
                xi = v2->x + dx1;
                yi = v2->y - dy1;
            }
            render_outline(vc->ras, vc->v1.x, vc->v1.y, xi, yi);
            vc->v1.x = xi;
            vc->v1.y = yi;

            x1 = v1->x - dx1;
            y1 = v1->y + dy1;
            x2 = v2->x - dx1;
            y2 = v2->y + dy1;
            x3 = v2->x - dx2;
            y3 = v2->y + dy2;
            vc->v2.x = x3;
            vc->v2.y = y3;
            render_outline(vc->ras, x2, y2, x1, y1);
            render_outline(vc->ras, x3, y3, x2, y2);
        } else {
            calc_intersection(v1->x - dx1, v1->y + dy1,
                v2->x - dx1, v2->y + dy1,
                v2->x - dx2, v2->y + dy2,
                v3->x - dx2, v3->y + dy2,
                &xi, &yi);

            if (calc_distance(v2->x, v2->y, xi, yi) >= limit) {
                xi = v2->x - dx1;
                yi = v2->y + dy1;
            }
            render_outline(vc->ras, xi, yi, vc->v2.x, vc->v2.y);
            vc->v2.x = xi;
            vc->v2.y = yi;

            x1 = v1->x + dx1;
            y1 = v1->y - dy1;
            x2 = v2->x + dx1;
            y2 = v2->y - dy1;
            x3 = v2->x + dx2;
            y3 = v2->y - dy2;
            vc->v1.x = x3;
            vc->v1.y = y3;
            render_outline(vc->ras, x1, y1, x2, y2);
            render_outline(vc->ras, x2, y2, x3, y3);
        }
        /*
        calc_intersection(v1->x + dx1, v1->y - dy1,
                          v2->x + dx1, v2->y - dy1,
                          v2->x + dx2, v2->y - dy2,
                          v3->x + dx2, v3->y - dy2,
                          &xi, &yi);
        vc->v1.x = xi;
        vc->v1.y = yi;
        calc_intersection(v1->x - dx1, v1->y + dy1,
                          v2->x - dx1, v2->y + dy1,
                          v2->x - dx2, v2->y + dy2,
                          v3->x - dx2, v3->y + dy2,
                          &xi, &yi);
        vc->v2.x = xi;
        vc->v2.y = yi;*/
    } else {
        if (len1 != 0) {
            vc->v1.x = v1->x + dx1;
            vc->v1.y = v1->y - dy1;
            vc->v2.x = v1->x - dx1;
            vc->v2.y = v1->y + dy1;
        } else if (len2 != 0) {
            vc->v1.x = v2->x + dx2;
            vc->v1.y = v2->y - dy2;
            vc->v2.x = v2->x - dx2;
            vc->v2.y = v2->y + dy2;
        }
    }
}

// add a line
void line_move_to(GUI_Rasterizer * ras, StrokeLine *vc, pos_t x, pos_t y)
{
    Vertex v, v1 = vc->v1, v2 = vc->v2;
    Vertex *vl1 = &vc->vl1;
    pos_t dx = x - vc->vl2.x;
    pos_t dy = y - vc->vl2.y;

    v.x = x;
    v.y = y;
    v.dist = calc_distance(x, y, vc->vl2.x, vc->vl2.y);

    if (v.dist == 0) { // coincident vertices, do nothing.
        return;
    }
    vc->ras = ras;
    calc_join(vc, &vc->vl1, &vc->vl2, &v, vc->vl2.dist, v.dist);
    switch (vc->cmd) {
    case cmd_start:
        render_outline(ras, vc->v2.x, vc->v2.y,
                            vc->v1.x, vc->v1.y);
        vc->cmd = cmd_outline;
        break;
    case cmd_outline:
        /*render_outline(ras, v1.x, v1.y, vc->v1.x, vc->v1.y);
        render_outline(ras, vc->v2.x, vc->v2.y, v2.x, v2.y);*/
        break;
    }
    vc->vl1 = vc->vl2;
    vc->vl2 = v;
}

// set end vertex
void line_stoke_end(GUI_Rasterizer * ras, StrokeLine *vc)
{
    Vertex *v1 = &vc->v1, v2, v3, *v4 = &vc->v2;

    pos_t dx = vc->width * (vc->vl2.y - vc->vl1.y) / vc->vl2.dist;
    pos_t dy = vc->width * (vc->vl2.x - vc->vl1.x) / vc->vl2.dist;

    v2.x = vc->vl2.x + dx;
    v2.y = vc->vl2.y - dy;
    v3.x = vc->vl2.x - dx;
    v3.y = vc->vl2.y + dy;

    render_outline(ras, v1->x, v1->y, v2.x, v2.y);
    render_outline(ras, v2.x, v2.y, v3.x, v3.y);
    render_outline(ras, v3.x, v3.y, v4->x, v4->y);
}

// set start vertex
void line_set_to(StrokeLine *vc, pos_t x, pos_t y)
{
    Vertex v;

    v.dist = 0;
    v.x = x;
    v.y = y;
    vc->v1 = v;
    vc->v2 = v;
    vc->vl1 = v;
    vc->vl2 = v;
    vc->cmd = cmd_start;
}
