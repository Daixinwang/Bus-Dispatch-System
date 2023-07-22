#include <stdlib.h>
#include "list.h"

ListNode* NewListNode(int command, int arg, int time, ListNode* next) {
    ListNode* node = (ListNode*)malloc(sizeof(ListNode));
    node->command = command;
    node->arg = arg;
    node->time = time;
    node->next = next;
    return node;
}

void FreeListNode(ListNode* node) {
    free(node);
}

