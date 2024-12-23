/*
 * clock.c
 *
 *  Created on: Nov 3, 2024
 *      Author: ckck
 */

#include "clock.h"

extern clockSt clock;
extern modeSelector mode;
int thirtyDays[4] = {4, 6, 9, 11};
clockSetting clockSet = SECOND;

void startClock() {
	// 시간 증가 함수
	clock.millisecond++;
	if(clock.millisecond == 1000) {
		clock.millisecond = 0;
		clock.second++;
	}
	if(clock.second == 60) {
		clock.second = 0;
		clock.minute++;
	}
	if(clock.minute == 60) {
		clock.minute = 0;
		clock.hour++;
		playMidTone();	// 정각에 부저 mid tone으로 울리는 함수
	}
	if(clock.hour == 24) {
		clock.hour = 0;
		clock.date++;
		playHighTone(); // 매일 0시에 부저 high tone으로 울리는 함수
	}
	if(clock.month == 2) {
		if(clock.leapYear == TRUE) {
			if(clock.date > 29) {
				clock.date = 1;
				clock.month++;
				clock.thiryMonth = isThirtyDays(clock.month);
			}
		} else {
			if(clock.date > 28) {
				clock.date = 1;
				clock.month++;
				clock.thiryMonth = isThirtyDays(clock.month);
			}
		}
	} else if(clock.thiryMonth == TRUE) { // 말일이 30일인 달 4, 6, 9, 11
		if(clock.date > 30) {
			clock.date = 1;
			clock.month++;
			clock.thiryMonth = isThirtyDays(clock.month);
		}
	} else if(clock.thiryMonth == FALSE) { // 말일이 31일인 달 1, 3, 5, 7, 8, 10, 12
		if(clock.date > 31) {
			clock.date = 1;
			clock.month++;
			clock.thiryMonth = isThirtyDays(clock.month);
		}
	}

	if(clock.month > 12) {
		clock.month = 1;
		clock.year++;
		clock.leapYear = leapYearCalculator(clock.year);
	}
}

void clcdDisplayClock() {
	// 시계기능 출력
	uint8_t buzzOnOff[8];
	uint8_t formet[12];
	if(clock.leapYear == TRUE) {
		sprintf(clock.buffer, "LEAP  %4d.%2d.%2d", clock.year, clock.month, clock.date);
	} else {
		sprintf(clock.buffer, "      %4d.%2d.%2d", clock.year, clock.month, clock.date);
	}
	if(getBuzzEnabled() == TRUE) {	// 부저 상태 출력
		sprintf(buzzOnOff, "BZ ON ");

	} else {
		sprintf(buzzOnOff, "BZ OFF");
	}

	if(getTimeFormet() == TRUE) {  // 24시간 포멧일경우
		if(getBlink() == TRUE) {
			sprintf(formet, "   %02d:%02d", clock.hour, clock.minute);
		} else { 	// 콜론X
			sprintf(formet, "   %02d %02d", clock.hour, clock.minute);
		}
	} else {					// AM/PM (12시간 포멧) 일경우
		if(getBlink() == TRUE) {
			if(clock.hour < 12) {	// AM
				if(clock.hour == 0) {
					// 24시 일때
					sprintf(formet, "AM %02d:%02d", 12, clock.minute);
				} else {
					sprintf(formet, "AM %02d:%02d", clock.hour, clock.minute);
				}
			} else {				// PM
				if(clock.hour == 12) {
					// 12시 일때
					sprintf(formet, "PM %02d:%02d", 12, clock.minute);
				} else {
					sprintf(formet, "PM %02d:%02d", (clock.hour - 12), clock.minute);
				}
			}
		} else {	// 콜론X
			if(clock.hour < 12) {	// AM
				if (clock.hour == 0) {
					// 24시 일때
					sprintf(formet, "AM %02d %02d", 12, clock.minute);
				} else {
					sprintf(formet, "AM %02d %02d", clock.hour, clock.minute);
				}
			} else {				// PM
				if (clock.hour == 12) {
					// 12시 일때
					sprintf(formet, "PM %02d %02d", 12, clock.minute);
				} else {
					sprintf(formet, "PM %02d %02d", (clock.hour - 12), clock.minute);
				}
			}
		}
	}
	CLCD_Puts(0, 0, clock.buffer);
	sprintf(clock.buffer, "%s  %s", buzzOnOff, formet);
	CLCD_Puts(0, 1, clock.buffer);
}

void tickClock() {
	// 7세그먼트에 초 단위 출력
	_7SEG_SetNumber(DGT1, clock.second / 10, 0);
	_7SEG_SetNumber(DGT2, clock.second % 10, getBlink());

}

