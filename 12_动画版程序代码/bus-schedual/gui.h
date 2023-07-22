#ifndef GUI_H
#define GUI_H

#include "bus.h"

void DrawBoardWithBus(Bus* bus);
void ReadCommandFromGUIAndAct(Bus* bus);
unsigned int __stdcall BackgroundClock(void* p);

#endif //GUI_H