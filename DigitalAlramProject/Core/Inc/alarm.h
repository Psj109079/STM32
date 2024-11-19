/*
 * alarm.h
 *
 *  Created on: Nov 14, 2024
 *      Author: ckck
 */

#ifndef INC_ALARM_H_
#define INC_ALARM_H_

#include "main.h"
#include "swControll.h"
#include "clock.h"
#include "buzzer.h"
#include "CLCD.h"

typedef struct _alarmSet {
	uint8_t hour;
	uint8_t minute;
	uint8_t enabled;
	uint8_t repeat;
} alarmSet;

typedef struct _alarmSt {
	alarmSet set[5];
	uint8_t select;
	uint8_t buffer[20];
} alarmSt;

void clcdDisplayAlarm();
void clcdDisplayAlarmTrigger();
void selectAlarm();
void alarmTrigger();
void returnToPreviousMode();
uint8_t getSelect();
void setSelect(uint8_t s);
uint8_t getAlarmEnabled(int num);
void setAlarmEnabled(uint8_t enable, int num);
uint8_t getAlarmRepeat(int num);
void setAlarmRepeat(uint8_t repeat, int num);

#endif /* INC_ALARM_H_ */
