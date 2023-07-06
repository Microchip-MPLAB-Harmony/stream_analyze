## SAEngine_UartReadEventHandler

### Table of Contents
- [SAEngine\_UartReadEventHandler](#saengine_uartreadeventhandler)
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
void SAEngine_UartReadEventHandler(SERCOM_USART_EVENT event, uintptr_t context )
```
### Description
Writes the data from UART ring buffer to SA Engine buffer

### Preconditions
- The SA Engine is properly initialized, and the [SAEngine_Initialize()](SAEngine_Initialize.md) has been successfully called.
- The UART peripheral has been configured

### Parameters
Function pointer to be called after UART read event

### Inputs
None

### Output
None

### return
None

### Building
For information about the build and compilation environment, please refer to the [Release notes](../../../release_notes.md).

### Contributing
Contributions are welcome, and bug reports are appreciated to help improve the API. If any issues are encountered or suggestions for enhancements arise, please don't hesitate to inform us.

## License
This API is distributed under specific license terms and conditions. Please review the [License Terms and Conditions](../../../Stream_Analyze_Terms_of_Use.pdf) for detailed information.

Please note that the use of this API implies acceptance of the license terms and conditions outlined in the provided document.