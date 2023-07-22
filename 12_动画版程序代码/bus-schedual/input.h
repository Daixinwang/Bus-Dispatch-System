#ifndef BUS_SCHEDULE_INPUT_H
#define BUS_SCHEDULE_INPUT_H
#include "bus.h"

void ReadInConfig(int* station_count, int* station_distance, int* strategy);
void ReadCommandAndAct(Bus* bus);

#endif //BUS_SCHEDULE_INPUT_H
