/*
 * stopWatch.h
 *
 *  Created on: Oct 19, 2024
 *      Author: ckck
 */

#ifndef INC_STOPWATCH_H_
#define INC_STOPWATCH_H_

#include <stdio.h>
#include "main.h"
#include "7SEG.h"
#include "swControll.h"
#include "CLCD.h"

#define CLCD_DEFAULT 			"STW 00:00:00.000"

typedef enum _stopWatchActionState {
	RUN = 0,
	PAUSE,
	STOP
} stopWatchActionState;

typedef struct _stopWatchTime {
	int hour;
	int minute;
	int second;
	int millisecond;
} stopWatchTime;

typedef struct _lapTime {
	stopWatchTime saveSpace[9]; // Laptime 저장변수
	uint8_t cnt;				// Laptime 저장시 카운트 변수
	uint8_t displayCnt;			// CLCD 출력시 카운트 변수
	uint8_t buffer[20];			// CLCD 출력 전 문자열 담기위한 변수
} lapTime;

typedef struct _stopWatch {
	uint8_t onOffState;			// 스탑워치 on/off 변수
	stopWatchActionState state;	// 스탑워치 동작 상태 변수
	stopWatchTime time;
	lapTime laptime;
} stopWatch;


void segUpCount();
void segReset();
void timerReset();
void timerPaused();
void timerResum();
void stopwatchTime();
void laptimeSave();
void laptimeDisplay();
void clcdStopWatchClear();
void clcdStopWatchMeasure();



#endif /* INC_STOPWATCH_H_ */
