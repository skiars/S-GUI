#ifndef __LINKED_LIST_H
#define __LINKED_LIST_H

#define ADT_ERR 1
#define ADT_OK  0

typedef unsigned short u_16;

typedef void *PData;
typedef struct Node Node;
typedef struct Node *PNode;
typedef PNode List;

struct Node{
    PData data;
    PNode next;
    u_16 datalen;
};

List ListInit(u_16 len);//创建一个链表，这个链表有一个空表头
u_16 ListLen(List pl);//获取链表长度
PData List_GetNodeData(List pl,u_16 node_num);//获取第node_num个链节的数据
u_16 GetNodeNum(List pl,PNode pt);  //获取链表pl中pt指向的链节数
char InsertNum(PData x,List pl,u_16 num);//插入到第num个节点后
char DeleteNum(List pl,u_16 num);//删除第num个节点
void DeleteList(List L);//删除链表
u_16 FindNodeStr(List pl,char* pd);//查找字符串节点，仅当链表节点为字符串时可以使用
void Insert(PData x,PNode pl);
PNode GetNodePtr(List pl,u_16 node_num);

#endif /*__LINKED_LIST_H*/
