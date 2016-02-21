#ifndef __LISTBOX_H
#define __LISTBOX_H

#include "Widget.h"
#include "Linked_List.h"

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
    WM_HWIN hScro;      /* 滚动条句柄 */
    u_16 ScbWidth;
}LISTBOX_Obj;

WM_HWIN LISTBOX_Create(i_16 x0,
                      i_16 y0,
                      u_16 xSize,
                      u_16 ySize,
                      WM_HWIN hParent,
                      u_16 Id,
                      u_8 Flag,
                      u_16 namepos);
GUI_RESULT LISTBOX_AddList(WM_HWIN hWin,char *name);
void LISTBOX_ItemDown(WM_HWIN hWin);
GUI_RESULT LISTBOX_ItemUp(WM_HWIN hWin);
u_16 LISTBOX_GetSel(WM_HWIN hWin);
char *LISTBOX_GetSelText(WM_HWIN hWin);
GUI_RESULT LISTBOX_SetSel(WM_HWIN hWin, u_16 Index);
GUI_RESULT LISTBOX_SetSelFromStr(WM_HWIN hWin, const char *Str);
void LISTBOX_ScrollDisplay(GUI_HWIN hWin);

#endif
