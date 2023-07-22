#include <stdio.h>
#include <stdlib.h>
#include "queue.h"

Queue* NewQueue() {
    Queue* queue = (Queue*)malloc(sizeof(Queue));
    queue->front = queue->rear = NULL;
    queue->count = 0;
    return queue;
}

void Enqueue(Queue* queue, int value) {
    QueueNode* node = (QueueNode*)malloc(sizeof(QueueNode));
    node->data = value;
    node->next = NULL;
    if (queue->rear == NULL) {
        queue->front = node;
        queue->rear = node;
    }
    else {
        queue->rear->next = node;
        queue->rear = node;
    }
    queue->count++;
}

int Dequeue(Queue* queue) {
    if (queue->front == NULL) {
        printf("Error: queue is empty!");
        exit(-1);
    }
    QueueNode* node = queue->front;
    int value = node->data;
    if (queue->front == queue->rear) {
        queue->front = NULL;
        queue->rear = NULL;
    }
    else {
        queue->front = queue->front->next;
    }
    free(node);
    queue->count--;
    return value;
}

int SizeQueue(Queue* queue) {
    return queue->count;
}

int FrontQueue(Queue* queue) {
    if (queue->front == NULL) {
        printf("Error: queue is empty!");
        exit(-1);
    }
    return queue->front->data;
}

