#include "head.h"
#include "FCFS.c"
#include "SSTF.c"
#include "SCAN.c"

void ActionOnCommand(Bus *bus, int command, int arg) {
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

// 从dict.dic文件读取配置参数
void ReadInConfig(int *station_count, int *station_distance, int *strategy) {
    FILE* fp = fopen("dict.dic", "r");
    if (fp == NULL) {
        printf("Failed to open config file!\n");
        return;
    }
    char line[200], key[20], value[20];
    while (!feof(fp)) {
        memset(line, 0, sizeof(line));
        fgets(line, sizeof(line), fp);
        if (strlen(line) == 0 || line[0] == '\n') break; // 读到文件末尾
        if (line[0] == '#') continue;
        int lineIdx = 0, keyIdx = 0, valIdx = 0;
        while (line[lineIdx] == ' ')    // 跳过每行开头的空格（如果有的话）
            lineIdx++;
        while (line[lineIdx] != ' ')    // 一直读到空格
            key[keyIdx++] = line[lineIdx++];
        key[keyIdx] = '\0';
        while (line[lineIdx] != '=')          // 找到等于号
            lineIdx++;
        lineIdx++; // 跳过等于号
        while (line[lineIdx] == ' ')    // 跳过等号后面的空格
            lineIdx++;
        while (line[lineIdx] != '\n' && line[lineIdx] != ' ')  // 一直读到空格或者换行符
            value[valIdx++] = line[lineIdx++];
        value[valIdx] = '\0';
        // printf("%s-%s\n", key, value);
        if (strcmp("TOTAL_STATION", key) == 0) {
            *station_count = atoi(value);
        } else if (strcmp("DISTANCE", key) == 0) {
            *station_distance = atoi(value);
        } else if (strcmp("STRATEGY", key) == 0) {
            if (strcmp("FCFS", value) == 0) {
                *strategy = STRATEGY_FCFS;
            } else if (strcmp("SSTF", value) == 0){
                *strategy = STRATEGY_SSTF;
            } else if(strcmp("SCAN", value)==0){
                *strategy = STRATEGY_SCAN;
            }
        }
    }
}

// 读入指令序列，并根据指令序列做出行为
void ReadCommandAndAct(Bus *bus) {
    int num = 0;
    char command[20];
    scanf("%s", command);
    ShowBusStatus(bus);
    while (strcmp(command, "end") != 0) {
        if (strcmp(command, "clock") == 0) {
            ActionOnCommand(bus, CMD_CLOCK, 0);
        }
        else if(strcmp(command, "target") == 0) {
            scanf("%d", &num);
            ActionOnCommand(bus, CMD_TARGET,  num);
        }
        else if(strcmp(command, "clockwise") == 0) {
            scanf("%d", &num);
            ActionOnCommand(bus, CMD_CLOCKWISE, num);
        }
        else if(strcmp(command, "counterclockwise") == 0) {
            scanf("%d", &num);
            ActionOnCommand(bus, CMD_COUNTERCLOCKWISE, num);
        }
        scanf("%s", command);
    }
    printf("end");
}

int main() {
    int station_count = 5;
    int station_distance = 2;
    int strategy = STRATEGY_FCFS;

    ReadInConfig(&station_count, &station_distance, &strategy);

    Bus *bus = NewBus(station_count, station_distance, strategy);

    ReadCommandAndAct(bus);

    DeleteBus(bus);
    return 0;
}
