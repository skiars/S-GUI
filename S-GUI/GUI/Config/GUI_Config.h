#ifndef __GUI_CONFIG_H
#define __GUI_CONFIG_H

/*  键盘缓冲区长度 */
#define KEY_BUFFER_NUM        10

/* 定义消息队列最大消息数 */
#define GUI_MSG_QUEUE_SIZE    20

/* 是否使用存储器缓冲.
 * 0:不使用屏幕缓冲.
 * 1:使用屏幕缓冲.
 **/
#define GUI_USE_MEMORY        1

/* 默认字体 */
#define GUI_DEF_FONT          Font_ASCII_4PP

/* 最大消息嵌套层数 */
#define GUI_MAX_MSG_NEST      10

/* 裁剪矩形链表堆大小，注意不能太小，发现现实出现问题时，应该先增加
 * GUI_RECT_HEAP_SIZE 的值，如果问题没有解决再去排除其他地方的问题。
 **/
#define GUI_RECT_HEAP_SIZE    100

#endif
