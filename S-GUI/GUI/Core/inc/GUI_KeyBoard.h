#ifndef __GUI_KEYBOARD_H
#define __GUI_KEYBOARD_H

#include "GUI_Typedef.h"

/* 1~127为标准ASCII码表 */
/* 128~255为按键键值 */
#define GUI_KEY_UP       128
#define GUI_KEY_DOWN     129
#define GUI_KEY_LEFT     130
#define GUI_KEY_RIGHT    131
#define GUI_KEY_ENTER    134
#define GUI_KEY_RETURN   135

void GUI_KeyBufferInit(void); /* 键盘缓冲区初始化 */
void GUI_KeyBufferDelete(void); /* 删除键盘缓冲区 */
void GUI_SendKeyMsg(u_8 Key); /* 把键值存储到键盘缓冲器 */
u_8 GUI_GetKey(void); /* 从键盘缓冲器读取键值 */
void GUI_KeyProcess(void); /* 键盘处理 */
void GUI_CleanKeyBuffer(void); /* 清空键盘缓冲器 */

#endif /* __GUI_KEYBOARD_H */
