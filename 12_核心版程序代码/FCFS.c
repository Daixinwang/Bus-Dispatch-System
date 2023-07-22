void FCFSScheduleNewRequest(Bus *bus) {
    if (!SizeQueue(bus->requests_queue)) {//请求队列里没有请求时停车
        bus->direction = 0;
        bus->cur_command = 0;
        return;
    }
    int front = Dequeue(bus->requests_queue);//取到第一个请求
    bus->cur_command = front / 100;//第一个请求的类型
    bus->cur_arg = front % 100;//第一个请求的参数值
    // 确定行进方向
    int target_pos = bus->cur_arg * bus->station_distance;//目标车站的位置
    int clockwise_distance = (target_pos - bus->position);
    if (clockwise_distance < 0) {
        clockwise_distance += bus->total_length;
    }
    if (clockwise_distance <= bus->total_length / 2)//选择路径短的方向
        bus->direction = 1;
    else
        bus->direction = -1;
}

void FCFSActionOnClock(Bus *bus){
    bus->time++;
    int arg_pos = bus->cur_arg * bus->station_distance;
    if (bus->cur_command != 0 && bus->position == arg_pos) { // 到达正在处理请求的目的地
        while (SizeQueue(bus->requests_queue)) {  // 遍历队列，把所有相同站点的已经发出的请求都处理完
            int next = FrontQueue(bus->requests_queue);
            int next_command = next / 100, next_arg = next % 100;
            if (bus->cur_arg == next_arg) {
                Dequeue(bus->requests_queue);
                if (next_command == CMD_TARGET) {
                    bus->target[bus->cur_arg] = 0;
                } else if (next_command == CMD_CLOCKWISE) {
                    bus->clockwise[bus->cur_arg] = 0;
                } else if (next_command == CMD_COUNTERCLOCKWISE) {
                    bus->counter_clock[bus->cur_arg] = 0;
                }
            } else {
                break;
            }
        }
        if (bus->cur_command == CMD_TARGET) {
            bus->target[bus->cur_arg] = 0;
        } else if (bus->cur_command == CMD_CLOCKWISE) {
            bus->clockwise[bus->cur_arg] = 0;
        } else if (bus->cur_command == CMD_COUNTERCLOCKWISE) {
            bus->counter_clock[bus->cur_arg] = 0;
        }
        bus->cur_command = 0;
        bus->direction = 0;
        return;
    }
    if (bus->cur_command == 0) {
        FCFSScheduleNewRequest(bus);
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

void FCFSActionOnCommand(Bus *bus, int command, int arg) {
    arg -= 1; // 把命令中的站点序号从 1-10 变成 0-9
    if (command == CMD_CLOCK) {
        FCFSActionOnClock(bus);
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
    Enqueue(bus->requests_queue, command * 100 + arg);
}