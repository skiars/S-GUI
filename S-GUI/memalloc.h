/*****************************************************************************
 * 动态内存分配程序(使用隐式链表实现).
 * 文件名: memalloc.h
 * 版  本: V1.2
 * 作  者: 官文亮
 * 日  期: 2016/4/23
 *****************************************************************************/
#ifndef __MEMALLOC_H
#define __MEMALLOC_H

#include <stdio.h>

/* 内存池堆节点 */
typedef struct MEM_NODE {
    size_t Tag;             /* 被使用的堆节点标记 */
    size_t Size;            /* 堆节点字节数 */
    struct MEM_NODE *pLast; /* 指向上一个堆节点 */
    struct MEM_NODE *pNext; /* 指向下一个堆节点 */
} MEM_NODE;

/* 内存池对象 */
typedef struct {
    size_t Size;            /* 内存池容量字节数 */
    size_t Usage;           /* 内存池使用量字节数 */
    MEM_NODE MemPool;       /* 内存池堆空间 */
} MEM_POOL;

int Mem_PoolInit(void *Mem, size_t Size);
void * memalloc(size_t Size, void *Mem);
void memfree(void *Ptr, void *Mem);
size_t MemGetUsageBytes(void *Mem);
size_t MemGetSizeBytes(void *Mem);

#endif /* __MEMALLOC_H */
