/*
 * stopWatch.c
 *
 *  Created on: Oct 19, 2024
 *      Author: ckck
 */

#include "stopWatch.h"
stopWatchTime pausedTimer = {0, 0, 0, 0};
extern stopWatch stopwatch;

void segUpCount() {
	// 스탑워치 시작 함수
	if(stopwatch.state == RUN) {
		_7SEG_SetNumber(DGT1, stopwatch.time.second % 10, getBlink());
	} else {
		_7SEG_SetNumber(DGT1, stopwatch.time.second % 10, 0);
	}
	_7SEG_SetNumber(DGT2, stopwatch.time.millisecond / 100, 0);

}

void segReset() {
	// 7세그먼트 초기화 함수
	_7SEG_SetNumber(DGT1, 0, 1);
	_7SEG_SetNumber(DGT2, 0, 0);
}

void timerReset() {
	// 스탑워치 시간 초기화 함수
	stopwatch.time.hour = 0;
	stopwatch.time.minute = 0;
	stopwatch.time.second = 0;
	stopwatch.time.millisecond = 0;
}

void timerPaused() {
	// 스탑워치 일시정지 함수
	pausedTimer.hour = stopwatch.time.hour;
	pausedTimer.minute = stopwatch.time.minute;
	pausedTimer.second = stopwatch.time.second;
	pausedTimer.millisecond = stopwatch.time.millisecond;
}

void timerResum() {
	// 스탑워치 재개 함수
	stopwatch.time.hour = pausedTimer.hour;
	stopwatch.time.minute = pausedTimer.minute;
	stopwatch.time.second = pausedTimer.second;
	stopwatch.time.millisecond = pausedTimer.millisecond;
}

void stopwatchTime() {
	// 스탑워치 시간 저장함수
	if(stopwatch.state == RUN) {
		stopwatch.time.millisecond++;
		if(stopwatch.time.millisecond % 1000 == 0) {
			stopwatch.time.millisecond = 0;
			stopwatch.time.second++;
		}
		if(stopwatch.time.second == 60) {
			stopwatch.time.second = 0;
			stopwatch.time.minute++;
		}
		if(stopwatch.time.minute == 60) {
			stopwatch.time.minute = 0;
			stopwatch.time.hour++;
		}
		if(stopwatch.time.hour == 24) {
			stopwatch.time.hour = 0;
		}
	}
}

void laptimeSave() {
	// 랩타임 저장 함수
	if(stopwatch.laptime.cnt < 9) {
		stopwatch.laptime.saveSpace[stopwatch.laptime.cnt].hour  = stopwatch.time.hour;
		stopwatch.laptime.saveSpace[stopwatch.laptime.cnt].minute = stopwatch.time.minute;
		stopwatch.laptime.saveSpace[stopwatch.laptime.cnt].second = stopwatch.time.second;
		stopwatch.laptime.saveSpace[stopwatch.laptime.cnt].millisecond = stopwatch.time.millisecond;

		sprintf(stopwatch.laptime.buffer, "LP%d %02d:%02d:%02d.%03d", // 랩타임 저장
				(stopwatch.laptime.cnt + 1),
				stopwatch.laptime.saveSpace[stopwatch.laptime.cnt].hour,
				stopwatch.laptime.saveSpace[stopwatch.laptime.cnt].minute,
				stopwatch.laptime.saveSpace[stopwatch.laptime.cnt].second,
				stopwatch.laptime.saveSpace[stopwatch.laptime.cnt].millisecond);
		CLCD_Puts(0, 1, stopwatch.laptime.buffer); // 랩타임 출력
		stopwatch.laptime.cnt++; // 카운트 증가
	} else if(stopwatch.laptime.cnt >= 9) {

		CLCD_Puts(0, 1, "                ");
		CLCD_Puts(0, 1, "LAP FULL(9/9)");
	}
}

void laptimeDisplay() {
	// CLCD 랩타임 출력 함수
	if(stopwatch.laptime.cnt > 0) {

		if(stopwatch.laptime.cnt <= stopwatch.laptime.displayCnt) {
			stopwatch.laptime.displayCnt = 0;
		}

		sprintf(stopwatch.laptime.buffer, "%d/%d %02d:%02d:%02d.%03d",
				(stopwatch.laptime.displayCnt + 1), stopwatch.laptime.cnt,
				stopwatch.laptime.saveSpace[stopwatch.laptime.displayCnt].hour,
				stopwatch.laptime.saveSpace[stopwatch.laptime.displayCnt].minute,
				stopwatch.laptime.saveSpace[stopwatch.laptime.displayCnt].second,
				stopwatch.laptime.saveSpace[stopwatch.laptime.displayCnt].millisecond);

		CLCD_Puts(0, 1,stopwatch.laptime.buffer);

		stopwatch.laptime.displayCnt++;

	} else {
		CLCD_Puts(0, 1, "                ");
		CLCD_Puts(0, 1, "NO LAP");
	}

}

void clcdStopWatchClear() {
	// CLCD 스탑워치 초기화 함수
	for(int i = 0; i < 9; i++) {
		// 랩타임 초기화
		stopwatch.laptime.saveSpace[i].hour = 0;
		stopwatch.laptime.saveSpace[i].minute = 0;
		stopwatch.laptime.saveSpace[i].second = 0;
		stopwatch.laptime.saveSpace[i].millisecond = 0;
	}
	stopwatch.laptime.cnt = 0;
	stopwatch.laptime.displayCnt = 0;
	timerReset();
	CLCD_Clear();
}

void clcdStopWatchMeasure() {
	// CLCD 스탑워치 출력함수
	if(getBlink() == TRUE || stopwatch.state != RUN) {
		sprintf(stopwatch.laptime.buffer, "STW %02d:%02d:%02d.%03d",
					stopwatch.time.hour, stopwatch.time.minute,
					stopwatch.time.second, stopwatch.time.millisecond);
	} else {
		sprintf(stopwatch.laptime.buffer, "STW %02d:%02d:%02d %03d",
					stopwatch.time.hour, stopwatch.time.minute,
					stopwatch.time.second, stopwatch.time.millisecond);
	}
	CLCD_Puts(0, 0, stopwatch.laptime.buffer);
}

