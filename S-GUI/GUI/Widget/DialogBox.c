#include "DialogBox.h"
#include "GUI.h"

/*
 *创建对话框
 *pWinfo,自动创建窗口数据
 *NumWidgets,自动创建的子窗口数量
 *cb,事件响应回调函数
 *hParent,父窗口句柄,NULL表示没有父窗口
 *x0,起始x坐标
 *y0,起始y坐标
 */
GUI_hWin GUI_CreateDialogBox(GUI_WIDGET_CREATE_INFO *pWinfo,
                             u_16 NumWidgets,
                             WM_CALLBACK *cb,
                             WM_hWin hParent,
                             i_16 x0,
                             i_16 y0)
{
#if 0
    u_16 i, xSize, ySize;
    GUI_hWin hWin;
    
    x0 += pWinfo[0].x0;
    y0 += pWinfo[0].y0;
    xSize = pWinfo[0].xSize;
    ySize = pWinfo[0].ySize;
    hWin = pWinfo[0].Auto_Create(x0, y0, xSize, ySize, pWinfo[i].Cmd);
    /* 关联子窗口 */
    for (i = 0; i < NumWidgets; i++) {
        
    }
    return hWin;

    //u_16 i, xSize, ySize;
    GUI_hWin hWin;

    /* 创建对话框 */
    WM_CreateWindowAsChild(x0, y0, 0, 0, hParent, 0, 0, 0, cb,0);
    x0 += pWinfo[0].x0;
    y0 += pWinfo[0].y0;
    xSize = pWinfo[0].xSize;
    ySize = pWinfo[0].ySize;
    return hWin;
#endif
    return NULL;
}

/*
*删除对话框
*/
void GUI_DeleteDialogBox(GUI_hWin hWin)
{
    
}
