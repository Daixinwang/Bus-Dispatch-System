#ifndef BUS_SCHEDULE_CONST_H
#define BUS_SCHEDULE_CONST_H

#define NULL 0

// 指令类型常量设置
#define CMD_CLOCK 0
#define CMD_TARGET 1
#define CMD_CLOCKWISE 2
#define CMD_COUNTERCLOCKWISE 3
#define CMD_END 4

// 策略类型常量设置
#define STRATEGY_FCFS 1
#define STRATEGY_SSTF 2
#define STRATEGY_SCAN 3

// 最大车站数量
#define SIZE_MAX_STATION 25

#endif //BUS_SCHEDULE_CONST_H
