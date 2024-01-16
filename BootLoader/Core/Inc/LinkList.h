#ifndef __LINKLIST_H_
#define __LINKLIST_H_

/*LinkList創建*/
//節點創建
typedef struct Node {
    uint32_t data;
    struct Node* next;
} Node;
//鍊表創建
typedef struct {
    Node* head;
    Node* tail;
} LinkedList;

// 全局链表变量
extern LinkedList dataBuffer;

/*variable*/

/*function*/

//Linklist 初始化 main 函數調用
void initLinkedList(LinkedList *list);
//添加節點
void appendNode(LinkedList *list, uint32_t data);
//处理接收到的数据 Uart RX 回調函數調用
void processReceivedData(LinkedList *list);
//移出節點
void removeNode(LinkedList *list, Node *targetNode);




















#endif