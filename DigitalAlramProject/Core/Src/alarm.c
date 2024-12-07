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
alarmSetting alarmSettingType = ALARM_NUM;

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

void clcdDisplayAlarmSetting() {
	switch(alarmSettingType) {
	case ALARM_NUM:
		if(getWaitingTime() < 200 || getBlink() == TRUE) {
			sprintf(alarm.buffer, "ALARM #%d %-3s %3s", alarm.select + 1,
					alarm.set[alarm.select].enabled == TRUE ? "ON" : "OFF",
					alarm.set[alarm.select].repeat == TRUE ? "RPT" : "ONC");
		} else {
			sprintf(alarm.buffer, "ALARM #  %-3s %3s", alarm.set[alarm.select].enabled == TRUE ? "ON" : "OFF",
					alarm.set[alarm.select].repeat == TRUE ? "RPT" : "ONC");
		}

		CLCD_Puts(0, 0, alarm.buffer);

		sprintf(alarm.buffer, "SET     %s %02d:%02d", alarm.set[alarm.select].hour > 11 ? "PM" : "AM",
				(alarm.set[alarm.select].hour > 12 ? (alarm.set[alarm.select].hour - 12) : alarm.set[alarm.select].hour == 0 ? 12 : alarm.set[alarm.select].hour),
				 alarm.set[alarm.select].minute);
		CLCD_Puts(0, 1, alarm.buffer);
		break;
	case ALARM_MINUTE:
		sprintf(alarm.buffer, "ALARM #%d %-3s %3s", alarm.select + 1,
				alarm.set[alarm.select].enabled == TRUE ? "ON" : "OFF",
				alarm.set[alarm.select].repeat == TRUE ? "RPT" : "ONC");
		CLCD_Puts(0, 0, alarm.buffer);
		if(getWaitingTime() < 200 || getBlink() == TRUE) {
			sprintf(alarm.buffer, "SET     %s %02d:%02d", alarm.set[alarm.select].hour > 11 ? "PM" : "AM",
					(alarm.set[alarm.select].hour > 12 ? (alarm.set[alarm.select].hour - 12) : alarm.set[alarm.select].hour == 0 ? 12 : alarm.set[alarm.select].hour),
					alarm.set[alarm.select].minute);
		} else {
			sprintf(alarm.buffer, "SET     %s %02d:  ", alarm.set[alarm.select].hour > 11 ? "PM" : "AM",
					(alarm.set[alarm.select].hour > 12 ? (alarm.set[alarm.select].hour - 12) : alarm.set[alarm.select].hour == 0 ? 12 : alarm.set[alarm.select].hour));
		}

		CLCD_Puts(0, 1, alarm.buffer);
		break;
	case ALARM_HOUR:
		sprintf(alarm.buffer, "ALARM #%d %-3s %3s", alarm.select + 1,
				alarm.set[alarm.select].enabled == TRUE ? "ON" : "OFF",
				alarm.set[alarm.select].repeat == TRUE ? "RPT" : "ONC");
		CLCD_Puts(0, 0, alarm.buffer);
		if(getWaitingTime() < 200 || getBlink() == TRUE) {
			sprintf(alarm.buffer, "SET     %s %02d:%02d", alarm.set[alarm.select].hour > 11 ? "PM" : "AM",
					(alarm.set[alarm.select].hour > 12 ? (alarm.set[alarm.select].hour - 12) : alarm.set[alarm.select].hour == 0 ? 12 : alarm.set[alarm.select].hour),
					alarm.set[alarm.select].minute);
		} else {
			sprintf(alarm.buffer, "SET     %s   :%02d", alarm.set[alarm.select].hour > 11 ? "PM" : "AM",
					alarm.set[alarm.select].minute);
		}
		CLCD_Puts(0, 1, alarm.buffer);
		break;
	case ALARM_REPEAT:
		if(getWaitingTime() < 200 || getBlink() == TRUE) {
			sprintf(alarm.buffer, "ALARM #%d %-3s %3s", alarm.select + 1,
							alarm.set[alarm.select].enabled == TRUE ? "ON" : "OFF",
							alarm.set[alarm.select].repeat == TRUE ? "RPT" : "ONC");
		} else {
			sprintf(alarm.buffer, "ALARM #%d %-3s    ", alarm.select + 1,
							alarm.set[alarm.select].enabled == TRUE ? "ON" : "OFF");
		}

		CLCD_Puts(0, 0, alarm.buffer);

		sprintf(alarm.buffer, "SET     %s %02d:%02d", alarm.set[alarm.select].hour > 11 ? "PM" : "AM",
				(alarm.set[alarm.select].hour > 12 ? (alarm.set[alarm.select].hour - 12) : alarm.set[alarm.select].hour == 0 ? 12 : alarm.set[alarm.select].hour),
				alarm.set[alarm.select].minute);
		CLCD_Puts(0, 1, alarm.buffer);
		break;
	case ALARM_ENABLE:
		if(getWaitingTime() < 200 || getBlink() == TRUE) {
			sprintf(alarm.buffer, "ALARM #%d %-3s %3s",alarm.select + 1,
					alarm.set[alarm.select].enabled == TRUE ? "ON" : "OFF",
					alarm.set[alarm.select].repeat == TRUE ? "RPT" : "ONC");
		} else {
			sprintf(alarm.buffer, "ALARM #%d     %3s",alarm.select + 1,
					alarm.set[alarm.select].repeat == TRUE ? "RPT" : "ONC");
		}

		CLCD_Puts(0, 0, alarm.buffer);

		sprintf(alarm.buffer, "SET     %s %02d:%02d", alarm.set[alarm.select].hour > 11 ? "PM" : "AM",
				(alarm.set[alarm.select].hour > 12 ? (alarm.set[alarm.select].hour - 12) : alarm.set[alarm.select].hour == 0 ? 12 : alarm.set[alarm.select].hour),
				 alarm.set[alarm.select].minute);
		CLCD_Puts(0, 1, alarm.buffer);
		break;
	}
}

