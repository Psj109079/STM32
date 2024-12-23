/*
 * buzzer.c
 *
 *  Created on: Nov 2, 2024
 *      Author: ckck
 */

#include "buzzer.h"

extern TIM_HandleTypeDef htim2;
buzzer buz = {1, 0, 0};
buz_tone_psc buzTone = BUZ_TONE_LOW_PSC;

void buzOnOff() {
	if(buz.onOff == TRUE) {
		buz.ms++;
		if(buz.ms < BUZ_OP_TIME) {
			TIM2->PSC = buzTone;
			HAL_TIM_PWM_Start(&htim2, TIM_CHANNEL_1);
		} else {
			buzTone = BUZ_TONE_LOW_PSC;
			buz.onOff = 0;
			buz.ms = 0;
			HAL_TIM_PWM_Stop(&htim2, TIM_CHANNEL_1);
		}
	}

}

void playMidTone() {
	if(getBuzzEnabled() == TRUE) {
		buzTone = BUZ_TONE_MID_PSC;
		setbuzzerOnOff(1);
	}
}

void playHighTone() {
	if(getBuzzEnabled() == TRUE) {
		buzTone = BUZ_TONE_HIGH_PSC;
		setbuzzerOnOff(1);
	}
}

void playAlram() {
	if(buz.ms < (BUZ_OP_TIME * 2)) {
		buz.ms++;
	}


	if(buz.ms < BUZ_OP_TIME) {
		if(getBuzzEnabled() == TRUE) {
			TIM2->PSC = BUZ_TONE_HIGH_PSC;
			HAL_TIM_PWM_Start(&htim2, TIM_CHANNEL_1);
		}
		HAL_GPIO_WritePin(GPIOD, GPIO_PIN_12, 0);
		HAL_GPIO_WritePin(GPIOC, GPIO_PIN_6, 0);
		HAL_GPIO_WritePin(GPIOD, GPIO_PIN_13, 0);
		HAL_GPIO_WritePin(GPIOB, GPIO_PIN_5, 0);
		HAL_GPIO_WritePin(GPIOD, GPIO_PIN_14, 0);
		HAL_GPIO_WritePin(GPIOB, GPIO_PIN_0, 0);

	} else if(buz.ms > BUZ_OP_TIME) {
		if(getBuzzEnabled() == TRUE) {
			TIM2->PSC = BUZ_TONE_LOW_PSC;
			HAL_TIM_PWM_Stop(&htim2, TIM_CHANNEL_1);
		}
		HAL_GPIO_WritePin(GPIOD, GPIO_PIN_12, 1);
		HAL_GPIO_WritePin(GPIOC, GPIO_PIN_6, 1);
		HAL_GPIO_WritePin(GPIOD, GPIO_PIN_13, 1);
		HAL_GPIO_WritePin(GPIOB, GPIO_PIN_5, 1);
		HAL_GPIO_WritePin(GPIOD, GPIO_PIN_14, 1);
		HAL_GPIO_WritePin(GPIOB, GPIO_PIN_0, 1);
		if(buz.ms >= (BUZ_OP_TIME * 2)) {
			buz.ms = 0;
		}
	}

}

void buzReset() {
	buz.ms = 0;
	buz.onOff = 0;
	TIM2->PSC = BUZ_TONE_LOW_PSC;
	HAL_TIM_PWM_Stop(&htim2, TIM_CHANNEL_1);
	HAL_GPIO_WritePin(GPIOD, GPIO_PIN_12, 1);
	HAL_GPIO_WritePin(GPIOC, GPIO_PIN_6, 1);
	HAL_GPIO_WritePin(GPIOD, GPIO_PIN_13, 1);
	HAL_GPIO_WritePin(GPIOB, GPIO_PIN_5, 1);
	HAL_GPIO_WritePin(GPIOD, GPIO_PIN_14, 1);
	HAL_GPIO_WritePin(GPIOB, GPIO_PIN_0, 1);
}

uint8_t getBuzzEnabled() {
	return buz.enabled;
}

void setBuzzEnabled(uint8_t enabled) {
	buz.enabled = enabled;
}

uint8_t getbuzzerOnOff() {
	return buz.onOff;
}

void setbuzzerOnOff(uint8_t onOff) {
	buz.onOff = onOff;
}



