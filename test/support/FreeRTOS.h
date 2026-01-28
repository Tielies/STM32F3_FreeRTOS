#ifndef INC_FREERTOS_H
#define INC_FREERTOS_H

#include <stdint.h>
#include <stddef.h>

#define BaseType_t long
#define UBaseType_t unsigned long
#define TickType_t uint32_t

#define pdFALSE ((BaseType_t) 0)
#define pdTRUE  ((BaseType_t) 1)
#define pdPASS  (pdTRUE)
#define pdFAIL  (pdFALSE)

#define configASSERT( x )

#define portYIELD_FROM_ISR(x) ((void)x)

#endif
