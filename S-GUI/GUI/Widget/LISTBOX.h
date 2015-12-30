#ifndef __LISTBOX_H
#define __LISTBOX_H

#include "Widget.h"
#include "Linked_List.h"

#define LBOX_DFT_LBKCOLOR       0x00524C3C        /* 内部背景色 */
#define LBOX_DFT_LSELCOLOR      0x00FFFFFF        /* 选中list后的字体颜色 */
#define LBOX_DFT_LSELBKCOLOR    0x00CDCD96        /* 选中list后的背景色 */
#define LBOX_DFT_LNCOLOR        0x00000000        /* 未选中的list字体颜色 */
#define LBOX_DFT_RIMCOLOR       0x00000000        /* 边框颜色 */
#define LBOX_DFT_LINECOLOR      0x0033302A        /* 分隔线颜色 */

#define LBOX_DFT_HEIGHT         28      /* 条目高度 */
#define LBOX_DFT_SCROENDWID     48      /* 滚动显示末尾空白宽度 */
#define LBOX_DFT_UNSCRO_TIME    600     /* 选中项开始不滚动的ms数 */
#define LBOX_DFT_SCRO_TIME      50      /* 选中项显示滚动一个像素间隔时间(ms) */
#define LBOX_SCB_WIDTH          5       /* 滚动条宽度为5个像素 */

typedef struct {
    WIDGET Widget;
    u_16 ItemHei;       /* 条目高度 */
    u_16 ItemNum;       /* 条目的数量 */
    u_16 TopIndex;      /* 顶端的条目索引 */
    u_16 DispPosPix;    /* 当前的偏移 */
    u_16 PageItems;     /* 每页的条目数 */
    u_16 SelPixs;       /* 选中条目的像素长度 */
    u_16 SelIndex;      /* 选中的索引 */
    char *SelItem;      /* 选中的条目 */
    List pList;         /* 链表 */
    PNode LastNode;     /* 尾部节点 */
    const char *StrTab;
    WM_hWin hScro;      /* 滚动条句柄 */
    u_16 ScbWidth;
}LISTBOX_Obj;

WM_hWin LISTBOX_Create(i_16 x0,
                      i_16 y0,
                      u_16 xSize,
                      u_16 ySize,
                      WM_hWin hParent,
                      u_16 Id,
                      u_8 Flag,
                      u_16 namepos);
u_8 LISTBOX_addlist(WM_hWin hWin,char *name);
void LISTBOX_ItemDown(WM_hWin hWin);
void LISTBOX_ItemUp(WM_hWin hWin);
u_16 LISTBOX_GetSel(WM_hWin hWin);
char *LISTBOX_GetSelText(WM_hWin hWin);
u_8 LISTBOX_SetSel(WM_hWin hWin, u_16 Index);
u_8 LISTBOX_SetSelFromStr(WM_hWin hWin, const char *Str);

#endif
