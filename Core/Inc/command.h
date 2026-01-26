/*
 * command.h
 *
 *  Created on: Jan 26, 2026
 *      Author: meyert
 */

#ifndef INC_COMMAND_H_
#define INC_COMMAND_H_

#include <stdint.h>

void commandTask(void *argument);

void commandReceiveISR(char *cmd, uint16_t len);

#endif /* INC_COMMAND_H_ */
