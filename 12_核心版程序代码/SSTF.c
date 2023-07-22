void SSTFScheduleNewRequest(Bus *bus) {
    if (bus->requests_list->next == NULL) {//请求队列里没有请求时停车
        bus->direction = 0;
        bus->cur_command = 0;
        return;
    }
    int distance = bus->total_length + 1;
    int target_pos;
    ListNode *pre = bus->requests_list, *p;
    while (pre->next != NULL) {  // 遍历所有请求，找到最近的
        p = pre->next;
        target_pos = p->arg * bus->station_distance;
        int clockwise_distance = (target_pos - bus->position);
        if (clockwise_distance < 0) {
            clockwise_distance += bus->total_length;
        }
        int counter_distance = bus->total_length - clockwise_distance;
        if (clockwise_distance <= counter_distance) {
            if (clockwise_distance <= distance) {
                distance = clockwise_distance;
                bus->direction = 1;
                bus->cur_command = p->command;
                bus->cur_arg = p->arg;
            }
        } else {
            if (counter_distance < distance) {
                distance = counter_distance;
                bus->direction = -1;
                bus->cur_command = p->command;
                bus->cur_arg = p->arg;
            }
        }
        pre = p;
    }
    // 从链表中删除已调度请求
    pre = bus->requests_list;
    while (pre->next != NULL) {
        p = pre->next;
        if (p->command == bus->cur_command && p->arg == bus->cur_arg) {
            pre->next = p->next;
            FreeListNode(p);
            break;
        } else {
            pre = p;
        }
    }
}

int SSTFAchieveStation(Bus *bus){
    int station = bus->position / bus->station_distance;  // 得到站序号
    int stop = 0; // 是否需要在该站停车
    if (bus->cur_command != 0) {
        if (station != bus->cur_arg) {  // 到达一个除目的地外的站点
            ListNode *pre = bus->requests_list, *p;
            int clockwise = bus->direction == 1;
            while (pre->next != NULL) {  // 把所有该站点的已经发出的下车请求和同方向上车请求都处理完
                p = pre->next;
                if (((clockwise && p->command == CMD_CLOCKWISE) ||
                     (!clockwise && p->command == CMD_COUNTERCLOCKWISE) ||
                     (p->command == CMD_TARGET)) && p->arg == station && bus->time > p->time) {
                    stop = 1; // 有请求需要被处理，则该站点需要停车
                    pre->next = p->next;
                    FreeListNode(p);
                } else {
                    pre = p;
                }
            }
            if (stop) {
                if (clockwise) {
                    bus->clockwise[station] = 0;
                } else {
                    bus->counter_clock[station] = 0;
                }
                bus->target[station] = 0;
            }
        } else {  // 到达正在处理请求的目的地
            stop = 1;
            ListNode *pre = bus->requests_list, *p;
            while (pre->next != NULL) {  // 把所有相同站点的已经发出的请求都处理完
                p = pre->next;
                if (p->arg == bus->cur_arg) {
                    pre->next = p->next;
                    FreeListNode(p);
                } else {
                    pre = p;
                }
            }
            bus->target[bus->cur_arg] = 0;
            bus->clockwise[bus->cur_arg] = 0;
            bus->counter_clock[bus->cur_arg] = 0;
            bus->cur_command = 0;
            bus->direction = 0;
        }
    }
    return stop;
}

void SSTFActionOnClock(Bus *bus){
    bus->time++;
    if (bus->position % bus->station_distance == 0) {  // 达到站点位置
        if(SSTFAchieveStation(bus)==1)
            return;
    }
    if (bus->cur_command == 0) {
        SSTFScheduleNewRequest(bus);
    }
    if (bus->cur_command == 0) return;
    bus->position += bus->direction;
    if (bus->position == -1) {
        bus->position = bus->total_length - 1;
    }
    if (bus->position == bus->total_length) {
        bus->position = 0;
    }
}

void SSTFActionOnCommand(Bus *bus, int command, int arg) {
    arg -= 1; // 把命令中的站点序号从 1-10 变成 0-9
    if (command == CMD_CLOCK) {
        SSTFActionOnClock(bus);
        return;
    }
    if (command == CMD_TARGET) {
        if (bus->target[arg]) return;
        bus->target[arg] = 1;
    } else if (command == CMD_CLOCKWISE) {
        if (bus->clockwise[arg]) return;
        bus->clockwise[arg] = 1;
    } else if (command == CMD_COUNTERCLOCKWISE) {
        if (bus->counter_clock[arg]) return;
        bus->counter_clock[arg] = 1;
    }
    ListNode *node = NewListNode(command, arg, bus->time+1, bus->requests_list->next);
    bus->requests_list->next = node;
}