/*
 * alarm.c
 *
 *  Created on: Nov 14, 2024
 *      Author: ckck
 */
#include "alarm.h"

extern clockSt clock;
extern modeSelector mode;
modeSelector tempMode;
alarmSt alarm = {{{13, 0, 0, 0}, {0, 0, 1, 0}, {14, 1, 0, 0}, {16, 0, 0, 0}, {0, 0, 1, 0}}, 0};
void clcdDisplayAlarm() {
	sprintf(alarm.buffer, "ALARM #%d %-3s %3s", alarm.select + 1, (alarm.set[alarm.select].enabled == FALSE ? "OFF" : "ON"), (alarm.set[alarm.select].repeat == TRUE ? "RPT" : "ONC"));
	CLCD_Puts(0, 0, alarm.buffer);
	sprintf(alarm.buffer, "        %s %02d:%02d", (alarm.set[alarm.select].hour < 12 ? "AM" : "PM"),
			(alarm.set[alarm.select].hour > 12 ? (alarm.set[alarm.select].hour - 12) : alarm.set[alarm.select].hour == 0 ? 12 : alarm.set[alarm.select].hour),
			alarm.set[alarm.select].minute);
	CLCD_Puts(0, 1, alarm.buffer);

}

void clcdDisplayAlarmTrigger() {
	if(getBlink() == TRUE) {
		sprintf(alarm.buffer, "ALARM TRIGGERED!");
	} else {
		sprintf(alarm.buffer, "                ");
	}
	CLCD_Puts(0, 0, alarm.buffer);
	sprintf(alarm.buffer, "                ");
	CLCD_Puts(0, 1, alarm.buffer);
}
void selectAlarm() {	// sw2 기능 알람 선택
	if(alarm.select >= ((sizeof(alarm) - 21) / sizeof(alarm.set[0])) - 1) { // 알람 구조체안에 set 구조체5개의 길이 - 21
		alarm.select = 0;
	} else {
		alarm.select++;
	}
}

void alarmTrigger() {	// 알람 울림
	if(mode == ALARM_TRIGGER) {
		playAlram();
	} else {
		for(int i = 0; i < (sizeof(alarm) - 21) / sizeof(alarm.set[0]); i++) {	// 5번 동작
			if(alarm.set[i].enabled == TRUE) {	// 알람 활성화 확인
				if (alarm.set[i].hour == clock.hour
						&&	// 알람 설정 시간의 0초, 0밀리초 될 때 알람 온
						alarm.set[i].minute == clock.minute && clock.second == 0
						&& clock.millisecond == 0) {
					if(alarm.set[i].repeat == FALSE) {
						alarm.set[i].enabled = 0;
					}
					if(mode != ALARM_TRIGGER) { // 같은 시간에 알람이 여러개 있을 경우 알람이 울리면서 tempmode 가 ALARM_TRIGGER 모드로 바뀌는것을 방지
						tempMode = mode;
					}
					mode = ALARM_TRIGGER;
				}
			}
		}
	}
}

void returnToPreviousMode() {
	mode = tempMode;
}

uint8_t getSelect() {
	return alarm.select;
}

void setSelect(uint8_t s) {
	alarm.select = s;
}
uint8_t getAlarmEnabled(int num) {
	return alarm.set[num].enabled;
}
void setAlarmEnabled(uint8_t enabled, int num) {
	alarm.set[num].enabled = enabled;
}

uint8_t getAlarmRepeat(int num) {
	return alarm.set[num].repeat;
}

void setAlarmRepeat(uint8_t repeat, int num) {
	alarm.set[num].repeat = repeat;
}
