#include<stdio.h>
#include<stdlib.h>
#include<string.h>

// 指令类型常量设置
#define CMD_CLOCK 0
#define CMD_TARGET 1
#define CMD_CLOCKWISE 2
#define CMD_COUNTERCLOCKWISE 3

// 策略类型常量设置
#define STRATEGY_FCFS 1
#define STRATEGY_SSTF 2
#define STRATEGY_SCAN 3

// 最大车站数量
#define SIZE_MAX_STATION 25

typedef struct QNode {
    int data;
    struct QNode *next;
} QueueNode;

typedef struct {
    QueueNode *front;
    QueueNode *rear;
    int count;
} Queue;

//新建一个队列
Queue* NewQueue() {
    Queue *queue = (Queue*)malloc(sizeof(Queue));
    queue->front = queue->rear = NULL;
    queue->count = 0;
    return queue;
}

//入队
void Enqueue(Queue *queue, int value) {
    QueueNode *node = (QueueNode*) malloc(sizeof(QueueNode));
    node->data = value;
    node->next = NULL;
    if (queue->rear == NULL) {
        queue->front = node;
        queue->rear = node;
    } else {
        queue->rear->next = node;
        queue->rear = node;
    }
    queue->count++;
}

//出队
int Dequeue(Queue *queue) {
    if (queue->front == NULL) {
        printf("Error: queue is empty!");
        exit(-1);
    }
    QueueNode *node = queue->front;
    int value = node->data;
    if (queue->front == queue->rear) {
        queue->front = NULL;
        queue->rear = NULL;
    } else {
        queue->front = queue->front->next;
    }
    free(node);
    queue->count--;
    return value;
}

//求队列大小
int SizeQueue(Queue *queue) {
    return queue->count;
}

//取队头
int FrontQueue(Queue *queue) {
    if (queue->front == NULL) {
        printf("Error: queue is empty!");
        exit(-1);
    }
    return queue->front->data;
}

typedef struct Node {
    int command;
    int arg;
    int time;
    struct Node *next;
} ListNode;

//新建一个节点
ListNode* NewListNode(int command, int arg, int time, ListNode *next) {
    ListNode *node = (ListNode*) malloc(sizeof(ListNode));
    node->command = command;
    node->arg = arg;
    node->time = time;
    node->next = next;
    return node;
}

//释放一个节点
void FreeListNode(ListNode *node) {
    free(node);
}

// 公交车结构体
typedef struct {
    int station_count;      // 车站的数量
    int station_distance;     // 车站间的距离
    int total_length;  // 轨道的总长度
    int strategy;     // 调度策略

    int time;         // 当前时间
    int direction;    // 当前行驶方向
    int position;     // 当前位置
    int clockwise[SIZE_MAX_STATION];      // 有顺时针请求的车站
    int counter_clock[SIZE_MAX_STATION];  // 有逆时针站请求的车站
    int target[SIZE_MAX_STATION];         // 车内的请求

    int cur_command; // 正在调度的请求
    int cur_arg;     // 正在调度的请求参数
    int requests_count;
    ListNode *requests_list;  // 已经发出的请求用链表保存
    Queue *requests_queue;    // 已经发出的请求用队列保存
} Bus;

// 创建并初始化公交车实例
Bus* NewBus(int total_station, int distance, int strategy) {
    Bus *bus = (Bus*) malloc(sizeof(Bus));
    bus->station_count = total_station;
    bus->station_distance = distance;
    bus->total_length = total_station * distance;
    bus->strategy = strategy;

    bus->time = 0;
    bus->direction = 0;
    bus->position = 0;
    for (int i = 0; i < SIZE_MAX_STATION; ++i) {
        bus->clockwise[i] = 0;
        bus->counter_clock[i] = 0;
        bus->target[i] = 0;
    }

    bus->cur_command = 0;
    bus->cur_arg = 0;
    bus->requests_list = NewListNode(0, 0, 0, NULL);
    if (strategy == STRATEGY_FCFS) {
        bus->requests_queue = NewQueue();
    }
    return bus;
}

//输出公交车状态
void ShowBusStatus(Bus *bus) {
    printf("TIME:%d\n", bus->time);
    printf("BUS:\n");
    printf("position:%d\n", bus->position);
    printf("target:");
    for (int i = 0; i < bus->station_count; ++i) {
        printf("%d", bus->target[i]);
    }
    printf("\n");
    printf("STATION:\n");
    printf("clockwise:");
    for (int i = 0; i < bus->station_count; ++i) {
        printf("%d", bus->clockwise[i]);
    }
    printf("\n");
    printf("counterclockwise:");
    for (int i = 0; i < bus->station_count; ++i) {
        printf("%d", bus->counter_clock[i]);
    }
    printf("\n");
}

// 释放公交车实例内存
void DeleteBus(Bus *bus) {
    free(bus);
}