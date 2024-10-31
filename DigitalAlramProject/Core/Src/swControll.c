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
			pe3.tempCnt++;
			HAL_GPIO_TogglePin(GPIOD, GPIO_PIN_12);
			pe3.flag = 1;
		}
		sprintf(pe3.buffer, "%4d %d %d%3d", pe3.pressCnt, MID_PRESS, LONG_PRESS, pe3.tempCnt);
		CLCD_Puts(0, 0, pe3.buffer);	// 누른 시간 출력
		if(pe3.pressCnt < MID_PRESS) {
			rsp = SHORT;
			CLCD_Puts(0, 1, "SHORT HOLD           ");
		} else if(pe3.pressCnt > MID_PRESS && pe3.pressCnt < LONG_PRESS) {
			rsp = MID;
			CLCD_Puts(0, 1, "MID HOLD             ");
		} else {
			rsp = LONG;
			CLCD_Puts(0, 1, "LONG HOLD            ");
		}
	} else if(pe3.state == FALSE && pe3.flag == TRUE) {
		pe3.flag = 0;
		// 누른 시간에 따라 short, mid, long release 출력
		switch (rsp) {
		case SHORT:
			CLCD_Puts(0, 1, "SHORT RELEASE        ");
			HAL_GPIO_TogglePin(GPIOC, GPIO_PIN_6);
			break;
		case MID:
			CLCD_Puts(0, 1, "MID RELEASE          ");
			HAL_GPIO_TogglePin(GPIOB, GPIO_PIN_5);
			break;
		case LONG:
			CLCD_Puts(0, 1, "LONG RELEASE         ");
			HAL_GPIO_TogglePin(GPIOB, GPIO_PIN_0);
			break;
		}
	}
}

void sw2Controll() {
	// SW2 PC15, PD13
	// 스탑워치 시작, 일시정지, 재개 함수
	if (stopwatch.onOffState == TRUE) {

		switch(stopwatch.state) {
		case STOP: timerReset(); // 시작 전 타이머 0으로 리셋
			break;
		case PAUSE: timerResum(); // 일시정지 전 흘렀던 시간 가져오기
			break;
		case RUN: break;
		}
		// 스탑워치 동작상태 RUN 으로 변경 후 실행
		stopwatch.state = RUN;
		segUpCount();
		clcdStopWatchMeasure();
	} else if(stopwatch.onOffState == FALSE && stopwatch.state == RUN) {
		// 스탑워치 동작상태 PAUSE 로 변경 후 흘렀던 시간 저장
		stopwatch.state = PAUSE;
		timerPaused();
	}
}

void sw3Controll() {
	// SW3 PD4, PD14
	// 스탑워치리셋, 랩타임저장, CLCD초기화 함수
	if(pd4.state == TRUE) {

		switch(stopwatch.state) {
		case STOP:
			if (pd4.flag == FALSE) {
				// 스탑워치 리셋, 랩타임 초기화
				stopwatch.state = STOP;
				segReset(); // 7세그먼트 초기화, clcd와 랩타임 저장 초기화
				clcdStopWatchClear();
				HAL_GPIO_TogglePin(GPIOD, GPIO_PIN_14);
				pd4.flag = 1;

			}
			break;
		case PAUSE:
			if (pd4.flag == FALSE) {
				// 스탑워치 리셋, 랩타임 초기화
				stopwatch.state = STOP;
				segReset(); // 7세그먼트 초기화, clcd와 랩타임 저장 초기화
				clcdStopWatchClear();
				HAL_GPIO_TogglePin(GPIOD, GPIO_PIN_14);
				pd4.flag = 1;

			}
			break;
		case RUN:
			// 버튼을 누르고 있을경우 랩타임이 계속 저장되는 경우를 방지
			if(pd4.flag == FALSE) {
				laptimeSave(); // 랩타임 저장
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
			laptimeDisplay(); // 랩타임 순서대로 출력
			HAL_GPIO_TogglePin(GPIOC, GPIO_PIN_6);
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


