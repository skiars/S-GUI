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

/**
 @ GUI在添加一个窗口时更新剪切域.
 @ hWin:添加的新窗口句柄.
 **/
void GUI_ClipNewWindow(GUI_HWIN hWin)
{
    WM_Obj *pWin = hWin;
    GUI_RECT Rect;
    GUI_AREA Area;

    /* 透明窗口直接返回 */
    if (pWin->Status & WM_WS_TRANS) {
        pWin->ClipArea = NULL;
        return;
    }
    WM_GetWindowAreaRect(hWin, &Rect);
    /* 在考虑遮挡之前,窗口就只有一个裁剪矩形 */
    Area = GUI_GetRectList(1);
    if (Area) { /* 当新建窗口的顶部还有窗口时存在Bug */
        Area->Rect = Rect;
        pWin->ClipArea = Area;
    }
    do {
        pWin = pWin->hParent; /* 跳过透明的祖先 */
    } while (pWin && pWin->Status & WM_WS_TRANS);
    /* 从窗口的父亲开始计算。 */
    while (pWin && pWin != hWin) {
        if (!(pWin->Status & WM_WS_TRANS)) {
            /* 在窗口原来剪切域的基础上排除新窗口的矩形 */
            pWin->ClipArea = GUI_ClipExcludeRect(pWin->ClipArea, &Rect);
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
    } while (pWin && pWin->Status & WM_WS_TRANS);
    while (pWin) {
        GUI_FreeRectList(pWin->ClipArea);
        if (!(pWin->Status & WM_WS_TRANS)) {
            GUI_ClipNewWindow(pWin);
        } else {
            pWin->ClipArea = NULL; /* 透明窗口不计算剪切域  */
        }
        pWin = pWin->hNextLine;
    }
}

/**
 @ 删除窗口的剪切域.
 **/
void GUI_DeleteWindowClipArea(GUI_HWIN hWin)
{
    GUI_FreeRectList(((WM_Obj *)hWin)->ClipArea);
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
        /* 先计算到hWin之前的剪切域 */
        for (pWin = pWin->hParent; pWin != pEnd; pWin = pWin->hNextLine) {
            GUI_FreeRectList(pWin->ClipArea);
            if (!(pWin->Status & WM_WS_TRANS)) {
                GUI_ClipNewWindow(pWin);
            } else {
                pWin->ClipArea = NULL; /* 透明窗口不计算剪切域  */
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
        if (pWin->Status & WM_WS_TRANS) {
            return GUI_CurrentClipArea();
        }
        return pWin->ClipArea;
    }
    return NULL;
}
