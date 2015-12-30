#include "Linked_List.h"
#include "stdio.h"
#include "string.h"
#include "mystring.h"

#include "GUI.h"

#define ADT_TEST 0

#define MALLOC(x)  GUI_malloc(x)
#define FREE(x)    GUI_free(x);

/* 将x指向的数据插入到pl指向的节点后 */
void Insert(PData x, PNode pl)
{
    PNode tmp;
    u_16 i, len;
    len = pl->datalen;
    tmp = MALLOC(sizeof(Node));
    if (len) {
        tmp->data = MALLOC(len);
    } else {
        tmp->data = x;
    } /* 指针链表 */
    if (tmp == NULL || tmp->data == NULL) {
#if ADT_TEST == 1
        printf("Out of space!\n");
        return;
#else
        /* Error code */

#endif
    }
    for (i = 0; i < len; i++) { /* 复制内容 */
        ((char *)(tmp->data))[i] = ((char *)x)[i];
    }
    tmp->datalen = len;
    tmp->next = pl->next;
    pl->next = tmp;
}

/* 查找链表pl中节点pt的前驱 */
static List FindPrecious(List pl, PNode pt)
{
    PNode p;
    p = pl;
    while (p->next != NULL && p->next != pt) {
        p = p->next;
    }
    return p;
}

/* 删除pt指向的节点,返回新节点的地址 */
static PNode Delete(List pl, PNode pt)
{
    PNode p;
    p = FindPrecious(pl, pt);
    if (pl != pt) {
        p->next = pt->next;
        if (pt->datalen) { /* 不是指针链表 */
            FREE(pt->data);
        }
        FREE(pt);
    }
    return p->next;
}

/* 创建一个链表，这个链表有一个空表头 */
List ListInit(u_16 len)
{
    PNode tmp;
    u_16 i;
    tmp = MALLOC(sizeof(List));
    if (len) { /* 不是指针链表 */
        tmp->data = MALLOC(len);
    }
    if (tmp == NULL || tmp->data == NULL) {
#if ADT_TEST == 1
        printf("Out of space!\n");
        return;
#else 
        /* Error code */

#endif
    }
    for (i = 0; i < len; i++) {
        ((char *)(tmp->data))[i] = 0;
    }
    tmp->datalen = len;
    tmp->next = NULL;
    return tmp;
}

/* 获取链表长度 */
u_16 ListLen(List pl)
{
    u_16 i;
    for (i = 0; pl->next != NULL; i++) {
        pl = pl->next;
    }
    return i;
}

/* 得到第node_num个链节的地址 */
PNode GetNodePtr(List pl, u_16 node_num)
{
    u_16 i;
    if (node_num > ListLen(pl)) {
        return NULL;
    }
    for (i = 0; i < node_num; i++) {
        pl = pl->next;
    }
    return pl;
}

/* 获取第node_num个链节的数据地址
 * 如果是指针链表则返回指针值
 **/
PData List_GetNodeData(List pl, u_16 node_num)
{
    PNode p;
    p = GetNodePtr(pl, node_num);
    if (p != NULL) {
        return p->data;
    } else {
        return NULL;
    }
}

/* 获取链表pl中pt指向的链节数 */
u_16 GetNodeNum(List pl, PNode pt)
{
    u_16 i;
    if (pl == pt) {
        return 0;
    }
    for (i = 1; pl->next != pt; i++) {
        pl = pl->next;
    }
    return i;
}

/* 插入到第num个节点后
 * num从1开始计数
 **/
char InsertNum(PData x, List pl, u_16 num)
{
    PNode p;
    if (num > ListLen(pl)) {
        return ADT_ERR;
    }
    p = GetNodePtr(pl, num);
    Insert(x, p);
    return ADT_OK;
}

/* 删除第num个节点 */
char DeleteNum(List pl, u_16 num)
{
    PNode p;
    if (num > ListLen(pl) || num == 0) {
        return ADT_ERR;
    }
    p = GetNodePtr(pl, num);
    Delete(pl, p);
    return ADT_OK;
}

/* 删除链表 */
void DeleteList(List L)
{
    PNode tmp;
    while (L != NULL) {
        tmp = L->next;
        if (L->datalen) { /* 不是指针链表 */
            FREE(L->data);
        }
        FREE(L);
        L = tmp;
    }
}

/* 查找字符串节点，仅当链表节点为字符串时可以使用,不区分大小写 */
u_16 FindNodeStr(List pl, char *pd)
{
    u_16 num;
    num = 1;
    while (pl->next != NULL) {
        pl = pl->next;
        if (mystricmp(pd, pl->data) == 0) { /* 忽略大小写比较 */
            return num;
        }
        num++;
    }
    return 0;
}

#if ADT_TEST==1
typedef struct {
    float a;
    char b;
} Data;

static void PrintfList(List pl)
{
    while (pl != NULL) {
        printf("%g\n", ((Data*)pl->data)->a);
        pl = pl->next;
    }
}

int main(void)
{
    List pl, pt;
    u_16 i;
    char status;
    Data a;
    Data *b;
    a.a = 1.33;
    a.b = 3;
    pl = ListInit(sizeof(Data));//创建链表

    for (i = 0; i < 20; i++) {
        status = InsertNum(&a, pl, i);
        if (status == ADT_ERR) {
            printf("Insert Error!\n");
        }
        a.a = a.a + 1;
    }
    PrintfList(pl);
    printf("ListLen=%d\n\n", ListLen(pl));

    for (i = 0; i < 20; i++) {
        status = DeleteNum(pl, 5);
        if (status == ADT_ERR) {
            printf("Delete Error!\n");
        }
        PrintfList(pl);
        printf("ListLen=%d\n\n", ListLen(pl));
    }

    b = List_GetNodeData(pl, 2);
    if (b != NULL) {
        printf("\nList Node2=%g\n", b->a);
        printf("\nNode 5 Num=%d\n", GetNodeNum(pl, GetNodePtr(pl, 5)));
        printf("sizeof(Node)=%d\n", sizeof(Node));
    } else {
        printf("Error:PData==NULL");
    }

    DeleteList(pl);

    pl = ListInit(100);//创建一个字符串链表，字符串长度最大为100字节

    InsertNum("Item0", pl, 0);
    InsertNum("Item1", pl, 0);
    InsertNum("Item2", pl, 1);
    printf((void*)List_GetNodeData(pl, 1));
    printf("\n");
    printf((void*)List_GetNodeData(pl, 2));
    printf("\n");
    printf((void*)List_GetNodeData(pl, 3));
    printf("\nItem2 is %d node.\n", FindNodeStr(pl, "Item2"));
    DeleteList(pl);
}
#endif


