#ifndef __GUI_CONFIG_H
#define __GUI_CONFIG_H

/* 定义消息队列最大消息数 */
#define GUI_MSG_QUEUE_SIZE    10

/* 默认字体 */
#define GUI_DEF_FONT          GUI_FontASCII_8x16

/* 最大消息嵌套层数 */
#define GUI_MAX_MSG_NEST      10

/* 裁剪矩形链表堆大小，注意不能太小，发现现实出现问题时，应该先增加
 * GUI_RECT_HEAP_SIZE 的值，如果问题没有解决再去排除其他地方的问题。
 **/
#define GUI_RECT_HEAP_SIZE    50

/* 使用图形加速器 */
#define GUI_USE_GRAPHPHY      1

/* 调试模式开关 */
#define GUI_DEBUG_MODE        1

#endif
