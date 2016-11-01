#ifndef __GUI_KEYBOARD_H
#define __GUI_KEYBOARD_H

#include "GUI_Typedef.h"
#include "GUI_Queue.h"

#define GUI_KEYUP        0x00
#define GUI_KEYDOWN      0x01

/* 0x00 ~ 0x7F为标准ASCII码表 */
#define KEY_BACK         0x08 /* 退格键 */
#define KEY_TAB          0x09 /* TAB键 */
#define KEY_ENTER        0x0D /* 回车 */
#define KEY_SHIFT        0x10 /* Shift键 */
#define KEY_SPACE        0x20 /* 空格 */
/* 0x80 ~ 0xFF为扩展键值 */
#define KEY_ESC          0x80 /* ESC键 */
#define KEY_DELETE       0x81 /* Delete键 */
#define KEY_INSERT       0x82 /* Insert键 */
#define KEY_PGUP         0x83 /* Page Up键 */
#define KEY_PGDN         0x84 /* Page Down键 */
#define KEY_END          0x85 /* End键 */
#define KEY_HOME         0x86 /* Home键 */
#define KEY_LEFT         0x90 /* 左 */
#define KEY_UP           0x91 /* 上 */
#define KEY_RIGHT        0x92 /* 右 */
#define KEY_DOWN         0x93 /* 下 */
#define KEY_LBUTTON      0x94 /* 鼠标左键 */
#define KEY_RBUTTON      0x95 /* 鼠标右键 */
#define KEY_MBUTTON      0x96 /* 鼠标中键 */

void GUI_SendKey(u_8 Key, u_8 Status);
GUI_RESULT GUI_KeyMessageProc(GUI_MESSAGE *pMsg);

#endif /* __GUI_KEYBOARD_H */
