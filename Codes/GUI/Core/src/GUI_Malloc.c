#include "GUI_Malloc.h"
#include "GUI_Core.h"

/**
 @ 内存池初始化.
 @ Mem: 内存池地址(建议这个地址是4字节对齐的, 以便能满足特殊需求).
 @ Size: 内存池大小(Byte).
 @ 返回值: GUI_OK: 内存池初始化成功, GUI_ERR: 内存池初始化失败.
 **/
GUI_RESULT GUI_HeapInit(void *Mem, size_t Size)
{
    MEM_HEAP *mPool;

    if (Size < sizeof(MEM_HEAP)) {
#if GUI_DEBUG_MODE
        GUI_DEBUG_OUT("GUI heap size is below the minimum.");
#endif
        return GUI_ERR; /* 容量太小 */
    }
    mPool = Mem;
    mPool->Size = Size;
    mPool->Usage = 0;
    mPool->MemPool.Tag = 0;
    mPool->MemPool.pLast = NULL;
    mPool->MemPool.pNext = NULL;
    mPool->MemPool.Size = Size - (sizeof(MEM_HEAP) - sizeof(MEM_NODE));
    return GUI_OK;
}

/**
 @ 申请内存, 内部调用.
 @ pl: 内存池的第一个堆节点地址.
 @ Size: 要申请的字节数.
 @ 返回值: 申请到的堆节点地址, 返回NULL表示申请失败.
 **/
static MEM_NODE * __alloc(MEM_NODE *pl, size_t Size)
{
    MEM_NODE *pn;

    /* 寻找空间足够的空闲节点 */
    while (pl && (pl->Size < Size + sizeof(MEM_NODE) || pl->Tag)) {
        pl = pl->pNext;
    }
    if (pl) {
        /* 空间够大则进行分割 */
        if (pl->Size > Size + sizeof(MEM_NODE) * 2) {
            pl->Size -= Size + sizeof(MEM_NODE);
            pn = (MEM_NODE *)((char *)pl + pl->Size);
            pn->pNext = pl->pNext;
            pn->Size = Size + sizeof(MEM_NODE);
            pn->pLast = pl;
            pl->pNext = pn;
            pl = pn;
            pn = pl->pNext;
            if (pn) {
                pn->pLast = pl;
            }
        }
        pl->Tag = 1;
    }
    return pl;
}

/**
 @ 释放内存, 内部调用.
 @ pl: 需要释放的堆节点地址.
 **/
static void __free(MEM_NODE *pl)
{
    MEM_NODE *pn;

    pl->Tag = 0; /* 标记为空闲 */
    /* 如果前一个堆节点是空闲的就合并两个节点 */
    pn = pl->pLast; /* 指向前一个链节 */
    if (pn && !pn->Tag) {
        pn->pNext = pl->pNext;
        pn->Size += pl->Size;
        pl = pn;
        pn = pl->pNext;
        if (pn) {
            pn->pLast = pl;
        }
    }
    /* 如果下一个堆节点是空闲的就合并两个节点 */
    pn = pl->pNext; /* 指向下一链节 */
    if (pn && !pn->Tag) {
        pl->pNext = pn->pNext;
        pl->Size += pn->Size;
        pn = pl->pNext;
        if (pn) {
            pn->pLast = pl;
        }
    }
}

/**
 @ 申请内存, 外部调用.
 @ Size: 要申请的字节数.
 @ Mem: 内存池地址.
 @ 返回值: 申请到空间的首地址.
 **/
void * GUI_Malloc(size_t Size, void * Mem)
{
    MEM_NODE *pn;
    if (Size) {
        if (Size & 0x03) { /* 4字节对齐 */
            Size += 4 - Size & 0x03;
        }
        pn = &((MEM_HEAP *)Mem)->MemPool;
        pn = __alloc(pn, Size);
        if (pn) {
            ((MEM_HEAP *)Mem)->Usage += pn->Size;
            return (char *)pn + sizeof(MEM_NODE);
        }
    }
#if GUI_DEBUG_MODE
    GUI_DEBUG_OUT("GUI alloc failed (heap overflow).");
#endif
    return NULL;
}

/**
 @ 释放内存, 外部调用.
 @ pl: 需要释放的堆节点地址.
 @ Mem: 内存池指针.
 **/
void GUI_Free(void *Ptr, void *Mem)
{
    if (Ptr) {
        Ptr = (char *)Ptr - sizeof(MEM_NODE);
        ((MEM_HEAP *)Mem)->Usage -= ((MEM_NODE *)Ptr)->Size;
        __free(Ptr);
    }
}

/**
 @ 获取内存池已使用字节数.
 @ Mem: 内存池地址.
 @ 返回值: 内存池已使用字节数.
 **/
size_t MemGetUsageBytes(void *Mem)
{
    return ((MEM_HEAP*)Mem)->Usage;
}

/**
 @ 获取内存池容量(单位为Byte).
 @ Mem: 内存池地址.
 @ 返回值: 内存池容量(单位为Byte).
 **/
size_t MemGetSizeBytes(void *Mem)
{
    return ((MEM_HEAP*)Mem)->Size - (sizeof(MEM_HEAP) - sizeof(MEM_NODE));
}
