## SAEngine_Initialize

### Table of Contents
- [SAEngine\_Initialize](#saengine_initialize)
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
void SAEngine_Initialize(void)
```
### Description

This function initializes the kernel tasks and MCU peripherals required for executing the AI/ML models based on MCC Configuration. It configures the following components:
- Nanocore version of SA Engine
- Nanocore tasks stack and slack 
- Nanocore communication and relay process
- Assign interrupt source for model time-out
- Start timer for time keeping
- Callback function on UART data-reception

### Preconditions
- UART, timer, and interrupts have been properly configured.

### Parameters
None

### Inputs
None

### Output
None

### return

### Building
For information about the build and compilation environment, please refer to the [Release notes](../../../release_notes.md).

### Contributing
Contributions are welcome, and bug reports are appreciated to help improve the API. If any issues are encountered or suggestions for enhancements arise, please don't hesitate to inform us.

## License
This API is distributed under specific license terms and conditions. Please review the [License Terms and Conditions](../../../Stream_Analyze_Terms_of_Use.pdf) for detailed information.

Please note that the use of this API implies acceptance of the license terms and conditions outlined in the provided document.