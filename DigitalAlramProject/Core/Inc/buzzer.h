/*
 * buzzer.h
 *
 *  Created on: Nov 2, 2024
 *      Author: ckck
 */

#ifndef INC_BUZZER_H_
#define INC_BUZZER_H_

#include "main.h"
#include "swControll.h"

#define BUZ_OP_TIME 30

typedef enum _buz_tone_psc {
	BUZ_TONE_LOW_PSC = 15000,
	BUZ_TONE_MID_PSC = 10000,
	BUZ_TONE_HIGH_PSC = 7500
} buz_tone_psc;

typedef struct _buzzer {
	uint8_t enabled;	// 활성화, 비활성화 변수
	uint8_t onOff;		// 동작, 정지 flag
	uint8_t ms;
} buzzer;


void buzOnOff();
void playMidTone();
void playHighTone();
void playAlram();
void buzReset();
uint8_t getBuzzEnabled();
void setBuzzEnabled(uint8_t enabled);
uint8_t getbuzzerOnOff();
void setbuzzerOnOff(uint8_t onOFF);

#endif /* INC_BUZZER_H_ */
