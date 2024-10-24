/*
 * stopWatch.c
 *
 *  Created on: Oct 19, 2024
 *      Author: ckck
 */

#include "stopWatch.h"
stopWatch swState = {STOP, {0, 0, 0, 0}};
uint8_t PC15_State;
uint8_t PD4_State;



void segUpCount() {
	if(swState.time.millisecond < 500) {
		_7SEG_SetNumber(DGT1, swState.time.second % 10, 1);
	} else {
		_7SEG_SetNumber(DGT1, swState.time.second % 10, 0);
	}
	_7SEG_SetNumber(DGT2, swState.time.millisecond / 100, 0);

}

void segReset() {
	_7SEG_SetNumber(DGT1, 0, 1);
	_7SEG_SetNumber(DGT2, 0, 0);
}

void timerReset() {
	swState.time.hour = 0;
	swState.time.minute = 0;
	swState.time.second = 0;
	swState.time.millisecond = 0;
}

void saveTime() {

	if(swState.time.millisecond % 1000 == 0) {
		swState.time.millisecond = 0;
		swState.time.second++;
	}
	if(swState.time.second == 60) {
		swState.time.second = 0;
		swState.time.minute++;
	}
	if(swState.time.minute == 60) {
		swState.time.minute = 0;
		swState.time.hour++;
	}
	if(swState.time.hour == 24) {
		swState.time.hour = 0;
	}
}

void sw2StopWatchControll() {
	// SW2 PC15, PD13
	if (PC15_State == TRUE) {
		if(swState.state == STOP) {
			timerReset();
		}
		swState.state = RUN;
		segUpCount();
	} else if(PC15_State == FALSE && swState.state == RUN) {
		swState.state = PAUSE;
	}
}

void sw3StopWatchReset() {
	// SW3 PD4, PD14
	if(PD4_State) {
		if(swState.state == PAUSE || swState.state == STOP) {
			swState.state = STOP;
			segReset();
		}
	}
}
