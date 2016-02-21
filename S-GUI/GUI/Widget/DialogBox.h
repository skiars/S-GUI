#ifndef __DIALOGBOX_H
#define __DIALOGBOX_H

#include "GUI.h"

/* 自动创建窗口回调函数类型定义 */
typedef GUI_HWIN GUI_WIDGET_CREATE_FUNC (i_16 x0, i_16 y0, u_16 xSize,
                                         u_16 ySize, u_8 Cmd);

/* 自动创建窗口数据结构定义 */
typedef struct {
    GUI_WIDGET_CREATE_FUNC * Auto_Create;
    u_16 Id;  /* 窗口ID */
    i_16 x0, y0;
    u_16 xSize, ySize;
    u_8 Cmd;
} GUI_WIDGET_CREATE_INFO;

GUI_HWIN GUI_CreateDialogBox(GUI_WIDGET_CREATE_INFO *pWinfo,
                             u_16 NumWidgets, WM_CALLBACK *cb,
                             WM_HWIN hParent, i_16 x0, i_16 y0);
void GUI_DeleteDialogBox(GUI_HWIN hWin_Del);

#endif
