/*
 * printf.c
 *
 *  Created on: Jan 26, 2026
 *      Author: meyert
 */

#include "printf.h"
#include "FreeRTOS.h"
#include "stream_buffer.h"
#include "usbd_cdc_if.h"
#include "task.h"

static StreamBufferHandle_t logStream;

void printfTask(void *argument)
{
	char buf[64];
	logStream = xStreamBufferCreate(512, 1);
	configASSERT(logStream != NULL);
	for(;;)
	{
        // Don't block here - just check if there's data and send it out
        size_t n = xStreamBufferReceive(logStream, buf, sizeof(buf), 0);
        CDC_Transmit_FS((uint8_t*)buf, n);
	}
}

int printfSend(char *buf, uint16_t len)
{
    // Check if data being sent from task context
    TaskHandle_t xTask = xTaskGetCurrentTaskHandle();
    if (xTask)
    {
        size_t n = xStreamBufferSend(logStream, buf, len, 0);
        return n == len ? 0 : -1;
    }
    else
    {
        BaseType_t xHigherPriorityTaskWoken = pdFALSE;
        size_t n = xStreamBufferSendFromISR(logStream, buf, len, &xHigherPriorityTaskWoken);
        portYIELD_FROM_ISR(xHigherPriorityTaskWoken);
        return n == len ? 0 : -1;
    }
}


