#include "unity.h"
#include "FreeRTOS.h"
#include "task.h"

#include "mock_stream_buffer.h"
#include "mock_stm32f3xx_hal.h"

#include "command.h"

/* Include source to test static functions */
#include "command.c"

void setUp(void)
{
    /* Reset static variables if necessary */
    xStreamBuffer = (StreamBufferHandle_t)0;
}

void tearDown(void)
{
}

void test_processCommand_should_toggle_LED4(void)
{
    char cmd[] = "LED4";
    // Length is 4

    HAL_GPIO_TogglePin_Expect(GPIOE, GPIO_PIN_8);

    processCommand(cmd, 4);
}

void test_processCommand_should_toggle_LED5(void)
{
    char cmd[] = "LED5";

    HAL_GPIO_TogglePin_Expect(GPIOE, GPIO_PIN_10);

    processCommand(cmd, 4);
}

void test_processCommand_should_toggle_LED6(void)
{
    char cmd[] = "LED6";

    HAL_GPIO_TogglePin_Expect(GPIOE, GPIO_PIN_15);

    processCommand(cmd, 4);
}

void test_processCommand_should_handle_unknown_command(void)
{
    char cmd[] = "HELLO";

    // Expect no calls to TogglePin
    // printf will output "Unknown command..."

    processCommand(cmd, 5);
}

void test_commandReceiveISR_should_send_to_stream_buffer(void)
{
    char cmd[] = "X";
    uint16_t len = 1;
    StreamBufferHandle_t handle = (StreamBufferHandle_t)0x1234;

    xStreamBuffer = handle;

    // Expect send
    // We ignore the pxHigherPriorityTaskWoken argument because it is a local variable address we can't predict
    BaseType_t xHigherPriorityTaskWoken = pdFALSE;
    xStreamBufferSendFromISR_ExpectAndReturn(handle, (void*)cmd, 1, &xHigherPriorityTaskWoken, 1);

    commandReceiveISR(cmd, len);
}
