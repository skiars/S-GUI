#ifndef __GUI_TYPEDEF_H
#define __GUI_TYPEDEF_H

typedef unsigned char  u_8;
typedef unsigned short u_16;
typedef unsigned long  u_32;
typedef short          i_16;
typedef int            i_32;


typedef void * GUI_hWin;      /* ´°¿Ú¾ä±ú */
typedef u_32 GUI_TIME;        /* GUIÊ±¼ä */
typedef u_32 GUI_COLOR;
typedef u_16 LCD_COLOR;

typedef enum {
    GUI_OK = 0,
    GUI_ERR
} GUI_RESULT;

#endif