void switchAlarmMode() { // 알람모드에서 알람설정 모드전환 함수
	if(mode == ALARM && getPressCount(1) == 700) {
		// 알람모드 에서 누른 시간이 700 인경우
		mode = ALARM_SETTING;
		HAL_GPIO_TogglePin(GPIOB, GPIO_PIN_5);
		playMidTone();

	} else if((mode == ALARM_SETTING && getPressCount(1) == 700)) {
		// 알람설정모드 에서 누른 시간이 700 인 경우
		mode = ALARM;
		alarmSettingType = ALARM_NUM;
		HAL_GPIO_TogglePin(GPIOB, GPIO_PIN_5);
		playMidTone();
		setWaitingTime(0);
	}
}

void settingAlarm() {	// 버튼 2번 동작 설정할 단위 변경
	if(alarmSettingType == ALARM_ENABLE) {
		alarmSettingType = ALARM_NUM;
	} else {
		alarmSettingType++;
	}
}

void incrementAlarmSetting() {	// 버튼 3번동작 (선택한 값 증가)
	switch(alarmSettingType) {
	case ALARM_NUM:
		if(alarm.select >= ((sizeof(alarm) - 21) / sizeof(alarm.set[0])) - 1) {
			alarm.select = 0;
		} else {
			alarm.select++;
		}
		break;
	case ALARM_MINUTE:
		if(alarm.set[alarm.select].minute == 59) {
			alarm.set[alarm.select].minute = 0;
		} else {
			alarm.set[alarm.select].minute++;
		}
		break;
	case ALARM_HOUR:
		if(alarm.set[alarm.select].hour == 23) {
			alarm.set[alarm.select].hour = 0;
		} else {
			alarm.set[alarm.select].hour++;
		}
		break;
	case ALARM_REPEAT:
		setAlarmRepeat(!getAlarmRepeat(alarm.select), alarm.select);
		break;
	case ALARM_ENABLE:
		setAlarmEnabled(!getAlarmEnabled(alarm.select), alarm.select);
		break;
	}
}

void decrementAlarmSetting() {	// 버튼 4번동작 (선택한 값 감소)
	switch(alarmSettingType) {
	case ALARM_NUM:
		if(alarm.select == 0) {
			alarm.select = 4;
		} else {
			alarm.select--;
		}
		break;
	case ALARM_MINUTE:
		if(alarm.set[alarm.select].minute == 0) {
			alarm.set[alarm.select].minute = 59;
		} else {
			alarm.set[alarm.select].minute--;
		}
		break;
	case ALARM_HOUR:
		if(alarm.set[alarm.select].hour == 0) {
			alarm.set[alarm.select].hour = 23;
		} else {
			alarm.set[alarm.select].hour--;
		}
		break;
	case ALARM_REPEAT:
		setAlarmRepeat(!getAlarmRepeat(alarm.select), alarm.select);
		break;
	case ALARM_ENABLE:
		setAlarmEnabled(!getAlarmEnabled(alarm.select), alarm.select);
		break;
	}

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
