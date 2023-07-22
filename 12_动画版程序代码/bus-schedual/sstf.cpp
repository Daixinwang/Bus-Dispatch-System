#include "bus.h"

void SSTFScheduleNewRequest(Bus* bus) {
    if (bus->requests_list->next == NULL) {
        bus->direction = 0;
        bus->cur_command = 0;
        return;
    }
    int distance = bus->total_length + 1;
    int target_pos;
    ListNode* pre = bus->requests_list, * p;
    while (pre->next != NULL) {  // �������������ҵ������
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
        }
        else {
            if (counter_distance < distance) {
                distance = counter_distance;
                bus->direction = -1;
                bus->cur_command = p->command;
                bus->cur_arg = p->arg;
            }
        }
        pre = p;
    }
    // ��������ɾ���ѵ�������
    pre = bus->requests_list;
    while (pre->next != NULL) {
        p = pre->next;
        if (p->command == bus->cur_command && p->arg == bus->cur_arg) {
            pre->next = p->next;
            FreeListNode(p);
            break;
        }
        else {
            pre = p;
        }
    }
}

void SSTFActionOnCommand(Bus* bus, int command, int arg) {
    arg -= 1; // �������е�վ����Ŵ� 1-10 ��� 0-9
    if (command == CMD_CLOCK) {
        bus->time++;
        if (bus->position % bus->station_distance == 0) {  // �ﵽվ��λ��
            int station = bus->position / bus->station_distance;  // �õ�վ���
            int stop = 0; // �Ƿ���Ҫ�ڸ�վͣ��
            if (bus->cur_command != 0) {
                if (station != bus->cur_arg) {  // ����һ����Ŀ�ĵ����վ��
                    ListNode* pre = bus->requests_list, * p;
                    int clockwise = bus->direction == 1;
                    while (pre->next != NULL) {  // �����и�վ����Ѿ��������³������ͬ�����ϳ����󶼴�����
                        p = pre->next;
                        if (((clockwise && p->command == CMD_CLOCKWISE) ||
                            (!clockwise && p->command == CMD_COUNTERCLOCKWISE) ||
                            (p->command == CMD_TARGET)) && p->arg == station && bus->time > p->time) {
                            stop = 1; // ��������Ҫ���������վ����Ҫͣ��
                            pre->next = p->next;
                            FreeListNode(p);
                        }
                        else {
                            pre = p;
                        }
                    }
                    if (stop) {
                        if (clockwise) {
                            bus->clockwise[station] = 0;
                        }
                        else {
                            bus->counter_clock[station] = 0;
                        }
                        bus->target[station] = 0;
                    }
                }
                else {  // �������ڴ��������Ŀ�ĵ�
                    stop = 1;
                    ListNode* pre = bus->requests_list, * p;
                    while (pre->next != NULL) {  // ��������ͬվ����Ѿ����������󶼴�����
                        p = pre->next;
                        if (p->arg == bus->cur_arg) {
                            pre->next = p->next;
                            FreeListNode(p);
                        }
                        else {
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
            if (stop) return;
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
        return;
    }
    if (command == CMD_TARGET) {
        if (bus->target[arg]) return;
        bus->target[arg] = 1;
    }
    else if (command == CMD_CLOCKWISE) {
        if (bus->clockwise[arg]) return;
        bus->clockwise[arg] = 1;
    }
    else if (command == CMD_COUNTERCLOCKWISE) {
        if (bus->counter_clock[arg]) return;
        bus->counter_clock[arg] = 1;
    }
    ListNode* node = NewListNode(command, arg, bus->time + 1, bus->requests_list->next);
    bus->requests_list->next = node;
}

