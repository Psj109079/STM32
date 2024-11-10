/*
 * btnControll.h
 *
 *  Created on: Oct 13, 2024
 *      Author: ckck
 */

#ifndef INC_SWCONTROLL_H_
#define INC_SWCONTROLL_H_

#include <stdio.h>
#include "main.h"
#include "7SEG.h"
#include "CLCD.h"
#include "stopWatch.h"
#include "buzzer.h"
#include "clock.h"


#define MID_PRESS 		700
#define LONG_PRESS		2500

typedef enum _releasePoint {
	SHORT = 0,
	MID,
	LONG
} releasePoint;

typedef enum _modeSelector{
	CLOCK = 0,
	STOPWATCH,
	CLOCK_SETTING	// clock 모드에서 mid 경계일때 설정모드로 진입하고 release 시에는 진입하지 못하는 값
} modeSelector;

typedef struct _button {
	uint8_t state;
	uint8_t flag;
	int pressCnt;			// 누르고 있는 시간 저장 변수
	uint8_t buffer[20];		// clcd 출력용 버퍼
} button;


void running();
void sw1Controll();
void sw2Controll();
void sw3Controll();
void sw4Controll();
int getPressCount(int swNum);
void holdEvent();
void releasePointing(int swNum);
void sw1RedLEDControll();
void sw2GreedLEDControll();
void sw3BlueLEDControll();
void sw4LEDSelect();
void sw1LEDRedToggle();
void sw2LEDGreenToggle();
void sw3LEDBlueToggle();
#endif /* INC_SWCONTROLL_H_ */
