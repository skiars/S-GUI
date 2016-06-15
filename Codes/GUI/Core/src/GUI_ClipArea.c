#include "GUI_ClipArea.h"
#include "GUI.h"

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

    Rect = WM_GetWindowAreaRect(hWin);
    /* 在考虑遮挡之前,窗口就只有一个裁剪矩形 */
    Area = GUI_GetRectList(1);
    if (Area) { /* 当新建窗口的顶部还有窗口时存在Bug */
        Area->Rect = Rect;
        pWin->ClipArea = Area;
    }
    /* 从窗口的父亲开始计算。 */
    for (pWin = pWin->hParent; pWin && pWin != hWin; pWin = pWin->hNextLine) {
        if (!(pWin->Status & WM_WS_TRANS)) {
            /* 在窗口原来剪切域的基础上排除新窗口的矩形 */
            pWin->ClipArea = GUI_ClipExcludeRect(pWin->ClipArea, &Rect);
        }
    }
}

/**
 @ 计算指定窗口及其上面窗口的剪切域.
 @ hWin:开始计算剪切域的窗口句柄.
 @ hEnd:最后一个要计算剪切域的窗口.
 **/
void GUI_WindowsClipArea(GUI_HWIN hWin, GUI_HWIN hEnd)
{
    WM_Obj *pWin = hWin;

    for (pWin = pWin->hParent; pWin; pWin = pWin->hNextLine) {
        GUI_FreeRectList(pWin->ClipArea);
        GUI_ClipNewWindow(pWin);
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
 @ 计算窗口剪切域.
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
            GUI_ClipNewWindow(pWin);
        }
    }
    GUI_UNLOCK();
}
