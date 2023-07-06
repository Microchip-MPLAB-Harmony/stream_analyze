## SAEngine_SerialBufferRead

### Table of Contents
- [SAEngine\_SerialBufferRead](#saengine_serialbufferread)
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
void SAEngine_SerialBufferRead(char*, int);
```
### Description
This function reads serial buffer

### Preconditions
- The SA Engine is properly initialized, and the [SAEngine_Initialize()](SAEngine_Initialize.md) has been successfully called.
- The UART peripheral has been configured

### Parameters
None

### Inputs
- **``Buffer``**: A pointer to the buffer where the read data will be stored
- **``Size``**:Buffer size

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