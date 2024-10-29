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


#define MID_PRESS 		700
#define LONG_PRESS		2500

typedef enum _releasePoint {
	SHORT = 0,
	MID,
	LONG
} releasePoint;

typedef struct _sw1 {
	uint8_t state;
	uint8_t flag;
	int pressCnt;
	uint8_t buffer[20];
} sw1;
typedef struct _sw2 {
	uint8_t state;
	uint8_t flag;
} sw2;

typedef struct _sw3 {
	uint8_t state;
	uint8_t flag;
} sw3;

typedef struct _sw4 {
	uint8_t state;
	uint8_t flag;
} sw4;

void sw1Controll();
void sw2Controll();
void sw3Controll();
void sw4Controll();
void sw1RedLEDControll();
void sw2GreedLEDControll();
void sw3BlueLEDControll();
void sw4LEDSelect();
void sw1LEDRedToggle();
void sw2LEDGreenToggle();
void sw3LEDBlueToggle();
#endif /* INC_SWCONTROLL_H_ */
