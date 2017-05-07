#ifndef __GUI_CORE_H
#define __GUI_CORE_H

#include "GUI_Config.h"
#include "GUI_Typedef.h"
#include "GUI_Bottom.h"
#include "GUI_Queue.h"
#include "GUI_Font.h"

#define GUI_ALIGN_TOP     0x01 /* 顶部对齐 */
#define GUI_ALIGN_BOTTOM  0x02 /* 底部对齐 */
#define GUI_ALIGN_VCENTER 0x04 /* 垂直中心对齐 */
#define GUI_ALIGN_LEFT    0x08 /* 左对齐 */
#define GUI_ALIGN_RIGHT   0x10 /* 右对齐 */
#define GUI_ALIGN_HCENTER 0x20 /* 水平对齐 */

#define GUI_HEAP_FAST     0 /* 快速的堆(MCU内部的SRAM)页面 */
#define GUI_HEAP_HCAP     1 /* 大容量的堆(比如SDRAM)页面 */

#define GUI_DEBUG_OUT(s) GUI_DebugOut(s);

GUI_RESULT GUI_Init(void);
void GUI_Unload(void);
void GUI_Delay(GUI_TIME tms);
void GUI_LOCK(void);
void GUI_UNLOCK(void);
void GUI_DebugOut(const char *s);

#endif /* __GUI_CORE_H */
