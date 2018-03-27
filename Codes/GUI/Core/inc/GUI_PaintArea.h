#ifndef __GUI_PAINTAREA_H
#define __GUI_PAINTAREA_H

#include "GUI_Typedef.h"
#include "GUI_Config.h"
#include "GUI_Context.h"

/* 绘制相关上下文 */
typedef struct {
    GUI_FONT *font;
    GUI_COLOR background;
    GUI_COLOR foreground;
    GUI_COLOR fontColor;
    int penSize;
} GUI_PAINTCONTEXT;

void GUI_BackupContext(void);
void GUI_SyncContext(void);
void GUI_SetContext(GUI_HWIN hWin);
void GUI_PaintAreaInit(GUI_RECT *p);
GUI_BOOL GUI_NextPaintArea(void);

#endif
