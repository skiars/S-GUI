#include "GUI_MemDev.h"
#include "GUI_WM.h"
#include "GUI_Rect.h"
#include "GUI_Malloc.h"
#include "GUI_Color.h"

static GUI_MEMDEV *_MemDevList, *_CurMemDev;

/* 创建内存设备 */
void GUI_MemDevCreate(GUI_HWIN hWin, u_8 PixelFormat)
{
    GUI_MEMDEV *pNode;
    GUI_RECT *r;
    u_8 BytesPerPixel = GUI_GetBytesPerPixel(PixelFormat);

    if (BytesPerPixel == 0) {
        return;
    }
    GUI_LOCK();
    for (pNode = _MemDevList; 
        pNode && pNode->hWin != hWin; pNode = pNode->pNext);
    if (pNode == NULL) {
        if (_MemDevList == NULL) {
            pNode = GUI_Malloc(sizeof(GUI_MEMDEV));
            _MemDevList = pNode;
        } else {
            pNode->pNext = GUI_Malloc(sizeof(GUI_MEMDEV));
            pNode = pNode->pNext;
        }
        pNode->pNext = NULL;
        pNode->hWin = hWin;
        ((WM_Obj *)hWin)->Status |= WM_WS_MEMDEV;
        r = WM_GetWindowRect(hWin);
        pNode->x0 = r->x0;
        pNode->y0 = r->y0;
        pNode->Width = r->x1 - r->x0 + 1;
        pNode->Height = r->y1 - r->y0 + 1;
        pNode->PixelFormat = PixelFormat;
        pNode->BytesPerPixel = BytesPerPixel;
        pNode->Alpha = 0;
        pNode->Pixels = GUI_Malloc(
            pNode->Width * pNode->Height * BytesPerPixel);
    }
    GUI_UNLOCK();
}

/*删除指定屏幕的内存设备 */
void GUI_MemDevDelete(GUI_HWIN hWin)
{
    GUI_MEMDEV *pNode, *pNext;

    GUI_LOCK();
    for (pNode = _MemDevList; pNode && pNode->pNext
        && pNode->pNext->hWin != hWin; pNode = pNode->pNext);
    if (pNode && pNode->pNext) {
        pNext = pNode->pNext->pNext;
        GUI_Free(pNode->pNext->Pixels);
        GUI_Free(pNode->pNext);
        pNode->pNext = pNext;
    }
    GUI_UNLOCK();
}

void GUI_SelectMemDev(GUI_HWIN hWin)
{
    if (((WM_Obj *)hWin)->Status & WM_WS_MEMDEV) {

    }
}
