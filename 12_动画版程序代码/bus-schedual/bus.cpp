#include <stdio.h>
#include <stdlib.h>
#include <Windows.h>
#include "bus.h"
#include "queue.h"
#include "list.h"

// 创建并初始化公交车实例
Bus* NewBus(int total_station, int distance, int strategy) {
    Bus* bus = (Bus*)malloc(sizeof(Bus));
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
    bus->end = 0;
    return bus;
}

void ActionOnCommand(Bus* bus, int command, int arg) {
    switch (bus->strategy) {
    case STRATEGY_FCFS:
        FCFSActionOnCommand(bus, command, arg);
        break;
    case STRATEGY_SSTF:
        SSTFActionOnCommand(bus, command, arg);
        break;
    case STRATEGY_SCAN:
        SCANActionOnCommand(bus, command, arg);
        break;
    }
    if (command == CMD_CLOCK) {
        ShowBusStatus(bus);
    }
}

void ActionOnCommandNoOutput(Bus* bus, int command, int arg) {
    switch (bus->strategy) {
    case STRATEGY_FCFS:
        FCFSActionOnCommand(bus, command, arg);
        break;
    case STRATEGY_SSTF:
        SSTFActionOnCommand(bus, command, arg);
        break;
    case STRATEGY_SCAN:
        SCANActionOnCommand(bus, command, arg);
        break;
    }
}

//输出公交车状态
void ShowBusStatus(Bus* bus) {
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
void DeleteBus(Bus* bus) {
    free(bus);
}
