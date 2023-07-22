#include<easyx.h>
#include<Windows.h>
#include<process.h>
#include "bus.h"
#include "gui.h"

void DrawBusStatus(Bus* bus) {
	wchar_t StringTime[] = L"TIME:7";
	wchar_t StringPosition[] = L"position:5";
	wchar_t StringTarget[] = L"target:0000000000";
	wchar_t StringClockwise[] = L"clockwise:0000000000";
	wchar_t StringCounterclockwise[] = L"couterclockwise:0000000000";

	wchar_t timeStr[50], positionStr[50],
		targetStr[50], clockwiseStr[50], counterStr[50];
	wsprintfW(timeStr, L"TIME:%d", bus->time);
	wsprintfW(positionStr, L"position:%d", bus->position);
	wsprintfW(targetStr, L"target:");
	wsprintfW(clockwiseStr, L"clockwise:");
	wsprintfW(counterStr, L"couterclockwise:");
	for (int i = 0; i < bus->station_count; i++) {
		wsprintfW(targetStr + 7 + i, L"%d", bus->target[i]);
		wsprintfW(clockwiseStr + 10 + i, L"%d", bus->clockwise[i]);
		wsprintfW(counterStr + 16 + i, L"%d", bus->counter_clock[i]);
	}
	outtextxy(650, 120, timeStr);
	outtextxy(650, 150, positionStr);
	outtextxy(650, 180, targetStr);
	outtextxy(650, 210, clockwiseStr);
	outtextxy(650, 240, counterStr);
}

