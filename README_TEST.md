# Unit Testing with Ceedling setup

This project is set up to use [Ceedling](http://www.throwtheswitch.org/ceedling) for unit tests.
The tests run inside a Docker container to ensure a consistent environment without installing Ruby/Ceedling locally.

## Directory Structure

*   `test/`: Contains unit tests and support files.
    *   `test/support/`: specific headers for mocking embedded and RTOS dependencies.
    *   `test_command.c`: Unit tests for `command.c`.
*   `project.yml`: Ceedling configuration file.

## Dependencies

*   Docker

## Running Tests

To run the tests, execute the `test.ps1` script from PowerShell:

```powershell
.\test.ps1
```

Or just run the Docker command directly:

```powershell
docker run --rm -v "${PWD}:/project" throwtheswitch/ceedling ceedling test:all
```

## How it works

1.  **Mocking Dependencies**: Since we are running on a PC, we cannot use the real STM32 HAL or FreeRTOS source code.
    *   We provide "fake" headers in `test/support` (e.g., `stm32f3xx_hal.h`, `FreeRTOS.h`, `stream_buffer.h`).
    *   These headers satisfy the include requirements of your source code and allow CMock to generate mocks for function calls (like `HAL_GPIO_TogglePin` or `xStreamBufferSendFromISR`).
2.  **Including Source**: In `test_command.c`, we `#include "command.c"`. This allows us to test static functions and variables (like `processCommand` internal logic).
3.  **Project YAML**: Configured to include `Core/Inc` and `test/support`, but NOT the real Drivers folder, ensuring we use our mocks.
