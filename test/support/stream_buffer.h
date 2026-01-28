#ifndef STREAM_BUFFER_H
#define STREAM_BUFFER_H

#include "FreeRTOS.h"

typedef struct StreamBufferDef_t * StreamBufferHandle_t;

StreamBufferHandle_t xStreamBufferCreate( size_t xBufferSizeBytes, size_t xTriggerLevelBytes );
size_t xStreamBufferReceive( StreamBufferHandle_t xStreamBuffer, void *pvRxData, size_t xBufferLengthBytes, TickType_t xTicksToWait );
size_t xStreamBufferSendFromISR( StreamBufferHandle_t xStreamBuffer, const void *pvTxData, size_t xDataLengthBytes, BaseType_t *pxHigherPriorityTaskWoken );

#endif
