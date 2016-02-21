#include "GUI_KeyBoard.h"
#include "GUI.h"

/*
*¼üÅÌ»º³åÇø³õÊ¼»¯
*/
void GUI_KeyBufferInit(void)
{
    //GUI_Data->KeyQueue = GUI_EventQueueInit(KEY_BUFFER_NUM, 1);
}

/* É¾³ý¼üÅÌ¶ÓÁÐ */
void GUI_KeyBufferDelete(void)
{
    //GUI_EventQueueDelete(GUI_Data->KeyQueue);
}

/*
*°Ñ¼üÖµ´æ´¢µ½¼üÅÌ»º³åÆ÷
*/
void GUI_SendKeyMsg(u_8 Key)
{
    //GUI_SendEvent(GUI_Data->KeyQueue, (GUI_EVENT)&Key);
}

/*
*´Ó¼üÅÌ»º³åÆ÷¶ÁÈ¡¼üÖµ
*/
u_8 GUI_GetKey(void)
{
    //u_8 Key,err;

    //err = GUI_GetEvent(GUI_Data->KeyQueue, (GUI_EVENT)&Key);
    //if (err == GUI_ERR)  return 0;
    //return Key;
    return 0;
}

/* Çå¿Õ»º³å */
void GUI_CleanKeyBuffer(void)
{
    //GUI_CleanQueue(GUI_Data->KeyQueue);
}

/* °´¼ü´¦Àí */
void GUI_KeyProcess(void)
{
    /*
    GUI_HWIN hWin;
    
    if (!GUI_QueueIsEmpty(GUI_Data->KeyQueue)) {
        hWin = WM_GetActiveMainWindow();
        WM_SendMessage(hWin, WM_KEY_CHECKED, NULL);
    }
    */
}
