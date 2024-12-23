/*
 * clock.h
 *
 *  Created on: Nov 3, 2024
 *      Author: ckck
 */

#ifndef INC_CLOCK_H_
#define INC_CLOCK_H_

#include "main.h"
#include <stdio.h>
#include "7SEG.h"
#include "CLCD.h"
#include "buzzer.h"

#define Y		2024
#define M		2
#define D		28
#define H		23
#define MIN		59
#define SEC		55

typedef enum _clockSetting {
	SECOND = 0,
	MINUTE,
	HOUR,
	DATE,
	MONTH,
	YEAR
} clockSetting;

typedef struct _clockSt {
		int year;
		int month;
		int date;
		int hour;
		int minute;
		int second;
		int millisecond;
		uint8_t leapYear;		// 윤년 판별변수 1이면 윤년 0이면 평년
		uint8_t thiryMonth;		// 말일이 30일인지 판별하는 변수 1이면 30일
		uint8_t timeFormet; 	// 12h/24h 설정변수 1이면 24, 0이면 12
		uint8_t buffer[20];		// clcd 출력용 버퍼
} clockSt;

void startClock();
void clcdDisplayClock();
void tickClock();
int leapYearCalculator(int year);
int isThirtyDays(int month);
void switchClockMode();
void clcdDisplayClockSetting();
void settingTime();
void incrementTime();
void decrementTime();
uint8_t getTimeFormet();
void setTimeFormet(int onOff);
#endif /* INC_CLOCK_H_ */