void DrawBoardWithBus(Bus* bus) {
	BeginBatchDraw();

	setbkcolor(WHITE);//设置背景为白色
	cleardevice();//清屏
	
	setlinecolor(BLACK);//设置边框颜色为黑色
	setlinestyle(PS_DASH, 2);//设置边框样式为虚线，宽度为2
	

	// 输出区
	rectangle(600, 100, 1050, 290);//画边界
	settextstyle(25, 0, L"宋体");
	setbkmode(TRANSPARENT);
	settextcolor(RED);
	DrawBusStatus(bus);

	//输入区
	setlinecolor(BLACK);//设置边框颜色为黑色
	setlinestyle(PS_SOLID, 2);//设置边框样式为虚线，宽度为2
	rectangle(350, 400, 750, 550);
	settextcolor(BLACK);
	settextstyle(20, 0, L"宋体");
	outtextxy(450, 415, L"点击指令输入指令参数");

	IMAGE cmdT, cmdC, cmdU, cmdEND;
	loadimage(&cmdT, L"pic\\T.png", 50, 50);
	loadimage(&cmdC, L"pic\\C.png", 50, 50);
	loadimage(&cmdU, L"pic\\U.png", 50, 50);
	loadimage(&cmdEND, L"pic\\END.png", 50, 50);
	putimage(375, 470, &cmdT);
	putimage(475, 470, &cmdC);
	putimage(575, 470, &cmdU);
	putimage(675, 470, &cmdEND);

	//动画区
	IMAGE BusU, BusD, BusL, BusR, StopU, StopD, StopL, StopR;
	loadimage(&BusU, L"pic\\bus_up.png", 30, 20);
	loadimage(&BusD, L"pic\\bus_down.png", 30, 20);
	loadimage(&BusL, L"pic\\bus_left.png", 20, 30);
	loadimage(&BusR, L"pic\\bus_right.png", 20, 30);

	loadimage(&StopU, L"pic\\stop_up.jpg", 20, 50);
	loadimage(&StopD, L"pic\\stop_down.jpg", 20, 50);
	loadimage(&StopL, L"pic\\stop_left.jpg", 50,20);
	loadimage(&StopR, L"pic\\stop_right.jpg", 50, 20);

	setlinecolor(BLACK);//设置边框颜色为黑色
	setlinestyle(PS_SOLID, 2);//设置边框样式为虚线，宽度为2
	rectangle(100, 100, 500, 300);//画边界

	int perDotRadius = 3, stationDotRadius = 4;

	// 画距离点
	setfillcolor(BLUE);
	double perPixel = 1200.0 / bus->total_length;
	for (int i = 0; i < bus->total_length; i++) {
		int fromStart = i * perPixel;
		if (fromStart <= 400) {
			solidcircle(fromStart + 100, 100, perDotRadius);
		}
		else if (fromStart <= 600) {
			solidcircle(500,  fromStart - 300, perDotRadius);
		}
		else if (fromStart <= 1000) {
			solidcircle(1100 - fromStart, 300, perDotRadius);
		}
		else {
			solidcircle(100, 1300 - fromStart, perDotRadius);
		}
	}

	// 画站点
	setfillcolor(RED);
	settextstyle(25, 0, L"宋体");
	setbkmode(TRANSPARENT);
	settextcolor(RED);
	wchar_t stationNum[10];
	double perStation = 1200.0 / bus->station_count;
	for (int i = 0; i < bus->station_count; i++) {
		int fromStart = i * perStation;
		wsprintfW(stationNum, L"%d", i+1);
		if (fromStart <= 400) {
			solidcircle(fromStart + 100, 100, stationDotRadius);
			putimage(fromStart + 84, 47, &StopU);
			outtextxy(fromStart + 105, 50, stationNum);
		}
		else if (fromStart <= 600) {
			solidcircle(500, fromStart - 300, stationDotRadius);
			putimage(503, fromStart - 316, &StopR);
			outtextxy(535, fromStart - 296, stationNum);
		}
		else if (fromStart <= 1000) {
			solidcircle(1100 - fromStart, 300, stationDotRadius);
			putimage(1096 - fromStart, 303, &StopD);
			outtextxy(1080 - fromStart, 330, stationNum);
		}
		else {
			solidcircle(100, 1300 - fromStart, stationDotRadius);
			putimage(46, 1297 - fromStart, &StopL);
			outtextxy(50, 1270 - fromStart, stationNum);
		}
	}

	// 画小车
	int fromStart = bus->position * perPixel;
	if (fromStart <= 400) {
		putimage(fromStart + 86, 77, &BusU);
	}
	else if (fromStart <= 600) {
		putimage(504, fromStart - 314, &BusR);
	}
	else if (fromStart <= 1000) {
		putimage(1086 - fromStart, 304, &BusD);
	}
	else {
		putimage(75, 1286 - fromStart, &BusL);
	}

	EndBatchDraw();
}

unsigned int __stdcall BackgroundClock(void *p) {
	Bus* bus = (Bus*)p;
	while (!bus->end) {
		Sleep(3000);
		ActionOnCommandNoOutput(bus, CMD_CLOCK, 0);
		DrawBoardWithBus(bus);
	}
	return 0;
}

void ReadCommandFromGUIAndAct(Bus* bus) {
	ExMessage m;
	int num;
	while (true) {
		m = getmessage(EM_MOUSE);
		if (m.message == WM_LBUTTONDOWN) {
			if (m.y >= 470 && m.y <= 520) {
				wchar_t s[10];
				if (m.x >= 375 && m.x <= 425) {
					InputBox(s, 10, L"请输入下车请求目标站点");
					int num = _wtoi(s);
					ActionOnCommandNoOutput(bus, CMD_TARGET, num);
				}
				else if (m.x >= 475 && m.x <= 525) {
					InputBox(s, 10, L"请输入顺时针上车请求站点");
					int num = _wtoi(s);
					ActionOnCommandNoOutput(bus, CMD_CLOCKWISE, num);
				}
				else if (m.x >= 575 && m.x <= 625) {
					InputBox(s, 10, L"请输入逆时针上车请求站点");
					int num = _wtoi(s);
					ActionOnCommandNoOutput(bus, CMD_COUNTERCLOCKWISE, num);
				}
				else if (m.x >= 675 && m.x <= 725) {
					bus->end = 1;
					return;
				}
			}
		}
	}
}