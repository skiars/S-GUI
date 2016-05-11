#include "linked_list.h"
#include "GUI.h"

#define MALLOC(x)  GUI_fastmalloc(x)
#define FREE(x)    GUI_fastfree(x);

/* 查找链表pl中节点pt的前驱 */
static LIST FindPrecious(LIST pl, LIST pt)
{
    LIST p;
    p = pl;

    while (p->pNext != NULL && p->pNext != pt) {
        p = p->pNext;
    }
    return p;
}

/* 删除pt指向的节点 */
void List_DeleteNode(LIST pl, LIST pt)
{
    LIST p;

    if (pl && pt) {
        p = FindPrecious(pl, pt);
        if (pl != pt) {
            p->pNext = pt->pNext;
            if (pt->DataLen) { /* 不是指针链表 */
                FREE(pt->pData);
            }
            FREE(pt);
        }
    }
}

/* 将x指向的数据插入到pl指向的节点后 */
GUI_RESULT List_InsertNode(LIST pl, void * x, int len)
{
    LIST l;

    if (pl == NULL) {
        return GUI_ERR;
    }
    l = MALLOC(sizeof(NODE));
    if (l == NULL) {
        return GUI_ERR;
    }
    if (len) { /* 申请空间 */
        l->pData = MALLOC(len);
        if (l->pData == NULL) {
            FREE(l);
            return GUI_ERR;
        }
        while (len--) { /* 复制内容 */
            ((char *)(l->pData))[len] = ((char *)x)[len];
        }
    } else { /* 指针链表 */
        l->pData = x;
    }
    l->DataLen = len;
    l->pNext = pl->pNext;
    pl->pNext = l;
    return GUI_OK;
}

/* 将一个节点插入到链表末尾 */
GUI_RESULT List_InsertEnd(LIST pl, void * x, int len)
{
    if (pl == NULL) {
        return GUI_ERR;
    }
    while (pl->pNext) {
        pl = pl->pNext;
    }
    return List_InsertNode(pl, x, len);
}

/* 创建一个链表，这个链表有一个空表头 */
LIST List_Init(void)
{
    LIST Front;

    Front = MALLOC(sizeof(NODE));
    Front->pData = NULL;
    if (Front == NULL) {
        return NULL;
    }
    Front->DataLen = 0;
    Front->pNext = NULL;
    return Front;
}

/* 删除链表 */
void List_Delete(LIST L)
{
    LIST l;

    while (L != NULL) {
        l = L->pNext;
        if (L->DataLen) { /* 不是指针链表 */
            FREE(L->pData);
        }
        FREE(L);
        L = l;
    }
}

/* 获取链表长度 */
int List_GetSize(LIST pl)
{
    int i = 0;

    if (pl) {
        for (; pl->pNext != NULL; ++i) {
            pl = pl->pNext;
        }
    }
    return i;
}

/* 得到第n个链节的地址 */
LIST List_GetNodePtr(LIST pl, int n)
{
    int i;

    if (n > List_GetSize(pl)) {
        return NULL;
    }
    for (i = 0; i < n; ++i) {
        pl = pl->pNext;
    }
    return pl;
}

/* 获取第n个链节的数据指针 */
void * List_GetNodeData(LIST pl, int n)
{
    LIST p;

    p = List_GetNodePtr(pl, n);
    if (p != NULL) {
        return p->pData;
    } else {
        return NULL;
    }
}

/* 获取链表pl中pt指向的链节索引 */
int List_GetNodeIndex(LIST pl, LIST pt)
{
    int i = 0;

    if (pl) {
        for (; pl->pNext != pt; ++i) {
            pl = pl->pNext;
        }
    }
    return i;
}

/* 查找字符串节点，仅当链表节点为字符串时可以使用,不区分大小写 */
int List_FindStr(LIST pl, char *pd)
{
    int i = 1;

    if (pl) {
        while (pl->pNext != NULL) {
            pl = pl->pNext;
            if (GUI_Stricmp(pd, pl->pData) == 0) { /* 忽略大小写比较 */
                return i;
            }
            ++i;
        }
    }
    return 0;
}
