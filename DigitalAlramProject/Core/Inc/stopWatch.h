/*
 * stopWatch.h
 *
 *  Created on: Oct 19, 2024
 *      Author: ckck
 */

#ifndef INC_STOPWATCH_H_
#define INC_STOPWATCH_H_

#include "main.h"
#include "7SEG.h"
#include "swControll.h"
#include "defines.h"

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

typedef struct _stopWatch {
	stopWatchActionState state;
	stopWatchTime time;
} stopWatch;

void segUpCount();
void segReset();
void timerReset();
void saveTime();
void sw2StopWatchControll();
void sw3StopWatchReset();


#endif /* INC_STOPWATCH_H_ */
