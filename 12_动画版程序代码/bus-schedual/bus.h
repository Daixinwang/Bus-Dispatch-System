#ifndef BUS_SCHEDULE_BUS_H
#define BUS_SCHEDULE_BUS_H

#include "const.h"
#include "queue.h"
#include "list.h"


// �������ṹ��
typedef struct {
    int station_count;      // ��վ������
    int station_distance;     // ��վ��ľ���
    int total_length;  // ������ܳ���
    int strategy;     // ���Ȳ���

    int time;         // ��ǰʱ��
    int direction;    // ��ǰ��ʻ����
    int position;     // ��ǰλ��
    int clockwise[SIZE_MAX_STATION];      // ��˳ʱ������ĳ�վ
    int counter_clock[SIZE_MAX_STATION];  // ����ʱ��վ����ĳ�վ
    int target[SIZE_MAX_STATION];         // ���ڵ�����

    int cur_command; // ���ڵ��ȵ�����
    int cur_arg;     // ���ڵ��ȵ��������
    int requests_count;
    ListNode* requests_list;  // �Ѿ�������������������
    Queue* requests_queue;    // �Ѿ������������ö��б���

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
