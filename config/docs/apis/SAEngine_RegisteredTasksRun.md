## SAEngine_RegisteredTasksRun

### Table of Contents
- [SAEngine\_RegisteredTasksRun](#saengine_registeredtasksrun)
  - [Table of Contents](#table-of-contents)
  - [Syntax](#syntax)
  - [Description](#description)
  - [Preconditions](#preconditions)
  - [Parameters](#parameters)
  - [Inputs](#inputs)
  - [Output](#output)
  - [return](#return)
  - [Building](#building)
  - [Contributing](#contributing)
- [License](#license)

### Syntax
```c
sa_task_state SAEngine_RegisteredTasksRun( void )
```
### Description
Runs SA Engine registered tasks required to execute AI/ML model and process external queries.

### Preconditions
- The SA Engine is properly initialized, and the [SAEngine_Initialize()](SAEngine_Initialize.md) has been successfully called.
- SysTick Timer for communication and longjmp

### Parameters
None

### Inputs
None

### Output
None

### return
Status of SA Engine
| Status            | Description   |
|-------------------|---------------|
| SA_STATUS_BROKEN  |               |
| SA_STATUS_RUNNING |               |
| SA_STATUS_YIELDED |               |
| SA_STATUS_WAITING |               |
| SA_STATUS_DONE    |               |

### Building
For information about the build and compilation environment, please refer to the [Release notes](../../../release_notes.md).

### Contributing
Contributions are welcome, and bug reports are appreciated to help improve the API. If any issues are encountered or suggestions for enhancements arise, please don't hesitate to inform us.

## License
This API is distributed under specific license terms and conditions. Please review the [License Terms and Conditions](../../../Stream_Analyze_Terms_of_Use.pdf) for detailed information.

Please note that the use of this API implies acceptance of the license terms and conditions outlined in the provided document.