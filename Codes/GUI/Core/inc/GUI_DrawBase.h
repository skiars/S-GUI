#ifndef __GUI_DRAWBASE_H
#define __GUI_DRAWBASE_H

#include "GUI_Config.h"
#include "GUI_Typedef.h"

/* ªÊÕºAPI∂®“Â */
typedef struct {
    void (*setPixel)(int, int, GUI_COLOR);
    GUI_COLOR (*getPixel)(int, int);
    void (*drawHLine)(int, int, int, GUI_COLOR);
    void (*drawVLine)(int, int, int, GUI_COLOR);
    void (*fillRect)(int, int, int, int, GUI_COLOR);
} GUI_GLAPI;

/* GL APIs pionter. */
extern GUI_GLAPI *gui_gl_apis;

#endif
