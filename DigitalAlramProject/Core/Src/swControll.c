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

extern stopWatch stopwatch;
extern releasePoint rsp;
extern sw1 pe3;
extern sw2 pc15;
extern sw3 pd4;
extern sw4 pd10;

void sw1Controll() {
	if(pe3.state == TRUE) {
		if(pe3.flag == 0) {
			pe3.pressCnt = 0;
			pe3.flag = 1;
		}
		sprintf(pe3.buffer, "%4d   %d  %d", pe3.pressCnt, MID_PRESS, LONG_PRESS);
		CLCD_Puts(0, 0, pe3.buffer);
	} else if(pe3.state == FALSE && pe3.flag == TRUE) {
		pe3.flag = 0;
		if(pe3.pressCnt < MID_PRESS) {
			rsp = SHORT;
			CLCD_Puts(0, 1, "SHORT RELEASE");
			HAL_GPIO_TogglePin(GPIOC, GPIO_PIN_6);
		} else if(pe3.pressCnt > MID_PRESS && pe3.pressCnt < LONG_PRESS) {
			rsp = MID;
			CLCD_Puts(0, 1, "MID RELEASE");
			HAL_GPIO_TogglePin(GPIOB, GPIO_PIN_5);
		} else {
			rsp = LONG;
			CLCD_Puts(0, 1, "LONG RELEASE");
			HAL_GPIO_TogglePin(GPIOB, GPIO_PIN_0);
		}

	}
}

void sw2Controll() {
	// SW2 PC15, PD13
	// 스탑워치 시작, 일시정지, 재개 함수
	if (pc15.state == TRUE) {

		switch(stopwatch.state) {
		case STOP: timerReset();
			break;
		case PAUSE: timerResum();
			break;
		case RUN:
			break;
		}
		stopwatch.state = RUN;
		segUpCount();
		clcdStopWatchMeasure();
	} else if(pc15.state == FALSE && stopwatch.state == RUN) {
		stopwatch.state = PAUSE;
		timerPaused();
	}
}

void sw3Controll() {
	// SW3 PD4, PD14
	// 스탑워치리셋, 랩타임저장, CLCD초기화 함수
	if(pd4.state) {

		switch(stopwatch.state) {
		case STOP:
			if (pd4.flag == FALSE) {
				stopwatch.state = STOP;
				segReset();
				clcdStopWatchClear();
				pd4.flag = 1;

			}
			break;
		case PAUSE:
			if (pd4.flag == FALSE) {
				stopwatch.state = STOP;
				segReset();
				clcdStopWatchClear();
				pd4.flag = 1;

			}
			break;
		case RUN:
			// 버튼을 누르고 있을경우 랩타임이 계속 저장되는 경우를 방지
			if(pd4.flag == FALSE) {
				laptimeSave();
				pd4.flag = 1;
			}
			break;
		}
	} else {
		pd4.flag = 0;
	}
}

void sw4Controll() {
	// SW4 PD10, PC6
	// 저장된 laptime 출력
	if(pd10.state == TRUE) {
		if(pd10.flag == FALSE) {
			laptimeDisplay();
			pd10.flag = 1;
		}
	} else {
		pd10.flag = 0;
	}
}





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
	// SW2 pc15, PD13, PB5
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


