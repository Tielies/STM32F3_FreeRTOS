/*
 * printf.h
 *
 *  Created on: Jan 26, 2026
 *      Author: meyert
 */

#ifndef INC_PRINTF_H_
#define INC_PRINTF_H_

#include "stdint.h"

#define MAX_MSG_LEN 128
#define MAX_LOG_LEN 4

typedef struct {
	char msg[MAX_MSG_LEN];
} printMsg_t;

void printfTask(void *argument);
int printfSend(char *buf, uint16_t len);

#endif /* INC_PRINTF_H_ */
