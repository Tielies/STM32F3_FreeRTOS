/*
 * printf.h
 *
 *  Created on: Jan 26, 2026
 *      Author: meyert
 */

#ifndef INC_CONSOLE_H_
#define INC_CONSOLE_H_

#include "stdint.h"

#define MAX_MSG_LEN 128
#define MAX_LOG_LEN 4

typedef struct {
	char msg[MAX_MSG_LEN];
} printMsg_t;

void consoleTask(void *argument);
int consoleSend(char *buf, uint16_t len);

#endif /* INC_CONSOLE_H_ */
