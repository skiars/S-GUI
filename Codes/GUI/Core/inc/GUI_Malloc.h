#ifndef __GUI_Malloc_H
#define __GUI_Malloc_H

#include "GUI_Config.h"
#include "GUI_Typedef.h"
#include <stdio.h>

/* 内存池堆节点 */
typedef struct MEM_NODE {
    size_t Tag;             /* 被使用的堆节点标记 */
    size_t Size;            /* 堆节点字节数 */
    struct MEM_NODE *pLast; /* 指向上一个堆节点 */
    struct MEM_NODE *pNext; /* 指向下一个堆节点 */
} MEM_NODE;

/* 内存池对象 */
typedef struct MEM_HEAP {
    size_t Size;
    struct MEM_HEAP *pNext; /* 指向下一个内存池 */
    MEM_NODE MemPool;       /* 内存池堆空间, 必须作为最后的成员 */
} MEM_HEAP;

GUI_RESULT GUI_MemoryManagementInit(void);
void * GUI_Malloc(size_t Size);
void GUI_Free(void *Ptr);
size_t GUI_GetMemUsage(void);
size_t GUI_GetMemSize(void);

#endif /* __MEMALLOC_H */
