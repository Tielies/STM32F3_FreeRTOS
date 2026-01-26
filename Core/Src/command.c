/*
 * command.c
 *
 *  Created on: Jan 26, 2026
 *      Author: meyert
 */

#include "command.h"
#include "FreeRTOS.h"
#include "task.h"
#include "stream_buffer.h"
#include "stdio.h"
#include "stdint.h"
#include "string.h"

static void processCommand(char *cmd, uint16_t len);


static StreamBufferHandle_t xStreamBuffer;

void commandTask(void *argument)
{
  char cmdBuf[64];
  char* cmdPtr = cmdBuf;
  xStreamBuffer = xStreamBufferCreate(256, 1);
  configASSERT(xStreamBuffer != NULL);
  for(;;)
  {
      // Block until we get a command
      size_t n = xStreamBufferReceive(xStreamBuffer, cmdPtr, 1, portMAX_DELAY);
      cmdPtr += n;
      if (cmdPtr - cmdBuf >= sizeof(cmdBuf) - 1)
      {
          // Command too long, reset
          printf("Command too long\r\n");
          cmdPtr = cmdBuf;
          continue;
      }
      if (*cmdPtr == '\n' || *cmdPtr == '\r')
      {
        // Null terminate command
        *cmdPtr = '\0';
        // Process command
        processCommand(cmdBuf, cmdPtr - cmdBuf);
        cmdPtr = cmdBuf;
      }
  }
}


void commandReceiveISR(char *cmd, uint16_t len)
{
  // Receive command from ISR context
  size_t xBytesSent;
  BaseType_t xHigherPriorityTaskWoken = pdFALSE;
  xBytesSent = xStreamBufferSendFromISR(xStreamBuffer, (void *)cmd, len, &xHigherPriorityTaskWoken);
  if (xBytesSent < len)
  {
      // Handle error - command buffer overflow
      printf("Command buffer overflow\r\n");
  }
  portYIELD_FROM_ISR( xHigherPriorityTaskWoken);
}

static void processCommand(char *cmd, uint16_t len)
{
    // Simple command processing example
    if (len >= 4 && strncmp(cmd, "LED1", 4) == 0)
    {
        // Toggle LED1
        printf("Toggling LED1\r\n");
    }
    else if (len >= 4 && strncmp(cmd, "LED2", 4) == 0)
    {
        // Toggle LED2
        printf("Toggling LED2\r\n");
    }
    else
    {
        printf("Unknown command: %.*s\r\n", len, cmd);
    }
}

