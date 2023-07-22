#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "const.h"
#include "bus.h"
#include "input.h"
#include "gui.h"

// ��dict.dic�ļ���ȡ���ò���
void ReadInConfig(int* station_count, int* station_distance, int* strategy) {
    FILE* fp = fopen("dict.dic", "r");
    if (fp == NULL) {
        printf("Failed to open config file!\n");
        return;
    }
    char line[200], key[20], value[20];
    while (!feof(fp)) {
        memset(line, 0, sizeof(line));
        fgets(line, sizeof(line), fp);
        if (strlen(line) == 0 || line[0] == '\n') break; // �����ļ�ĩβ
        if (line[0] == '#') continue;
        int lineIdx = 0, keyIdx = 0, valIdx = 0;
        while (line[lineIdx] == ' ')    // ����ÿ�п�ͷ�Ŀո�����еĻ���
            lineIdx++;
        while (line[lineIdx] != ' ')    // һֱ�����ո�
            key[keyIdx++] = line[lineIdx++];
        key[keyIdx] = '\0';
        while (line[lineIdx] != '=')          // �ҵ����ں�
            lineIdx++;
        lineIdx++; // �������ں�
        while (line[lineIdx] == ' ')    // �����Ⱥź���Ŀո�
            lineIdx++;
        while (line[lineIdx] != '\n' && line[lineIdx] != ' ')  // һֱ�����ո���߻��з�
            value[valIdx++] = line[lineIdx++];
        value[valIdx] = '\0';
        // printf("%s-%s\n", key, value);
        if (strcmp("TOTAL_STATION", key) == 0) {
            *station_count = atoi(value);
        }
        else if (strcmp("DISTANCE", key) == 0) {
            *station_distance = atoi(value);
        }
        else if (strcmp("STRATEGY", key) == 0) {
            if (strcmp("FCFS", value) == 0) {
                *strategy = STRATEGY_FCFS;
            }
            else if (strcmp("SSTF", value) == 0) {
                *strategy = STRATEGY_SSTF;
            }
            else if (strcmp("SCAN", value) == 0) {
                *strategy = STRATEGY_SCAN;
            }
        }
    }
}

// ����ָ�����У�������ָ������������Ϊ
void ReadCommandAndAct(Bus* bus) {
    int num = 0;
    char command[20];
    scanf("%s", command);
    ShowBusStatus(bus);
    while (strcmp(command, "end") != 0) {
        // printf("Command: %s\n", command);
        if (strcmp(command, "clock") == 0) {
            ActionOnCommand(bus, CMD_CLOCK, 0);
        }
        else if (strcmp(command, "target") == 0) {
            scanf("%d", &num);
            ActionOnCommand(bus, CMD_TARGET, num);
        }
        else if (strcmp(command, "clockwise") == 0) {
            scanf("%d", &num);
            ActionOnCommand(bus, CMD_CLOCKWISE, num);
        }
        else if (strcmp(command, "counterclockwise") == 0) {
            scanf("%d", &num);
            ActionOnCommand(bus, CMD_COUNTERCLOCKWISE, num);
        }
        scanf("%s", command);
    }
    printf("end");
}
