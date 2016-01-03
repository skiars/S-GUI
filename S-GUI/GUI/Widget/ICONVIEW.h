#ifndef __ICONVIEW_H
#define __ICONVIEW_H

#include "WIDGET.h"

typedef struct
{
    WIDGET Widget;
    u_16 IconNum;       /* 图标数量 */
    u_16 SelIndex;      /* 选中的索引 */
    GUI_COLOR **BmpTab; /* 图标图片缓冲 */
    u_16 IconWidth;     /* 图标宽度 */
    u_16 IconHeight;    /* 图标高度 */
    u_16 DstncX;        /* 水平间距 */
    u_16 DstncY;        /* 垂直间距 */
} ICONVIEW_Obj;

#endif

