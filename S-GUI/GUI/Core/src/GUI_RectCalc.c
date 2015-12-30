#include "GUI_RectCalc.h"
#include "GUI.h"

/* 将表示矩形的坐标转换为结构体 */
GUI_RESULT GUI_Val2Rect(GUI_RECT *pDst,
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
        return GUI_OK;
    }
    return GUI_ERR;
}

/* 取两个矩形相交的部分,结果存储在*pDst里面,比GUI_RectAndCalc快 */
GUI_RESULT GUI_RectOverlay(GUI_RECT *pDst, GUI_RECT *a, GUI_RECT *b)
{
    /*  左上角的交点  */
    pDst->x0 = a->x0 > b->x0 ? a->x0 : b->x0;
    pDst->y0 = a->y0 > b->y0 ? a->y0 : b->y0;
    /*  右下角的交点  */
    pDst->x1 = a->x1 < b->x1 ? a->x1 : b->x1;
    pDst->y1 = a->y1 < b->y1 ? a->y1 : b->y1;
    
    if (pDst->x0 > pDst->x1 || pDst->y0 > pDst->y1) {
        return GUI_ERR; /* 两个矩形不相交 */
    }
    return GUI_OK;
}

/* 矩形与运算 */
GUI_RECT GUI_RectAndCalc(GUI_RECT *pRect1, GUI_RECT *pRect2)
{
    GUI_RECT Rect;
    
    /*  左上角的交点  */
    Rect.x0 = pRect1->x0 > pRect2->x0 ? pRect1->x0 : pRect2->x0;
    Rect.y0 = pRect1->y0 > pRect2->y0 ? pRect1->y0 : pRect2->y0;
    /*  右下角的交点  */
    Rect.x1 = pRect1->x1 < pRect2->x1 ? pRect1->x1 : pRect2->x1;
    Rect.y1 = pRect1->y1 < pRect2->y1 ? pRect1->y1 : pRect2->y1;
    return Rect;
}

/* 矩形或运算 */
GUI_RECT GUI_RectOrCalc(GUI_RECT *pRect1, GUI_RECT *pRect2)
{
    GUI_RECT Rect;
    
    /*  左上角的开始点  */
    Rect.x0 = pRect1->x0 < pRect2->x0 ? pRect1->x0 : pRect2->x0;
    Rect.y0 = pRect1->y0 < pRect2->y0 ? pRect1->y0 : pRect2->y0;
    /*  右下角的结束点  */
    Rect.x1 = pRect1->x1 > pRect2->x1 ? pRect1->x1 : pRect2->x1;
    Rect.y1 = pRect1->y1 > pRect2->y1 ? pRect1->y1 : pRect2->y1;
    return Rect;
}

/* 检查一个点是否在一个矩形内 */
u_8 GUI_CheckPointAtRect(u_16 x, u_16 y, GUI_RECT *Rect)
{
    if (x >= Rect->x0 && x <= Rect->x1 &&
        y >= Rect->y0 && y <= Rect->y1) {
        return 1;
    }
    return 0;
}

/* 检查一个矩形是否为空 */
u_8 GUI_CheckRectNull(GUI_RECT *Rect)
{
    if (Rect->x1 >= Rect->x0 && Rect->y1 >= Rect->y0) {
        return 1;  /* 非空 */
    }
    return 0;
}

/* 检查两个矩形是否相交*/
u_8 GUI_CheckRectIntersect(GUI_RECT *pRect1, GUI_RECT *pRect2)
{
    if (pRect1->x0 > pRect2->x1 || pRect1->y0 > pRect2->y1
     || pRect1->x1 < pRect2->x0 || pRect1->y1 < pRect2->y0) {
        return 0;    /* 不相交 */
    }
    return 1;/* 相交 */
}

void GUI_MoveRect(GUI_RECT *Rect, i_16 dX, i_16 dY)
{
    Rect->x0 += dX;
    Rect->x1 += dX;
    Rect->y0 += dY;
    Rect->y1 += dY;
}

/* 窗口裁剪矩形区域私有堆初始化 */
u_8 GUI_RectListInit(u_16 num)
{
    RECT_NODE *pNode;

    /* 申请内存(包括一个表头) */
    ++num;
    GUI_Data->RectList = GUI_fastmalloc(sizeof(RECT_NODE) * (u_32)num);
    if (GUI_Data->RectList == NULL) {
        return 1;
    }
    pNode = GUI_Data->RectList;
    while (--num) { /* 关联链表pNext */
        pNode->pNext = pNode + 1;
        ++pNode;
    }
    pNode->pNext = NULL;
    return 0;
}

