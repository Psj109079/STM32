/*
 * swControll.c
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
releasePoint rsp = SHORT;
modeSelector mode = CLOCK;
extern button sw1;
extern button sw2;
extern button sw3;
extern button sw4;
extern system sys;

void running() {
	// 모드 동작
	switch (mode) {
	case CLOCK:
		clcdDisplayClock();
		tickClock();
		break;
	case ALARM:
		clcdDisplayAlarm();
		tickClock();
		break;

	case STOPWATCH:
		clcdStopWatchMeasure();
		segUpCount();
		break;
	case CLOCK_SETTING:
		clcdDisplayClockSetting();
		tickClock();
		break;
	case ALARM_SETTING:
		clcdDisplayAlarmSetting();
		tickClock();
		break;
	case ALARM_TRIGGER:
		clcdDisplayAlarmTrigger();
		tickClock();
		break;
	}


}

void sw1Controll() {
	// SW1 PE3
	// 모드 변경 기능
	if(mode != ALARM_TRIGGER) {
		if (sw1.state == TRUE) {
			if (sw1.flag == FALSE) {
				HAL_GPIO_TogglePin(GPIOD, GPIO_PIN_12);
				sw1DefaultHandler();
				setWaitingTime(0);
				sw1.flag = 1;		// 플래그 변수를 1로 만들어서 버튼을 누르고 있을 때 다시 동작하는 경우 방지
			}
			releasePointing(1);
		} else {
			if (sw1.flag == TRUE) {
				if(rsp == SHORT) {
					if(mode <= STOPWATCH) {
						if (mode == STOPWATCH) { // 모드변경
							mode = CLOCK;
						} else {
							mode++;
						}
					}
					CLCD_Clear();
					HAL_GPIO_TogglePin(GPIOC, GPIO_PIN_6);
				}
				sw1.flag = 0;
			}
		}
	} else {
		if(sw1.state == TRUE) {
			if(sw1.flag == FALSE) {
				sw1.flag = 1;
			}
		} else {
			if(sw1.flag == TRUE) {
				returnToPreviousMode();
				buzReset();
				sw1.flag = 0;
			}
		}
	}

}

void sw2Controll() {
	// SW2 PC15
	switch (mode) {
	case CLOCK:		// 시계모드 일때 PC15 기능
		// 부저 on, off
		if(sw2.state == TRUE) {
			if (sw2.flag == FALSE) {
				sw2.flag = 1;
				sw2DefaultHandler();
				HAL_GPIO_TogglePin(GPIOD, GPIO_PIN_12);

			}
			releasePointing(2);

		} else {
			if (sw2.flag == TRUE) {
				if (rsp == SHORT) {
					setBuzzEnabled(!getBuzzEnabled()); 	// 부저 활성, 비활성 함수
				}
				HAL_GPIO_TogglePin(GPIOC, GPIO_PIN_6);
				sw2.flag = 0;
			}
		}


		break;
	case ALARM:	// 알람 모드일 때 PC15 기능
		if (sw2.state == TRUE) {
			if (sw2.flag == FALSE) {
				sw2.flag = 1;
				sw2DefaultHandler();
				setWaitingTime(0);
				HAL_GPIO_TogglePin(GPIOD, GPIO_PIN_12);
			}
			releasePointing(2);
		} else {
			if(sw2.flag == TRUE) {
				if (rsp == SHORT) {
					selectAlarm();
				}
				sw2.flag = 0;
				HAL_GPIO_TogglePin(GPIOC, GPIO_PIN_6);
			}
		}

		break;
	case STOPWATCH: // 스탑워치 모드일 때 PC15 기능
		// 스탑워치 시작, 일시정지, 재개 함수
		if(sw2.state == TRUE) {
			if (sw2.flag == FALSE) {
				sw2.flag = 1;
				sw2DefaultHandler();
				setWaitingTime(0);
				HAL_GPIO_TogglePin(GPIOD, GPIO_PIN_12);
			}

		} else {
			if (sw2.flag == TRUE) {
				sw2.flag = 0;
				HAL_GPIO_TogglePin(GPIOC, GPIO_PIN_6);
			}
		}
		if(stopwatch.onOffState == TRUE) {

			switch (stopwatch.state) {
			case STOP:
				timerReset(); // 시작 전 타이머 0으로 리셋
				break;
			case PAUSE:
				timerResum(); // 일시정지 전 흘렀던 시간 가져오기
				break;
			case RUN:
				break;
			}
			// 스탑워치 동작상태 RUN 으로 변경
			stopwatch.state = RUN;

		} else if(stopwatch.onOffState == FALSE && stopwatch.state == RUN) {
			// 스탑워치 동작상태 PAUSE 로 변경 후 흘렀던 시간 저장
			stopwatch.state = PAUSE;
			timerPaused();
		}

		break;
	case CLOCK_SETTING: // 시계설정모드 일때 PC15 기능
		if(sw2.state == TRUE) {
			if(sw2.flag == FALSE) {
				sw2.flag = 1;
				sw2DefaultHandler();
				setWaitingTime(0);
				settingTime();
				HAL_GPIO_TogglePin(GPIOD, GPIO_PIN_12);
			}
			releasePointing(2);
		} else {
			if(sw2.flag == TRUE) {
				sw2.flag = 0;
				HAL_GPIO_TogglePin(GPIOC, GPIO_PIN_6);
			}
		}
		break;
	case ALARM_SETTING:
		if(sw2.state == TRUE) {
			if(sw2.flag == FALSE) {
				sw2.flag = 1;
				sw2DefaultHandler();
				setWaitingTime(0);
				settingAlarm();
				HAL_GPIO_TogglePin(GPIOD, GPIO_PIN_12);
			}
			releasePointing(2);
		} else {
			if(sw2.flag == TRUE) {
				sw2.flag = 0;
				HAL_GPIO_TogglePin(GPIOC, GPIO_PIN_6);
			}
		}
		break;
	case ALARM_TRIGGER:
		if(sw2.state == TRUE) {
			if(sw2.flag == FALSE) {
				sw2.flag = 1;
			}
		} else {
			if(sw2.flag == TRUE) {
				returnToPreviousMode();
				buzReset();
				sw2.flag = 0;
			}
		}
		break;
	}
}

void sw3Controll() {
	// SW3 PD4
	switch (mode) {
	case CLOCK:		// 시계모드 일때 PD4 기능
		//	시간 포멧 변경
		if (sw3.state == TRUE) {
			if (sw3.flag == FALSE) {
				sw3.flag = 1;
				sw3DefaultHandler();
				HAL_GPIO_TogglePin(GPIOD, GPIO_PIN_12);
			}
			releasePointing(3);
		} else {
			if (sw3.flag == TRUE) {
				if (rsp == SHORT) {
					setTimeFormet(!getTimeFormet()); // 시간포멧 변경
				}
				sw3.flag = 0;
				HAL_GPIO_TogglePin(GPIOC, GPIO_PIN_6);
			}
		}

		break;
	case ALARM:	// 알람 모드일 때 PD4 기능
		if (sw3.state == TRUE) {
			if (sw3.flag == FALSE) {
				sw3.flag = 1;
				sw3DefaultHandler();
				setWaitingTime(0);
				HAL_GPIO_TogglePin(GPIOD, GPIO_PIN_12);
			}
			releasePointing(3);
		} else {
			if (sw3.flag == TRUE) {
				if (rsp == SHORT) {
					setAlarmEnabled(!getAlarmEnabled(getSelect()), getSelect());
					// 해당번호의 알람 활성화 / 비활성화
				}
				sw3.flag = 0;
				HAL_GPIO_TogglePin(GPIOC, GPIO_PIN_6);
			}
		}
		break;
	case STOPWATCH:	// 스탑워치 모드일 때 PD4 기능
		// 스탑워치리셋, 랩타임저장, CLCD초기화 함수
		if (sw3.state == TRUE) {
			setWaitingTime(0);
			switch (stopwatch.state) {
			case STOP:
				if (sw3.flag == FALSE) {
					// 스탑워치 리셋, 랩타임 초기화
					stopwatch.state = STOP;
					segReset(); // 7세그먼트 초기화, clcd와 랩타임 저장 초기화
					clcdStopWatchClear();
					sw4DefaultHandler();
					HAL_GPIO_TogglePin(GPIOD, GPIO_PIN_12);
					sw3.flag = 1;

				}
				break;
			case PAUSE:
				if (sw3.flag == FALSE) {
					// 스탑워치 리셋, 랩타임 초기화
					stopwatch.state = STOP;
					clcdStopWatchClear();// clcd와 랩타임 저장 초기화
					sw4DefaultHandler();
					HAL_GPIO_TogglePin(GPIOD, GPIO_PIN_12);
					sw3.flag = 1;

				}
				break;
			case RUN:
				// 버튼을 누르고 있을경우 랩타임이 계속 저장되는 경우를 방지
				if (sw3.flag == FALSE) {
					laptimeSave(); // 랩타임 저장
					sw4DefaultHandler();
					HAL_GPIO_TogglePin(GPIOD, GPIO_PIN_12);
					sw3.flag = 1;
				}
				break;
			}
		} else {
			if(sw3.flag == TRUE) {
				sw3.flag = 0;
				HAL_GPIO_TogglePin(GPIOC, GPIO_PIN_6);
			}
		}
		break;
	case CLOCK_SETTING: // 시계설정모드 일때 PD4 기능
		if (sw3.state == TRUE) {
			if (sw3.flag == FALSE) {
				sw3.flag = 1;
				sw3DefaultHandler();
				incrementTime();
				HAL_GPIO_TogglePin(GPIOD, GPIO_PIN_12);
			}
			setWaitingTime(0);
			releasePointing(3);
		} else {
			if (sw3.flag == TRUE) {
				sw3.flag = 0;
				rsp = SHORT;
				HAL_GPIO_TogglePin(GPIOC, GPIO_PIN_6);
			}
		}
		break;
	case ALARM_SETTING:
		if (sw3.state == TRUE) {
			if (sw3.flag == FALSE) {
				sw3.flag = 1;
				sw3DefaultHandler();
				incrementAlarmSetting();
				HAL_GPIO_TogglePin(GPIOD, GPIO_PIN_12);
			}
			setWaitingTime(0);
			releasePointing(3);
		} else {
			if (sw3.flag == TRUE) {
				sw3.flag = 0;
				rsp = SHORT;
				HAL_GPIO_TogglePin(GPIOC, GPIO_PIN_6);
			}
		}
		break;
	case ALARM_TRIGGER:
		if(sw3.state == TRUE) {
			if(sw3.flag == FALSE) {
				sw3.flag = 1;
			}
		} else {
			if(sw3.flag == TRUE) {
				returnToPreviousMode();
				buzReset();
				sw3.flag = 0;
			}
		}
		break;
	}
}

void sw4Controll() {
	// SW4 PD10
	switch (mode) {
	case CLOCK:		// 시계모드 일때 PD10 기능
		if (sw4.state == TRUE) {
			if (sw4.flag == FALSE) {
				sw4.flag = 1;
				sw4DefaultHandler();
				HAL_GPIO_TogglePin(GPIOD, GPIO_PIN_12);
			}
			releasePointing(4);
		} else {
			if (sw4.flag == TRUE) {
				HAL_GPIO_TogglePin(GPIOC, GPIO_PIN_6);
				sw4.flag = 0;
			}
		}
		break;
	case ALARM:	// 알람 모드일 때 PD10 기능
		if (sw4.state == TRUE) {
			if (sw4.flag == FALSE) {
				sw4.flag = 1;
				setWaitingTime(0);
				sw4DefaultHandler();
				HAL_GPIO_TogglePin(GPIOD, GPIO_PIN_12);
			}
			releasePointing(4);
		} else {
			if (sw4.flag == TRUE) {
				if(rsp == SHORT) {
					setAlarmRepeat(!getAlarmRepeat(getSelect()), getSelect());
					// 해당번호의 알람반복 활성화 / 비활성화
				}
				sw4.flag = 0;
				HAL_GPIO_TogglePin(GPIOC, GPIO_PIN_6);
			}
		}
		break;
	case STOPWATCH: // 스탑워치 모드일 때 PD10 기능
		// 저장된 laptime 출력
		if (sw4.state == TRUE) {
			if (sw4.flag == FALSE) {
				laptimeDisplay(); // 랩타임 순서대로 출력
				setWaitingTime(0);
				sw4DefaultHandler();
				HAL_GPIO_TogglePin(GPIOD, GPIO_PIN_12);
				sw4.flag = 1;
			}
		} else {
			if (sw4.flag == TRUE) {
				sw4.flag = 0;
				HAL_GPIO_TogglePin(GPIOC, GPIO_PIN_6);
			}
		}
		break;
	case CLOCK_SETTING:  // 시계설정모드 일때 PD10 기능
		if (sw4.state == TRUE) {
			if (sw4.flag == FALSE) {
				sw4.flag = 1;
				sw4DefaultHandler();
				decrementTime();
				HAL_GPIO_TogglePin(GPIOD, GPIO_PIN_12);
			}
			setWaitingTime(0);
			releasePointing(4);
		} else {
			if (sw4.flag == TRUE) {
				sw4.flag = 0;
				rsp = SHORT;
				HAL_GPIO_TogglePin(GPIOC, GPIO_PIN_6);
			}
		}
		break;
	case ALARM_SETTING:
		if(sw4.state == TRUE) {
			if(sw4.flag == FALSE) {
				sw4.flag = 1;
				sw4DefaultHandler();
				decrementAlarmSetting();
				HAL_GPIO_TogglePin(GPIOD, GPIO_PIN_12);
			}
			setWaitingTime(0);
			releasePointing(4);
		} else {
			if(sw4.flag == TRUE) {
				sw4.flag = 0;
				rsp = SHORT;
				HAL_GPIO_TogglePin(GPIOC, GPIO_PIN_6);
			}
		}
		break;
	case ALARM_TRIGGER:
		if(sw4.state == TRUE) {
			if(sw4.flag == FALSE) {
				sw4.flag = 1;
			}
		} else {
			if(sw4.flag == TRUE) {
				returnToPreviousMode();
				buzReset();
				sw4.flag = 0;
			}
		}
	}
}

int getPressCount(int swNum) {
	int temp;
	switch(swNum) {
	case 1:
		temp = sw1.pressCnt;
		break;
	case 2:
		temp = sw2.pressCnt;
		break;
	case 3:
		temp = sw3.pressCnt;
		break;
	case 4:
		temp = sw4.pressCnt;
		break;
	}
	return temp;

}

void holdEvent() {
	if(sw1.state == TRUE) {
		if(sw1.pressCnt <= LONG_PRESS) {
			sw1.pressCnt++;
		}
		switch(rsp) {
		case SHORT:
			break;
		case MID:
			if(sys.millisecond % 150 == 0) {
			}
			break;
		case LONG:
			if(sys.millisecond % 20 == 0) {
			}
			break;

		}
	}
	if(sw2.state == TRUE) {
		if(sw2.pressCnt <= LONG_PRESS) {
			sw2.pressCnt++;
		}
		switch(rsp) {
		case SHORT:
			break;
		case MID:
			if(sys.millisecond % 150 == 0) {
			}
			break;
		case LONG:
			if(sys.millisecond % 20 == 0) {
			}
			break;

		}
	}
	if(sw3.state == TRUE) {
		if(sw3.pressCnt <= LONG_PRESS) {
			sw3.pressCnt++;
		}
		if(sw3.pressCnt == MID_PRESS && (mode == CLOCK_SETTING || mode == ALARM_SETTING)) {
			playMidTone();
		} else if(sw3.pressCnt == LONG_PRESS && (mode == CLOCK_SETTING || mode == ALARM_SETTING)) {
			playHighTone();
		}
		switch(rsp) {
		case SHORT:
			break;
		case MID:
			if(sys.millisecond % 150 == 0) {
				if(mode == CLOCK_SETTING) {
					incrementTime();
				}
				if(mode == ALARM_SETTING) {
					incrementAlarmSetting();
				}
			}
			break;
		case LONG:
			if(sys.millisecond % 20 == 0) {
				if(mode == CLOCK_SETTING) {
					incrementTime();
				}
				if(mode == ALARM_SETTING) {
					incrementAlarmSetting();
				}
			}
			break;

		}
	}
	if(sw4.state == TRUE) {
		if(sw4.pressCnt <= LONG_PRESS) {
			sw4.pressCnt++;
		}
		if(sw4.pressCnt == MID_PRESS && (mode == CLOCK_SETTING || mode == ALARM_SETTING)) {
			playMidTone();
		} else if(sw4.pressCnt == LONG_PRESS && (mode == CLOCK_SETTING || mode == ALARM_SETTING)) {
			playHighTone();
		}
		switch(rsp) {
		case SHORT:
			break;
		case MID:
			if(sys.millisecond % 150 == 0) {
				if(mode == CLOCK_SETTING) {
					decrementTime();
				}
				if(mode == ALARM_SETTING) {
					decrementAlarmSetting();
				}
			}
			break;
		case LONG:
			if(sys.millisecond % 20 == 0) {
				if(mode == CLOCK_SETTING) {
					decrementTime();
				}
				if(mode == ALARM_SETTING) {
					decrementAlarmSetting();
				}
			}
			break;

		}
	}
}
void releasePointing(int swNum) {
		int cnt = getPressCount(swNum);
		if(cnt < MID_PRESS) {
			rsp = SHORT;
		} else if(cnt > MID_PRESS && cnt < LONG_PRESS) {
			rsp = MID;
		} else {
			rsp = LONG;
		}
}

void sw1DefaultHandler() {
	sw1.pressCnt = 0;
	if(getBuzzEnabled() == TRUE) {
		setbuzzerOnOff(1);		   // 부저 on
	}
}
void sw2DefaultHandler() {
	sw2.pressCnt = 0;
	if(getBuzzEnabled() == TRUE) {
		setbuzzerOnOff(1);		   // 부저 on
	}
}
void sw3DefaultHandler() {
	sw3.pressCnt = 0;
	if(getBuzzEnabled() == TRUE) {
		setbuzzerOnOff(1);		   // 부저 on
	}
}
void sw4DefaultHandler() {
	sw4.pressCnt = 0;
	if(getBuzzEnabled() == TRUE) {
		setbuzzerOnOff(1);		   // 부저 on
	}
}

int getSystemMillisecond() {
	return sys.millisecond;
}

uint8_t getBlink() {
	return sys.blink;
}

int getWaitingTime() {
	return sys.waitingTime;
}

void setWaitingTime(int t) {
	sys.waitingTime = t;
}

void systemMillisecondCount() {
	if(sys.millisecond < 1000) {
		sys.millisecond++;
	} else {
		sys.millisecond = 0;
	}
}

void blinking() {
	if(sys.millisecond > 500) {
		sys.blink = 1;
	} else {
		sys.blink = 0;
	}
}
void waitingTimeCnt() {
	if(mode == CLOCK_SETTING || mode == ALARM || mode == ALARM_SETTING || (mode == STOPWATCH && stopwatch.state == STOP)){
		if(sys.waitingTime <= 30000) { // 30초 대기
			sys.waitingTime++;
		} else if((mode == CLOCK_SETTING || mode == ALARM || mode == STOPWATCH) && sys.waitingTime > 30000){
			if(mode == CLOCK_SETTING) {
				HAL_GPIO_WritePin(GPIOB, GPIO_PIN_5, 1);
			}
			setbuzzerOnOff(1);
			sys.waitingTime = 0;
			mode = CLOCK;
		} else if(mode == ALARM_SETTING && sys.waitingTime > 30000) {
			setbuzzerOnOff(1);
			sys.waitingTime = 0;
			  HAL_GPIO_WritePin(GPIOB, GPIO_PIN_5, 1);
			mode = ALARM;
		}
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




