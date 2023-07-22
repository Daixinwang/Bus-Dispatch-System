#include "input.h"
#include "bus.h"
#include "gui.h"
#include<Windows.h>
#include<process.h>
#include <graphics.h>

int main() {
    int station_count = 5;
    int station_distance = 2;
    int strategy = STRATEGY_FCFS;

    ReadInConfig(&station_count, &station_distance, &strategy);

    Bus* bus = NewBus(station_count, station_distance, strategy);

    initgraph(1200, 600, NULL); // EW_SHOWCONSOLE
    DrawBoardWithBus(bus);

    unsigned int tid;
    HANDLE handle = (HANDLE)_beginthreadex(nullptr, 0, &BackgroundClock, bus, 0, &tid);

    ReadCommandFromGUIAndAct(bus);

    WaitForSingleObject(handle, INFINITE);
    CloseHandle(handle);

    DeleteBus(bus);
    return 0;
}