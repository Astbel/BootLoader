/*include function*/
#include "main.h"

// 申明鍊表
LinkedList dataBuffer;

/**
 * @brief
 *  初始化链表
 * @param list 鍊表
 */
void initLinkedList(LinkedList *list)
{
    list->head = NULL;
    list->tail = NULL;
}

/**
 * @brief
 * 在链表尾部添加节点
 * @param list 當前節點
 * @param data 數據
 */
void appendNode(LinkedList *list, uint32_t *data)
{
    Node *newNode = (Node *)malloc(sizeof(Node));
    newNode->data = *data; // 直接将 data 赋值给 newNode->data
    newNode->next = NULL;

    if (list->tail == NULL)
    {
        list->head = newNode;
        list->tail = newNode;
    }
    else
    {
        list->tail->next = newNode;
        list->tail = newNode;
    }
}

/**
 * @brief
 * 处理接收到的数据
 * @param list 當前節點
 */
void processReceivedData(LinkedList *list)
{
    Node *current = list->head;

    while (current != NULL)
    {
        // 在这里处理每个节点的数据（current->data）
        // 例如，将数据发送到其他地方或执行其他处理
        // 移动到下一个节点
        Node *next = current->next;
        free(current);
        current = next;
    }

    list->head = NULL;
    list->tail = NULL;
}

/**
 * @brief
 * 移出節點
 * @param list       鍊表選擇
 * @param targetNode 要移除的節點
 */
void removeNode(LinkedList *list, Node *targetNode)
{
    // 如果链表为空，直接返回
    if (list->head == NULL)
        return;

    // 如果目标节点是头节点
    if (targetNode == list->head)
        list->head = targetNode->next;
    else
    {
        // 否则，找到目标节点的前一个节点
        Node *current = list->head;
        while (current->next != NULL && current->next != targetNode)
            current = current->next;

        // 如果找到目标节点的前一个节点，将其 next 指向目标节点的下一个节点
        if (current->next != NULL)
            current->next = targetNode->next;
    }
    // 如果目标节点是尾节点，更新链表尾指针
    if (targetNode == list->tail)
        list->tail = targetNode->next;

    // 释放目标节点的内存
    free(targetNode);
}

/*Uart Rx CallBack 回調函示*/
/**
 * @brief
 * RX 回調函數當接收到時插入linklist
 * @param huart
 */
void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart)
{
    if (huart == pc_uart)
    {
        // 将接收到的数据添加到链表中
        appendNode(&dataBuffer, &Test_Buffer[0]);

        // 处理接收到的数据
        processReceivedData(&dataBuffer);

        // 准备下一次接收
        HAL_UART_Receive_IT(pc_uart, (uint8_t *)Test_Buffer, sizeof(Test_Buffer));
    }
}

/**
 * @brief 
 * 釋放節點數據
 * @param list 
 */
void freeLinkedList(LinkedList *list)
{
    Node *current = list->head;
    Node *next;

    while (current != NULL)
    {
        next = current->next;
        free((void*)current->data);  // 释放节点的 data
        free(current);        // 释放节点本身
        current = next;
    }

    list->head = NULL;
    list->tail = NULL;
}

