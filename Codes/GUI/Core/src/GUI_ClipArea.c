#include "GUI_ClipArea.h"
#include "GUI.h"

/* 注意: 目前支持透明窗口的剪切域计算逻辑尚未实现. */

/**
 @ 剪切域排除一个矩形.
 @ Area:要被修改的剪切域.
 @ Rect:将要排除的矩形.
 @ 返回值:排除矩形后的剪切域.
 **/
GUI_AREA GUI_ClipExcludeRect(GUI_AREA Area, GUI_RECT *Rect)
{
    GUI_AREA pNode, pDst = NULL, pNext = NULL, p;

    /* 获得链表头,直到获得非空的指针(链表头) */
    for (pNode = Area; !pDst && pNode; pNode = pNext) {
        /* 计算第一个剪切域 */
        pDst = GUI_RectCut(&pNode->Rect, Rect);
        pNext = pNode->pNext;
        GUI_FreeRectListNode(pNode); /* 释放节点 */
    }
    if (pDst != NULL) {
        p = pDst;
        for (; pNode; pNode = pNext) {
            for (; p->pNext; p = p->pNext); /* 到链表尾 */
            p->pNext = GUI_RectCut(&pNode->Rect, Rect); /* 计算剪切域并连接链表 */
            pNext = pNode->pNext;
            GUI_FreeRectListNode(pNode); /* 释放节点 */
        }
    }
    return pDst;
}

/* 裁剪透明窗口的子窗口 */
static GUI_AREA _ClipTransChildren(GUI_AREA L, WM_Obj *pWin)
{
    for (pWin = pWin->hFirstChild; pWin && L; pWin = pWin->hNext) {
        if (pWin->status & WM_WS_TRANS) {
            L = _ClipTransChildren(L, pWin); /* 裁剪子窗口 */
        } else {
            L = GUI_ClipExcludeRect(L, &pWin->rect);  /* 裁剪透明窗口的孩子 */
        }
    }
    return L;
}

/* 裁剪一个窗口 */
GUI_AREA GUI_ClipOneWindow(GUI_HWIN hWin)
{
    WM_Obj *pWin = hWin, *pObj;
    GUI_RECT r;
    GUI_AREA Area;

    /* 在考虑遮挡之前,窗口就只有一个裁剪矩形 */
    Area = GUI_GetRectList(1);
    if (Area) {
        WM_GetWindowAreaRect(pWin, &r);;
        Area->Rect = r;
    }
    /* 窗口会被它的儿子们或者右边的兄弟们(如果有的话)裁剪,也就是遮挡，遍历它的孩子和兄弟们，
       逐个计算窗口的裁剪矩形链表，最后就能得到这个窗口被它们遮挡后的裁剪矩形链表. */
    /* 再遍历它右边的同属窗口及祖先的同属窗口 */
    pObj = pWin;
    while (pObj != gui_rootwin && Area) {
        while (pObj->hNext && Area) {
            pObj = pObj->hNext; /* 向右遍历 */
            /* 如果是普通窗口就直接计算裁剪，如果是透明窗口就用它的孩子来计算裁剪 */
            if (pObj->status & WM_WS_TRANS) {
                Area = _ClipTransChildren(Area, pObj);
            } else {
                Area = GUI_ClipExcludeRect(Area, &pObj->rect);
            }
        }
        pObj = pObj->hParent; /* 向上遍历 */
    }
    if (pWin->hFirstChild && Area) {
        /* 先遍历子窗口 */
        pObj = pWin->hFirstChild;
        while (pObj && Area) {
            /* 如果是普通窗口就直接计算裁剪，如果是透明窗口就用它的孩子来计算裁剪 */
            if (pObj->status & WM_WS_TRANS) {
                Area = _ClipTransChildren(Area, pObj);
            } else {
                Area = GUI_ClipExcludeRect(Area, &pObj->rect);
            }
            pObj = pObj->hNext; /* 向右遍历 */
        }
    }
    return Area;
}

/**
 @ GUI在添加一个窗口时更新剪切域.
 @ hWin:添加的新窗口句柄.
 **/
void GUI_ClipNewWindow(GUI_HWIN hWin)
{
    WM_Obj *pWin = hWin;
    GUI_RECT Rect;

    /* 透明窗口直接返回 */
    if (pWin->status & WM_WS_TRANS) {
        pWin->clipArea = NULL;
        return;
    }
    /* 计算新窗口的剪切域 */
    pWin->clipArea = GUI_ClipOneWindow(hWin);
    WM_GetWindowAreaRect(hWin, &Rect);
    do {
        pWin = pWin->hParent; /* 跳过透明的祖先 */
    } while (pWin && pWin->status & WM_WS_TRANS);
    /* 从窗口的父亲开始计算。 */
    while (pWin && pWin != hWin) {
        if (!(pWin->status & WM_WS_TRANS)) {
            /* 在窗口原来剪切域的基础上排除新窗口的矩形 */
            pWin->clipArea = GUI_ClipExcludeRect(pWin->clipArea, &Rect);
        }
        pWin = pWin->hNextLine;
    }
}

/**
 @ 计算指定窗口及被其遮挡窗口的的剪切域.
 @ hWin:开始计算剪切域的窗口句柄.
 @ hEnd:最后一个要计算剪切域的窗口.
 **/
void GUI_WindowClipArea(GUI_HWIN hWin)
{
    WM_Obj *pWin = hWin;

    do {
        pWin = pWin->hParent; /* 跳过透明的祖先 */
    } while (pWin && pWin->status & WM_WS_TRANS);
    while (pWin) {
        GUI_FreeRectList(pWin->clipArea);
        if (!(pWin->status & WM_WS_TRANS)) {
            GUI_ClipNewWindow(pWin);
        } else {
            pWin->clipArea = NULL; /* 透明窗口不计算剪切域  */
        }
        pWin = pWin->hNextLine;
    }
}

/**
 @ 删除窗口的剪切域.
 **/
void GUI_DeleteWindowClipArea(GUI_HWIN hWin)
{
    GUI_FreeRectList(WM_HandleToPtr(hWin)->clipArea);
}

/**
 @ 计算窗口及它所有子窗口的剪切域.
 @ hWin:要移动的窗口.
 @ dx:x方向位移.
 @ dy:y方向位移
 **/
void GUI_ClipWindows(GUI_HWIN hWin)
{
    WM_Obj *pWin = hWin, *pEnd;

    GUI_LOCK();
    pEnd = WM_GetTopChildWindow(pWin);
    if (pEnd) {
        pEnd = pEnd->hNextLine;
        if (pWin->hParent) {
            pWin = pWin->hParent;
        }
        /* 先计算到hWin之前的剪切域 */
        for (; pWin != pEnd; pWin = pWin->hNextLine) {
            GUI_FreeRectList(pWin->clipArea);
            if (!(pWin->status & WM_WS_TRANS)) {
                GUI_ClipNewWindow(pWin);
            } else {
                pWin->clipArea = NULL; /* 透明窗口不计算剪切域  */
            }
        }
    }
    GUI_UNLOCK();
}

/* 获取窗口的剪切域 */
GUI_AREA GUI_GetWindowClipArea(GUI_HWIN hWin)
{
    WM_Obj *pWin = hWin;

    if (pWin) {
        /* 透明窗口直接返回上一个窗口用的剪切域 */
        if (pWin->status & WM_WS_TRANS) {
            return GUI_CurrentClipArea();
        }
        return pWin->clipArea;
    }
    return NULL;
}
