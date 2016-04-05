#ifndef __LINKED_LIST_H
#define __LINKED_LIST_H

#include "GUI_Typedef.h"

typedef struct NODE NODE;
typedef struct NODE * LIST;

struct NODE {
    void * pData;
    LIST pNext;
    int DataLen;
};

LIST List_Init(void);
void List_Delete(LIST L);
GUI_RESULT List_InsertNode(LIST pl, void * x, int len);
GUI_RESULT List_InsertEnd(LIST pl, void * x, int len);
void List_DeleteNode(LIST pl, LIST pt);
int List_GetSize(LIST pl);
LIST List_GetNodePtr(LIST pl, int n);
void * List_GetNodeData(LIST pl, int n);
int List_GetNodeIndex(LIST pl, LIST pt);
int List_FindStr(LIST pl, char *pd);

#endif /* __LINKED_LIST_H */
