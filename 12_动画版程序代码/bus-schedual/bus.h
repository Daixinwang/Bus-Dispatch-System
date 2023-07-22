#ifndef BUS_SCHEDULE_BUS_H
#define BUS_SCHEDULE_BUS_H

#include "const.h"
#include "queue.h"
#include "list.h"


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
    ListNode* requests_list;  // 已经发出的请求用链表保存
    Queue* requests_queue;    // 已经发出的请求用队列保存

    int end;
} Bus;

Bus* NewBus(int total_station, int distance, int strategy);
void ActionOnCommand(Bus* bus, int command, int arg);
void ActionOnCommandNoOutput(Bus* bus, int command, int arg);
void ShowBusStatus(Bus* bus);
void DeleteBus(Bus* bus);

void FCFSActionOnCommand(Bus* bus, int command, int arg);
void SSTFActionOnCommand(Bus* bus, int command, int arg);
void SCANActionOnCommand(Bus* bus, int command, int arg);

#endif //BUS_SCHEDULE_BUS_H
