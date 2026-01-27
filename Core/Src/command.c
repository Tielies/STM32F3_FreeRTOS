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
#include "main.h"


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
      size_t n = xStreamBufferReceive(xStreamBuffer, cmdPtr, 1, 100);
      if (n != 0) {
        for (int i = 0; i < n; i++)
        {
          // echo back the received character
          printf("%c", *(cmdPtr + i));
        }
      } else {
    	  continue;
      }
      // Advance pointer
      cmdPtr += n;
      if (cmdPtr - cmdBuf >= sizeof(cmdBuf) - 1)
      {
          // Command too long, reset
          printf("Command too long\r\n");
          // Reset the pointer
          cmdPtr = cmdBuf;
          continue;
      }
      if (*(cmdPtr - 1) == '\n' || *(cmdPtr - 1) == '\r')
      {
        // Null terminate command
        *(cmdPtr - 1) = '\0';
        // Process command
        processCommand(cmdBuf, cmdPtr - cmdBuf);
        // Reset the pointer
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
    if (len >= 4 && strncmp(cmd, "LED4", 4) == 0)
    {
        // Toggle LED6
        printf("Toggling LED4\r\n");
        HAL_GPIO_TogglePin(LD4_GPIO_Port, LD4_Pin);
    }
    else if (len >= 4 && strncmp(cmd, "LED5", 4) == 0)
    {
        // Toggle LED5
        printf("Toggling LED5\r\n");
        HAL_GPIO_TogglePin(LD5_GPIO_Port, LD5_Pin);
    }
    else if (len >= 4 && strncmp(cmd, "LED6", 4) == 0)
    {
        // Toggle LED6
        printf("Toggling LED6\r\n");
        HAL_GPIO_TogglePin(LD6_GPIO_Port, LD6_Pin);
    }
    else
    {
        printf("Unknown command: %.*s\r\n", len, cmd);
    }
}