int leapYearCalculator(int year) {
	// 윤년 계산
	if(year % 4 == 0){
		if(year % 100 == 0) {
			if(year % 400 == 0) {
				return TRUE; // 4, 100, 400 으로 나누어 떨어지면 윤년
			} else {
				return FALSE; // 4, 100 으로 나누어 떨어지고 400으로 나누어 떨어지지 않으면 평년
			}
		} else {
			return TRUE;	// 4로 나누어 떨어지고 100으로 나누어 떨어지지 않으면 윤년
		}
	} else {
		return FALSE;	// 4로 나누어 떨어지지 않으면 평년
	}
}

int isThirtyDays(int month) {
	// 30일로 끝나는 달 구분
	int bool = FALSE;
	for(int i = 0; i < sizeof(thirtyDays) / sizeof(int); i++) {
		if(thirtyDays[i] == month) {
			bool = TRUE;
			break;
		}
	}
	return bool;
}

void switchClockMode() { // 시계모드에서 시계설정 모드전환 함수
	if(mode == CLOCK && getPressCount(1) == 700) {
		// 시계모드 이면서 누른 시간이 700 인경우
		mode = CLOCK_SETTING;
		HAL_GPIO_TogglePin(GPIOB, GPIO_PIN_5);
		playMidTone();

	} else if((mode == CLOCK_SETTING && getPressCount(1) == 700)) {
		// 시계설정모드 이면서 누른 시간이 700 인 경우
		mode = CLOCK;
		HAL_GPIO_TogglePin(GPIOB, GPIO_PIN_5);
		playMidTone();
		setWaitingTime(0);
	}
}



void clcdDisplayClockSetting() { // 시계설정모드 clcd출력
	// 0.5초 마다 점멸
	switch(clockSet) {
	case SECOND:
		sprintf(clock.buffer, "CLOCK %4d.%2d.%2d", clock.year, clock.month, clock.date);
		CLCD_Puts(0, 0, clock.buffer);
		if(getWaitingTime() < 200|| getBlink() == TRUE) {
			sprintf(clock.buffer, "SET     %02d:%02d:%02d", clock.hour, clock.minute, clock.second);
		} else {
			sprintf(clock.buffer, "SET     %02d:%02d:  ", clock.hour, clock.minute);
		}
		CLCD_Puts(0, 1, clock.buffer);
		break;
	case MINUTE:
		sprintf(clock.buffer, "CLOCK %4d.%2d.%2d", clock.year, clock.month, clock.date);
		CLCD_Puts(0, 0, clock.buffer);
		if(getWaitingTime() < 200 || getBlink() == TRUE) {
			sprintf(clock.buffer, "SET     %02d:%02d:%02d", clock.hour, clock.minute, clock.second);
		} else {
			sprintf(clock.buffer, "SET     %02d:  :%02d", clock.hour, clock.second);
		}
		CLCD_Puts(0, 1, clock.buffer);
		break;
	case HOUR:
		sprintf(clock.buffer, "CLOCK %4d.%2d.%2d", clock.year, clock.month, clock.date);
		CLCD_Puts(0, 0, clock.buffer);
		if(getWaitingTime() < 200 || getBlink() == TRUE) {
			sprintf(clock.buffer, "SET     %02d:%02d:%02d", clock.hour, clock.minute, clock.second);
		} else {
			sprintf(clock.buffer, "SET       :%02d:%02d", clock.minute, clock.second);
		}
		CLCD_Puts(0, 1, clock.buffer);
		break;
	case DATE:
		if(getWaitingTime() < 200 || getBlink() == TRUE) {
			sprintf(clock.buffer, "CLOCK %4d.%2d.%2d", clock.year, clock.month, clock.date);
		} else {
			sprintf(clock.buffer, "CLOCK %4d.%2d.  ", clock.year, clock.month);
		}
		CLCD_Puts(0, 0, clock.buffer);
		sprintf(clock.buffer, "SET     %02d:%02d:%02d", clock.hour, clock.minute, clock.second);
		CLCD_Puts(0, 1, clock.buffer);
		break;
	case MONTH:
		if(getWaitingTime() < 200 || getBlink() == TRUE) {
			sprintf(clock.buffer, "CLOCK %4d.%2d.%2d", clock.year, clock.month, clock.date);
		} else {
			sprintf(clock.buffer, "CLOCK %4d.  .%2d", clock.year, clock.date);
		}
		CLCD_Puts(0, 0, clock.buffer);
		sprintf(clock.buffer, "SET     %02d:%02d:%02d", clock.hour, clock.minute, clock.second);
		CLCD_Puts(0, 1, clock.buffer);
		break;
	case YEAR:
		if(getWaitingTime() < 200 || getBlink() == TRUE) {
			sprintf(clock.buffer, "CLOCK %4d.%2d.%2d", clock.year, clock.month, clock.date);
		} else {
			sprintf(clock.buffer, "CLOCK     .%2d.%2d", clock.month, clock.date);
		}
		CLCD_Puts(0, 0, clock.buffer);
		sprintf(clock.buffer, "SET     %02d:%02d:%02d", clock.hour, clock.minute, clock.second);
		CLCD_Puts(0, 1, clock.buffer);
		break;
	}
}