/* 申请一个裁剪矩形链表 */
RECT_LIST GUI_GetRectList(u_16 num)
{
    RECT_NODE *pNode;
    RECT_LIST List = GUI_Data->RectList;

    if (!num) {
        return NULL;
    }
    pNode = List;
    while (pNode && num--) {
        pNode = pNode->pNext;
    }
    if (pNode == NULL) { /* 容量不够 */
        return NULL;
    }
    List = List->pNext;
    GUI_Data->RectList->pNext = pNode->pNext;
    pNode->pNext = NULL;
    return List;
}

/* 释放一个裁剪矩形链表 */
GUI_RESULT GUI_FreeRectList(RECT_NODE *pNode)
{
    RECT_NODE *p;
    
    if (pNode == NULL) {
        return GUI_ERR;
    }
    p = GUI_Data->RectList->pNext;
    GUI_Data->RectList->pNext = pNode; /* 插入到链表的最前面 */
    while (pNode->pNext) {
        pNode = pNode->pNext;
    }
    pNode->pNext = p;
    return GUI_OK;
}

/* 裁剪一个矩形 
 * 算法有待优化
 */
RECT_LIST GUI_RectCut(GUI_RECT *Src, GUI_RECT *Dst)
{
    u_8 i, j, t[4];
    GUI_RECT Rect[4], r;
    RECT_NODE *pNode;
    RECT_LIST List;

    if (GUI_RectOverlay(&r, Src, Dst) == GUI_ERR) { /* 判断是否相交 */
        List = GUI_GetRectList(1); /* 申请链表 */
        if (List) {
            List->Rect = *Src;
        }
        return List;
    }
    /* 每一个矩形最多可以被另外一个矩形分割为4个 */
    Rect[0].x0 = Src->x0;
    Rect[0].y0 = Src->y0;
    Rect[0].x1 = Src->x1;
    Rect[0].y1 = r.y0 - 1;

    Rect[1].x0 = Src->x0;
    Rect[1].y0 = r.y1 + 1;
    Rect[1].x1 = Src->x1;
    Rect[1].y1 = Src->y1;

    Rect[2].x0 = Src->x0;
    Rect[2].y0 = r.y0;
    Rect[2].x1 = r.x0 - 1;
    Rect[2].y1 = r.y1;

    Rect[3].x0 = r.x1 + 1;
    Rect[3].y0 = r.y0;
    Rect[3].x1 = Src->x1;
    Rect[3].y1 = r.y1;
    
    /* 统计有效矩形数量 */
    for (i = 0, j = 0; i < 4; ++i) {
        if (Rect[i].x1 >= Rect[i].x0 && Rect[i].y1 >= Rect[i].y0) {
            ++j;
            t[i] = 1; /* 标记有效矩形 */
        } else {
            t[i] = 0; /* 标记无效矩形 */
        }
    }
    List = GUI_GetRectList(j); /* 申请链表 */
    if (List == NULL) {
        return NULL;
    }
    pNode = List;
    for (i = 0; i < 4; ++i) {
        if (t[i]) { /* 将有效的矩形赋值给链表成员 */
            pNode->Rect = Rect[i];
            pNode = pNode->pNext;
        }
    }
    return List;
}

/* 为一个裁剪矩形链表重新计算裁剪矩形链表 */
RECT_LIST GUI_ReCalcRectList(RECT_LIST List, GUI_RECT *Rect)
{
    RECT_NODE *pNode;
    RECT_LIST p = NULL, q;

    /* 获得链表头,直到获得非空的指针(链表头) */
    for (q = List; !p && q; q = q->pNext) {
        p = GUI_RectCut(&q->Rect, Rect);
    }
    if (p != NULL) {
        pNode = p;
        for (; q; q = q->pNext) { /* 裁剪剩下的矩形 */
            while (pNode->pNext) { /* 直到最后一个链节 */
                pNode = pNode->pNext;
            }
            pNode->pNext = GUI_RectCut(&q->Rect, Rect); /* 连接链表 */
        }
    }
    GUI_FreeRectList(List); /* 释放原来的链表 */
    return p;
}
