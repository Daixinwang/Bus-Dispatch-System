void SCANDecideDirection(Bus *bus) {
    static int first_decide_flag = 1;  // 是否是第一次决定方向
    if (bus->requests_list->next == NULL) {
        bus->direction = 0;
        first_decide_flag = 1;
        return;
    }
    int target_pos;
    if (first_decide_flag) {
        first_decide_flag = 0;
        int distance = bus->total_length + 1;
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
                }
            } else {
                if (counter_distance < distance) {
                    distance = counter_distance;
                    bus->direction = -1;
                }
            }
            pre = p;
        }
    } else {
        int should_turn_flag = 1;
        ListNode *pre = bus->requests_list, *p;
        while (pre->next != NULL) {  // 遍历所有请求，看是否需要转向
            p = pre->next;
            target_pos = p->arg * bus->station_distance;
            int clockwise_distance = (target_pos - bus->position);
            if (clockwise_distance < 0) {
                clockwise_distance += bus->total_length;
            }
            int counter_distance = bus->total_length - clockwise_distance;
            if (bus->direction == 1 && clockwise_distance <= bus->total_length / 2) {
                should_turn_flag = 0;
            }
            if (bus->direction == -1 && counter_distance <= bus->total_length / 2) {
                should_turn_flag = 0;
            }
            pre = p;
        }
        if (should_turn_flag) bus->direction = - bus->direction;
    }
}

void SCANActionOnCommand(Bus *bus, int command, int arg) {
    arg -= 1; // 把命令中的站点序号从 1-10 变成 0-9
    if (command == CMD_CLOCK) {
        bus->time++;
        if (bus->position % bus->station_distance == 0) {  // 达到站点位置
            int station = bus->position / bus->station_distance;  // 得到站序号
            int stop = 0; // 是否需要在该站停车
            ListNode *pre = bus->requests_list, *p;
            while (pre->next != NULL) {  // 把所有该站点的已经发出的请求都处理完
                p = pre->next;
                if (p->arg == station && bus->time > p->time) {
                    stop = 1;
                    pre->next = p->next;
                    FreeListNode(p);
                } else {
                    pre = p;
                }
            }
            if (stop) {
                bus->target[station] = 0;
                bus->clockwise[station] = 0;
                bus->counter_clock[station] = 0;
                return;
            }
        }
        SCANDecideDirection(bus);
        if (bus->direction == 0) return;
        bus->position += bus->direction;
        if (bus->position == -1) {
            bus->position = bus->total_length - 1;
        }
        if (bus->position == bus->total_length) {
            bus->position = 0;
        }
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