void settingTime() {	// 버튼 2번 동작 설정할 단위 변경
	if(clockSet == YEAR) {
		clockSet = SECOND;
	} else {
		clockSet++;
	}
}

void incrementTime() {	// 버튼 3번동작 (시간 증가)
	switch(clockSet) {
	case SECOND:
		clock.second = 0;
		clock.millisecond = 0;
		break;
	case MINUTE:
		if(clock.minute == 59) {
			clock.minute = 0;
		} else {
			clock.minute++;
		}
		break;
	case HOUR:
		if(clock.hour == 23) {
			clock.hour = 0;
		} else {
			clock.hour++;
		}
		break;
	case DATE:
		clock.date++;
		if(clock.month == 2) {
			if(clock.leapYear == TRUE) {
				if(clock.date > 29) {
					clock.date = 1;
				}
			} else {
				if(clock.date > 28) {
					clock.date = 1;
				}
			}
		} else if(clock.thiryMonth == TRUE) { // 말일이 30일인 달 4, 6, 9, 11
			if(clock.date > 30) {
				clock.date = 1;
			}
		} else if(clock.thiryMonth == FALSE) { // 말일이 31일인 달 1, 3, 5, 7, 8, 10, 12
			if(clock.date > 31) {
				clock.date = 1;
			}
		}
		break;
	case MONTH:
		clock.month++;
		clock.thiryMonth = isThirtyDays(clock.month);
		if(clock.month == 2) {
			if(clock.leapYear == TRUE) {
				if(clock.date > 29) {
					clock.date = 29;
				}
			} else {
				if(clock.date > 28) {
					clock.date = 28;
				}
			}
		} else if(clock.thiryMonth == TRUE) {
			if(clock.date > 30) {
				clock.date = 30;
			}
		}
		if(clock.month > 12) {
			clock.month = 1;
		}
		break;
	case YEAR:
		clock.year++;
		clock.leapYear = leapYearCalculator(clock.year);
		if(clock.leapYear == FALSE) {
			if(clock.month == 2 && clock.date > 28) {
				clock.date = 28;
			}
		}
		break;
	}
}

void decrementTime() {	// 버튼 4번동작 (시간 감소)
	switch(clockSet) {
	case SECOND:
		clock.second = 0;
		clock.millisecond = 0;
		break;
	case MINUTE:
		if(clock.minute == 0) {
			clock.minute = 59;
		} else {
			clock.minute--;
		}
		break;
	case HOUR:
		if(clock.hour == 0) {
			clock.hour = 23;
		} else {
			clock.hour--;
		}
		break;
	case DATE:
		clock.date--;
		if(clock.month == 2) {
			if(clock.leapYear == TRUE) {
				if(clock.date < 1) {
					clock.date = 29;
				}
			} else {
				if(clock.date < 1) {
					clock.date = 28;
				}
			}
		} else if(clock.thiryMonth == TRUE) { // 말일이 30일인 달 4, 6, 9, 11
			if(clock.date < 1) {
				clock.date = 30;
			}
		} else if(clock.thiryMonth == FALSE) { // 말일이 31일인 달 1, 3, 5, 7, 8, 10, 12
			if(clock.date < 1) {
				clock.date = 31;
			}
		}
		break;
	case MONTH:
		clock.month--;
		clock.thiryMonth = isThirtyDays(clock.month);
		if(clock.month == 2) {
			if(clock.leapYear == TRUE) {
				if(clock.date > 29) {
					clock.date = 29;
				}
			} else {
				if(clock.date > 28) {
					clock.date = 28;
				}
			}
		} else if(clock.thiryMonth == TRUE) {
			if(clock.date > 30) {
				clock.date = 30;
			}
		}
		if(clock.month < 1) {
			clock.month = 12;
		}
		break;
	case YEAR:
		if(clock.year > 1900) {
			clock.year--;
		}
		clock.leapYear = leapYearCalculator(clock.year);
		if(clock.leapYear == FALSE) {
			if(clock.month == 2 && clock.date > 28) {
				clock.date = 28;
			}
		}
		break;
	}

}

uint8_t getTimeFormet() {
	return clock.timeFormet;
}

void setTimeFormet(int onOff) {
	clock.timeFormet = onOff;
}
