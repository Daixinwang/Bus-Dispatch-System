#ifndef BUS_SCHEDULE_QUEUE_H
#define BUS_SCHEDULE_QUEUE_H

#include "const.h"

typedef struct QNode {
    int data;
    struct QNode* next;
} QueueNode;

typedef struct {
    QueueNode* front;
    QueueNode* rear;
    int count;
} Queue;

Queue* NewQueue();
void Enqueue(Queue* queue, int data);
int Dequeue(Queue* queue);
int FrontQueue(Queue* queue);
int SizeQueue(Queue* queue);

#endif //BUS_SCHEDULE_QUEUE_H
