/*
 * uartLEDControll.h
 *
 *  Created on: Oct 20, 2024
 *      Author: ckck
 */

#ifndef INC_UARTLEDCONTROLL_H_
#define INC_UARTLEDCONTROLL_H_

#include "main.h"
#include "swControll.h"

typedef struct _uartRx {
	uint8_t uart3_rx_data;
	uint8_t uart3_rx_flag;
} uartRx;

void uartLedControll();

#endif /* INC_UARTLEDCONTROLL_H_ */
