#ifndef __GUI_CONFIG_H
#define __GUI_CONFIG_H

/* 定义消息队列最大消息数 */
#define GUI_MSG_QUEUE_SIZE    10

/* 默认字体 */
#define GUI_DEF_FONT          GUI_FontUI17_4pp

/* 最大消息嵌套层数 */
#define GUI_MAX_MSG_NEST      10

/* 使用图形加速器 */
#define GUI_USE_GRAPHPHY      0

/* 调试模式开关 */
#define GUI_DEBUG_MODE        1

/* 自动维护时间 */
#define GUI_AUTO_MANAGE_TIME  5000

/* 绘制多变形时支持的最大边数 */
#define GUI_MAX_POLY_CORNERS  1000

/**
 @ 字体显示模式
 @ 0:默认的模式.
 @ 1:行加速(使用位图绘制).
 @ 2:整个字符加速(使用位图绘制).
 **/
#define GUI_DISP_FONT_MODE    0

/**
 @ 定义最大字符宽度,GUI_DISP_FONT_MODE >= 1时有效
 **/
#define GUI_MAX_FONT_WID      24

/**
 @ 定义最大字符宽度,GUI_DISP_FONT_MODE == 2时有效
 **/
#define GUI_MAX_FONT_HEI      48

#endif
