#ifndef BUS_SCHEDULE_LIST_H
#define BUS_SCHEDULE_LIST_H

typedef struct Node {
    int command;
    int arg;
    int time;
    struct Node* next;
} ListNode;

ListNode* NewListNode(int command, int arg, int time, ListNode* next);
void FreeListNode(ListNode* node);

#endif //BUS_SCHEDULE_LIST_H
