/*
 * printf.c
 *
 *  Created on: Jan 26, 2026
 *      Author: meyert
 */

#include <console.h>
#include "cmsis_os.h"
#include "stream_buffer.h"
#include "usbd_cdc_if.h"
#include "task.h"
#include "usb_device.h"

static StreamBufferHandle_t logStream;

void consoleTask(void *argument)
{
    char buf[64];
    uint8_t status;
    logStream = xStreamBufferCreate(512, 1);
    /* init code for USB_DEVICE */
    MX_USB_DEVICE_Init();
    do
    {
        osDelay(1000);
    } while (!isUsbReady());
    osDelay(1000);
    const char* test = "Hello world!";
    do {
      status = CDC_Transmit_FS((uint8_t *) test, strlen(test));
    } while (status == USBD_BUSY);
    configASSERT(logStream != NULL);
    for(;;)
    {
        // Don't block here - just check if there's data and send it out
        size_t n = xStreamBufferReceive(logStream, buf, sizeof(buf), 0);
        if (n != 0) {
          CDC_Transmit_FS((uint8_t*)buf, n);
        }
    }
}

int consoleSend(char *buf, uint16_t len)
{
    // Check if data being sent from task context
    TaskHandle_t xTask = xTaskGetCurrentTaskHandle();
    // Add critical section to prevent USB ISR and task context corruption of the stream buffer
    if (xTask)
    {
        taskENTER_CRITICAL();
        size_t n = xStreamBufferSend(logStream, buf, len, 0);
        taskEXIT_CRITICAL();
        return n == len ? 0 : -1;
    }
    else
    {
        BaseType_t xHigherPriorityTaskWoken = pdFALSE;
        taskENTER_CRITICAL();
        size_t n = xStreamBufferSendFromISR(logStream, buf, len, &xHigherPriorityTaskWoken);
        taskEXIT_CRITICAL();
        portYIELD_FROM_ISR(xHigherPriorityTaskWoken);
        return n == len ? 0 : -1;
    }
}


