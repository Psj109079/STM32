/*
 * uartLEDControll.c
 *
 *  Created on: Oct 20, 2024
 *      Author: ckck
 */
#include "uartLEDControll.h"

extern uint8_t select_LED;
uartRx uartRxfd;

void uartLedControll() {
	if (uartRxfd.uart3_rx_flag == 1) {
		uartRxfd.uart3_rx_flag = 0;
		switch (uartRxfd.uart3_rx_data) {
		case '1':
			if (!select_LED) {
				if (HAL_GPIO_ReadPin(GPIOD, GPIO_PIN_12)) {
					HAL_GPIO_WritePin(GPIOD, GPIO_PIN_12, 0);
				} else {
					HAL_GPIO_WritePin(GPIOD, GPIO_PIN_12, 1);
				}
			} else {
				if (HAL_GPIO_ReadPin(GPIOC, GPIO_PIN_6)) {
					HAL_GPIO_WritePin(GPIOC, GPIO_PIN_6, 0);
				} else {
					HAL_GPIO_WritePin(GPIOC, GPIO_PIN_6, 1);
				}
			}
			break;
		case '2':
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
			break;
		case '3':
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
			break;
		case '4':
			select_LED = !select_LED;
			break;
		}

	}
}

