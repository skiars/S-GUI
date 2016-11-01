#include "GUI_Malloc.h"
#include "GUI_Core.h"

static MEM_HEAP *__HeapList;   /* 堆链表 */
static size_t __SizeBytes;     /* 总容量 */
static size_t __UsageBytes;    /* 总使用量 */
#if GUI_USE_MEM_LOCK
static void * __LockPtr;       /* 互斥锁 */
#endif

static void _ListInsert(MEM_HEAP *heap)
{
    MEM_HEAP *p = __HeapList;

    if (p) {
        while (p->pNext) {
            p = p->pNext;
        }
        p->pNext = heap;
    } else {
        __HeapList = heap;
    }
}

/**
 @ 内存池初始化.
 @ Mem: 内存池地址(建议这个地址是4字节对齐的, 以便能满足特殊需求).
 @ Size: 内存池大小(Byte).
 @ 返回值: GUI_OK: 内存池初始化成功, GUI_ERR: 内存池初始化失败.
 **/
static GUI_RESULT _HeapInit(void *Mem, size_t Size)
{
    MEM_HEAP *mPool;

    if (!Mem || Size < sizeof(MEM_HEAP)) {
        return GUI_ERR;
    }
    mPool = Mem;
    mPool->Size = Size;
    mPool->pNext = NULL;
    mPool->MemPool.Tag = 0;
    mPool->MemPool.pLast = NULL;
    mPool->MemPool.pNext = NULL;
    mPool->MemPool.Size = Size - (sizeof(MEM_HEAP) - sizeof(MEM_NODE));
    _ListInsert(mPool); /* 添加到链表 */
    __SizeBytes += Size - (sizeof(MEM_HEAP) - sizeof(MEM_NODE));
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
        if (pl->Size > Size + (sizeof(MEM_NODE) << 1)) {
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

/* 内存管理初始化 */
GUI_RESULT GUI_MemoryManagementInit(void)
{
    int page = 0;
    u_32 size;
    void *p;
    GUI_RESULT res = GUI_ERR;

#if GUI_USE_MEM_LOCK
    __LockPtr = GUI_TaskCreateLock(); /* 创建互斥锁 */
#endif
    do {
        p = _GUI_GetHeapBuffer(page++, &size); /* 获取内存池 */
        if (_HeapInit(p, size) == GUI_OK) {
            res = GUI_OK;
        }
    } while (p != NULL);
#if GUI_DEBUG_MODE
    if (res == GUI_ERR) {
        GUI_DEBUG_OUT("GUI memory init faild, the memory pool is too small.");
    }
#endif
    return res;
}

/**
 @ 申请内存, 外部调用.
 @ Size: 要申请的字节数.
 @ Mem: 内存池地址.
 @ 返回值: 申请到空间的首地址.
 **/
void * GUI_Malloc(size_t Size)
{
    MEM_NODE *pn = NULL;
    MEM_HEAP *ph;

    if (Size) {
        if (Size & 0x03) { /* 4字节对齐 */
            Size += 4 - (Size & 0x03);
        }
#if GUI_USE_MEM_LOCK
        GUI_TaskLock(__LockPtr); /* 上锁 */
#endif
        /* 遍历所有的堆 */
        for (ph = __HeapList; ph && !pn; ph = ph->pNext) {
            pn = __alloc(&ph->MemPool, Size);
        }
#if GUI_USE_MEM_LOCK
        GUI_TaskUnlock(__LockPtr); /* 解锁 */
#endif
        if (pn) {
            __UsageBytes += pn->Size;
            return (void *)((u_32)pn + sizeof(MEM_NODE));
        }
#if GUI_DEBUG_MODE
        GUI_DEBUG_OUT("GUI alloc failed (heap overflow).");
#endif
    }
    return pn;
}

/**
 @ 释放内存, 外部调用.
 @ pl: 需要释放的堆节点地址.
 @ Mem: 内存池指针.
 **/
void GUI_Free(void *Ptr)
{
    size_t addr1 = (size_t)Ptr - sizeof(MEM_NODE), addr2;
    MEM_HEAP *ph;

    if (Ptr) {
#if GUI_USE_MEM_LOCK
        GUI_TaskLock(__LockPtr); /* 上锁 */
#endif
        for (ph = __HeapList; ph; ph = ph->pNext) {
            addr2 = (size_t)ph;
            if (addr1 > addr2 && addr1 < addr2 + ph->Size) {
                __UsageBytes -= ((MEM_NODE *)addr1)->Size;
                __free((MEM_NODE *)addr1);
                break;
            }
        }
#if GUI_USE_MEM_LOCK
        GUI_TaskUnlock(__LockPtr); /* 解锁 */
#endif
#if GUI_DEBUG_MODE
        if (ph == NULL) {
            GUI_DEBUG_OUT("GUI free filed (pointer is invalid).");
        }
#endif
    }
}

/**
 @ 获取内存池已使用字节数.
 @ Mem: 内存池地址.
 @ 返回值: 内存池已使用字节数.
 **/
size_t GUI_GetMemUsage(void)
{
    return __UsageBytes;
}

/**
 @ 获取内存池容量(单位为Byte).
 @ Mem: 内存池地址.
 @ 返回值: 内存池容量(单位为Byte).
 **/
size_t GUI_GetMemSize(void)
{
    return __SizeBytes;
}
