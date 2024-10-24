/*
 * btnControll.c
 *
 *  Created on: Oct 13, 2024
 *      Author: ckck
 */
#include "swControll.h"


uint8_t sw1_flag;
uint8_t sw2_flag;
uint8_t sw3_flag;
uint8_t sw4_flag;
uint8_t select_LED;


void sw1RedLEDControll() {
	// SW1 PE3, PD12, PC6
	if (HAL_GPIO_ReadPin(GPIOE, GPIO_PIN_3)) {

		if (sw1_flag) {
			sw1_flag = 0;
			if (HAL_GPIO_ReadPin(GPIOD, GPIO_PIN_12)) {
				HAL_GPIO_WritePin(GPIOD, GPIO_PIN_12, 0);
			} else {
				HAL_GPIO_WritePin(GPIOD, GPIO_PIN_12, 1);
			}
		}
	} else {
		if (!sw1_flag) {
			if (HAL_GPIO_ReadPin(GPIOC, GPIO_PIN_6)) {
				HAL_GPIO_WritePin(GPIOC, GPIO_PIN_6, 0);
			} else {
				HAL_GPIO_WritePin(GPIOC, GPIO_PIN_6, 1);

			}
		}
	}
}

void sw2GreedLEDControll() {
	// SW2 PC15, PD13, PB5
	if (HAL_GPIO_ReadPin(GPIOC, GPIO_PIN_15)) {
		if (sw2_flag) {
			sw2_flag = 0;
			if (HAL_GPIO_ReadPin(GPIOD, GPIO_PIN_13)) {
				HAL_GPIO_WritePin(GPIOD, GPIO_PIN_13, 0);
			} else {
				HAL_GPIO_WritePin(GPIOD, GPIO_PIN_13, 1);
			}
		}
	} else {
		if (!sw2_flag) {
			sw2_flag = 1;
			if (HAL_GPIO_ReadPin(GPIOB, GPIO_PIN_5)) {
				HAL_GPIO_WritePin(GPIOB, GPIO_PIN_5, 0);
			} else {
				HAL_GPIO_WritePin(GPIOB, GPIO_PIN_5, 1);

			}
		}
	}
}

void sw3BlueLEDControll() {
	//SW3 PD4, PD14, PB0
	if (HAL_GPIO_ReadPin(GPIOD, GPIO_PIN_4)) {
		if (sw3_flag) {
			sw3_flag = 0;
			if (HAL_GPIO_ReadPin(GPIOD, GPIO_PIN_14)) {
				HAL_GPIO_WritePin(GPIOD, GPIO_PIN_14, 0);
			} else {
				HAL_GPIO_WritePin(GPIOD, GPIO_PIN_14, 1);
			}
		}
	} else {
		if (!sw3_flag) {
			sw3_flag = 1;
			if (HAL_GPIO_ReadPin(GPIOB, GPIO_PIN_0)) {
				HAL_GPIO_WritePin(GPIOB, GPIO_PIN_0, 0);
			} else {
				HAL_GPIO_WritePin(GPIOB, GPIO_PIN_0, 1);

			}
		}
	}

}
void sw4LEDSelect() {
	// SW PD10 LED select
	if (HAL_GPIO_ReadPin(GPIOD, GPIO_PIN_10) != sw4_flag) {
		sw4_flag = !sw4_flag;
		if(sw4_flag) {
			select_LED = !select_LED;
		}
	}
}


void sw1LEDRedToggle() {
	// SW1 PE3, LED PD12, PC6
	if (HAL_GPIO_ReadPin(GPIOE, GPIO_PIN_3) != sw1_flag) {
		sw1_flag = !sw1_flag;
		if (sw1_flag) {
			if (!select_LED) {
				if (HAL_GPIO_ReadPin(GPIOD, GPIO_PIN_12)) {
					HAL_GPIO_WritePin(GPIOD, GPIO_PIN_12, 0);
				} else {
					HAL_GPIO_WritePin(GPIOD, GPIO_PIN_12, 1);
				}

			}else {
				if (HAL_GPIO_ReadPin(GPIOC, GPIO_PIN_6)) {
					HAL_GPIO_WritePin(GPIOC, GPIO_PIN_6, 0);
				} else {
					HAL_GPIO_WritePin(GPIOC, GPIO_PIN_6, 1);
				}
			}
		}
	}
}

void sw2LEDGreenToggle() {
	// SW2 PC15, LED PD13, PB5
	if (HAL_GPIO_ReadPin(GPIOC, GPIO_PIN_15) != sw2_flag) {
		sw2_flag = !sw2_flag;
		if (sw2_flag) {
			if (!select_LED) {
				if (HAL_GPIO_ReadPin(GPIOD, GPIO_PIN_13)) {
					HAL_GPIO_WritePin(GPIOD, GPIO_PIN_13, 0);
				} else {
					HAL_GPIO_WritePin(GPIOD, GPIO_PIN_13, 1);
				}
			} else {
				if (HAL_GPIO_ReadPin(GPIOB, GPIO_PIN_5)) {
					HAL_GPIO_WritePin(GPIOB, GPIO_PIN_5, 0);
				} else {
					HAL_GPIO_WritePin(GPIOB, GPIO_PIN_5, 1);
				}
			}
		}
	}

}

void sw3LEDBlueToggle() {
	// SW3 PD4, LED PD14, PB0
	if (HAL_GPIO_ReadPin(GPIOD, GPIO_PIN_4) != sw3_flag) {
		sw3_flag = !sw3_flag;
		if (sw3_flag) {
			if (!select_LED) {
				if (HAL_GPIO_ReadPin(GPIOD, GPIO_PIN_14)) {
					HAL_GPIO_WritePin(GPIOD, GPIO_PIN_14, 0);
				} else {
					HAL_GPIO_WritePin(GPIOD, GPIO_PIN_14, 1);
				}
			} else {
				if (HAL_GPIO_ReadPin(GPIOB, GPIO_PIN_0)) {
					HAL_GPIO_WritePin(GPIOB, GPIO_PIN_0, 0);
				} else {
					HAL_GPIO_WritePin(GPIOB, GPIO_PIN_0, 1);
				}
			}

		}
	}
}


