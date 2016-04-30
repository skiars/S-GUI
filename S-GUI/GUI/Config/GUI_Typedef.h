#ifndef __GUI_TYPEDEF_H
#define __GUI_TYPEDEF_H

typedef unsigned char  u_8;
typedef unsigned short u_16;
typedef unsigned long  u_32;
typedef short          i_16;
typedef int            i_32;

typedef void * GUI_HWIN;      /* ´°¿Ú¾ä±ú */
typedef u_32 GUI_TIME;        /* GUIÊ±¼ä */
typedef u_32 GUI_COLOR;
typedef u_16 LCD_COLOR;

#define GUI_RGB888     1
#define GUI_RGB565     2

typedef enum {
    GUI_OK = 0,
    GUI_ERR,
} GUI_RESULT;

typedef u_8 GUI_BOOL;

#ifndef TRUE
#define TRUE 1
#endif

#ifndef FALSE
  #define FALSE 0
#endif

#ifndef NULL
#ifdef __cplusplus
  #define NULL 0
#else
  #define NULL ((void *)0)
#endif
#endif

#endif /* __GUI_TYPEDEF_H */
