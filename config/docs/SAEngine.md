## Harmony Stream Analyze

### Table of Contents
- [Harmony Stream Analyze](#harmony-stream-analyze)
  - [Table of Contents](#table-of-contents)
  - [Description](#description)
  - [APIs](#apis)
  - [Preconditions](#preconditions)
  - [Usage](#usage)
  - [Building](#building)
  - [Contributing](#contributing)
- [License](#license)

### Description
SA Engine is a lightweight streaming analytics and AI platform designed for edge devices including microcontrollers. It is an end-to-end platform enabling efficient development, training, deployment, and orchestration of analytics, computational, and AI models. For more detailed information, please refer to the [Stream Analyze Documentation](https://studio.streamanalyze.com/docs/overview/intro).

Harmony Stream Analyze integrates a customized SA Engine with the Harmony framework, enabling effortless deployment of the SA Engine on Microchip's MCU32 devices.

### APIs
- [SAEngine_Initialize()](./apis/SAEngine_Initialize.md)
- [SAEngine_TaskStatusEvaluate()](./apis/SAEngine_TaskStatusEvaluate.md)
- [SAEngine_RegisteredTasksRun()](./apis/SAEngine_RegisteredTasksRun.md)
- [SAEngine_SignalInitialize()](./apis/SAEngine_SignalInitialize.md)
- [SAEngine_SensorSampleGet()](./apis/SAEngine_SensorSampleGet.md)
- [SAEngine_TrackTimerCountGet()](./apis/SAEngine_TrackTimerCountGet.md)
- [SAEngine_SerialBufferWrite()](./apis/SAEngine_SerialBufferWrite.md)
- [SAEngine_UartReadEventHandler()](./apis/SAEngine_UartReadEventHandler.md)

### Preconditions
- UART, timer, and interrupts is available.

### Usage 
```c
#include <stddef.h>                     // Defines NULL
#include <stdbool.h>                    // Defines true
#include <stdlib.h>                     // Defines EXIT_FAILURE
#include "definitions.h"                // SYS function prototypes
#include "sa_utilities.h"

// *****************************************************************************
// *****************************************************************************
// Section: Main Entry Point
// *****************************************************************************
// *****************************************************************************
extern float32_t streamSignal_1, streamSignal_2;

int main ( void )
{
    /* Initialize all modules */
    SYS_Initialize ( NULL );


    /* Initialize SA Engine */
    SAEngine_Initialize();

    /* Initialize stream signals */
    SAEngine_SignalInitialize(0u, &streamSignal_1);
    SAEngine_SignalInitialize(1u, &streamSignal_2);

    /* Loop evaluation */
    SAEngine_TaskStatusEvaluate();

    while ( true )
    {
        /* Maintain state machines of all polled MPLAB Harmony modules. */
        SYS_Tasks ( );

        /* USER TASKS: BEGIN  */


        /* USER TASKS: END  */

        /* Run SA registered tasks */
        SAEngine_RegisteredTasksRun();

    }

    /* Execution should not come here during normal operation */

    return ( EXIT_FAILURE );
}
```

### Building
For information about the build and compilation environment, please refer to the [Release notes](../../../release_notes.md).

### Contributing
Contributions are welcome, and bug reports are appreciated to help improve the API. If any issues are encountered or suggestions for enhancements arise, please don't hesitate to inform us.

## License
This API is distributed under specific license terms and conditions. Please review the [License Terms and Conditions](../../Stream_Analyze_Terms_of_Use.pdf) for detailed information.

Please note that the use of this API implies acceptance of the license terms and conditions outlined in the provided document.