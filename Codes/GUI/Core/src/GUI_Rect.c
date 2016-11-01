#include "GUI_Rect.h"
#include "GUI.h"

/* 将表示矩形的坐标转换为结构体 */
GUI_BOOL GUI_Val2Rect(GUI_RECT *pDst,
    i_16 x0,
    i_16 y0,
    u_16 xSize,
    u_16 ySize)
{
    pDst->x0 = x0;
    pDst->y0 = y0;
    pDst->x1 = x0 + xSize - 1;
    pDst->y1 = y0 + ySize - 1;
    if (xSize && ySize) {
        return TRUE;
    }
    return FALSE;
}

/* 取两个矩形相交的部分,结果存储在*pDst里面,比GUI_RectAndCalc()快 */
GUI_BOOL GUI_RectOverlay(GUI_RECT *pDst, GUI_RECT *a, GUI_RECT *b)
{
    /*  左上角的交点  */
    pDst->x0 = MAX(a->x0, b->x0);
    pDst->y0 = MAX(a->y0, b->y0);
    /*  右下角的交点  */
    pDst->x1 = MIN(a->x1, b->x1);
    pDst->y1 = MIN(a->y1, b->y1);
    
    if (pDst->x0 > pDst->x1 || pDst->y0 > pDst->y1) {
        return FALSE; /* 两个矩形不相交 */
    }
    return TRUE;
}

/* 取两个矩形并集,比GUI_RectOrCalc()快，a和b不能是NULL */
void GUI_RectSum(GUI_RECT *pDst, GUI_RECT *a, GUI_RECT *b)
{
    /*  左上角的起点  */
    pDst->x0 = MIN(a->x0, b->x0);
    pDst->y0 = MIN(a->y0, b->y0);
    /*  右下角的终点  */
    pDst->x1 = MAX(a->x1, b->x1);
    pDst->y1 = MAX(a->y1, b->y1);
}

/* 检查一个点是否在一个矩形内 */
GUI_BOOL GUI_CheckPointAtRect(i_16 x, i_16 y, GUI_RECT *Rect)
{
    if (x >= Rect->x0 && x <= Rect->x1 &&
        y >= Rect->y0 && y <= Rect->y1) {
        return TRUE;
    }
    return FALSE;
}

/* 检查一个矩形是否为空 */
GUI_BOOL GUI_CheckRectNull(GUI_RECT *Rect)
{
    if (Rect->x1 >= Rect->x0 && Rect->y1 >= Rect->y0) {
        return TRUE;  /* 非空 */
    }
    return FALSE;
}

/* 检查两个矩形是否相交*/
GUI_BOOL GUI_CheckRectIntersect(GUI_RECT *pRect1, GUI_RECT *pRect2)
{
    if (pRect1->x0 > pRect2->x1 || pRect1->y0 > pRect2->y1
     || pRect1->x1 < pRect2->x0 || pRect1->y1 < pRect2->y0) {
        return FALSE;    /* 不相交 */
    }
    return TRUE;/* 相交 */
}

/* 检查一个矩形是否包含另一个矩形 */
GUI_BOOL GUI_RectInclude(GUI_RECT *pSrc, GUI_RECT *pDst)
{
    if (pSrc->x0 <= pDst->x0 && pSrc->y0 <= pDst->y0
     && pSrc->x1 >= pDst->x1 && pSrc->y1 >= pDst->y1) {
        return TRUE;
    }
    return FALSE;
}

/* 移动一个矩形 */
void GUI_MoveRect(GUI_RECT *Rect, i_16 dX, i_16 dY)
{
    Rect->x0 += dX;
    Rect->x1 += dX;
    Rect->y0 += dY;
    Rect->y1 += dY;
}

/* 获取一个窗口裁剪堆 */
static GUI_AREA _GetRectList(u_16 num)
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
GUI_AREA GUI_GetRectList(u_16 num)
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
 * -Src与Dst必须是有效地矩形.
 */
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
