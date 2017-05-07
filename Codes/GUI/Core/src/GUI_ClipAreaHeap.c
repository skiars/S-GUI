#include "GUI_ClipAreaHeap.h"
#include "GUI_Malloc.h"
#include "GUI_Rect.h"

/* 私有变量 */
static GUI_AREA GUI_AreaHeap;       /* 裁剪区域堆 */

/* 获取一个窗口裁剪堆 */
static GUI_AREA _GetRectList(int num)
{
    GUI_AREA pNode, pList;

    if (!num) {
        return NULL;
    }
    /* 申请内存(包括一个表头) */
    pList = GUI_Malloc(sizeof(AREA_NODE));
    for (pNode = pList; --num && pNode; pNode = pNode->pNext) {
        pNode->pNext = GUI_Malloc(sizeof(AREA_NODE));
    }
    if (pNode == NULL) {
        /* 释放链表 */
        for (pNode = pList; pNode; pNode = pNode->pNext) {
            GUI_Free(pNode);
        }
        return NULL;
    } else {
        pNode->pNext = NULL;
    }
    return pList;
}

/* 窗口裁剪矩形区域私有堆初始化 */
GUI_RESULT GUI_RectListInit(void)
{
    GUI_AreaHeap = _GetRectList(1);
    if (GUI_AreaHeap) {
        return GUI_OK;
    }
    return GUI_ERR;
}

/* 释放空闲的剪切域堆 */
void GUI_FreeIdleRectList(void)
{
    GUI_AREA pNode, pNext;

    pNode = GUI_AreaHeap->pNext;
    while (pNode) {
        pNext = pNode->pNext;
        GUI_Free(pNode);
        pNode = pNext;
    }
    GUI_AreaHeap->pNext = NULL;
}

/* 申请一个裁剪矩形链表 */
GUI_AREA GUI_GetRectList(int num)
{
    int n = num;
    GUI_AREA pNode, pLast, Area = GUI_AreaHeap;

    if (!num && !Area) {
        return NULL;
    }
    pNode = Area;
    pLast = Area;
ReTest:
    while (pNode && n--) {
        pLast = pNode;
        pNode = pNode->pNext;
    }
    if (pNode == NULL) { /* 容量不够 */
        pLast->pNext = _GetRectList(n + 1);
        pNode = pLast->pNext;
        goto ReTest;
    }
    Area = Area->pNext;
    GUI_AreaHeap->pNext = pNode->pNext;
    pNode->pNext = NULL;
    return Area;
}

/* 释放一个裁剪矩形链表 */
GUI_RESULT GUI_FreeRectList(GUI_AREA Area)
{
    GUI_AREA p;

    if (Area == NULL) {
        return GUI_ERR;
    }
    p = GUI_AreaHeap->pNext;
    GUI_AreaHeap->pNext = Area; /* 插入到链表的最前面 */
    while (Area->pNext) {
        Area = Area->pNext;
    }
    Area->pNext = p;
    return GUI_OK;
}

/* 释放一个剪切域节点,注意Area不能为NULL */
void GUI_FreeRectListNode(GUI_AREA Area)
{
    GUI_AREA p;

    p = GUI_AreaHeap->pNext;
    GUI_AreaHeap->pNext = Area; /* 插入到链表的最前面 */
    Area->pNext = p;
}

/* 将矩形Src用矩形Dst去裁剪
 @ -Src与Dst必须是有效地矩形.
 **/
GUI_AREA GUI_RectCut(GUI_RECT *Src, GUI_RECT *Dst)
{
    u_8 n = 0;
    GUI_RECT r;
    GUI_AREA p;

    GUI_AREA Area;

    if (GUI_RectOverlay(&r, Src, Dst) == FALSE) { /* 判断是否相交 */
        /* 不相交，裁剪区域就是Src矩形自己 */
        Area = GUI_GetRectList(1); /* 申请链表 */
        if (Area) {
            Area->Rect = *Src;
        }
        return Area;
    }
    /* Src完全被Src与Dst的相交部分遮挡,将不会有裁剪区域 */
    if (Src->x0 >= r.x0 && Src->x1 <= r.x1
        && Src->y0 >= r.y0 && Src->y1 <= r.y1) {
        return NULL;
    }
    /* 每一个矩形最多可以被另外一个矩形分割为4个 */
    Area = GUI_GetRectList(4); /* 申请链表 */
    if (Area == NULL) { /* 申请失败 */
        return NULL;
    }
    p = Area;
    /* 逐个计算裁剪区域 */
    if (Src->y0 < r.y0) {
        p->Rect.x0 = Src->x0;
        p->Rect.y0 = Src->y0;
        p->Rect.x1 = Src->x1;
        p->Rect.y1 = r.y0 - 1;
        p = p->pNext;
        ++n;
    }
    if (r.y1 < Src->y1) {
        p->Rect.x0 = Src->x0;
        p->Rect.y0 = r.y1 + 1;
        p->Rect.x1 = Src->x1;
        p->Rect.y1 = Src->y1;
        p = p->pNext;
        ++n;
    }
    if (Src->x0 < r.x0) {
        p->Rect.x0 = Src->x0;
        p->Rect.y0 = r.y0;
        p->Rect.x1 = r.x0 - 1;
        p->Rect.y1 = r.y1;
        p = p->pNext;
        ++n;
    }
    if (r.x1 < Src->x1) {
        p->Rect.x0 = r.x1 + 1;
        p->Rect.y0 = r.y0;
        p->Rect.x1 = Src->x1;
        p->Rect.y1 = r.y1;
        p = p->pNext;
        ++n;
    }
    GUI_FreeRectList(p); /* 释放多余的链表 */
    if (n) { /* 当裁剪次数不为0时设置链表尾 */
        for (p = Area; --n; p = p->pNext);
        p->pNext = NULL;
        return Area;
    }
    /* 如果裁剪次数为0，说明两Src被遮挡，则返回值为NULL */
    /* bug标记.此处不应该被执行到，因为前面已经判定了遮挡 */
    return NULL;
}