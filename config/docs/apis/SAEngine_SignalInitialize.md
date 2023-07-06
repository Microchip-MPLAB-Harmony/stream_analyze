## SAEngine_SignalInitialize

### Table of Contents
- [SAEngine\_SignalInitialize](#saengine_signalinitialize)
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
bool SAEngine_SignalInitialize( int16_t identifier, volatile float32_t * const source )
```
### Description
Initializes the stream signal source

### Preconditions
- The SA Engine is properly initialized, and the [SAEngine_Initialize()](SAEngine_Initialize.md) has been successfully called.

### Parameters

| Parameters            | Type   | Description|
|-------------------|---------------|---------|
| identifier  |    int16_t     | Unique ID assigned to the streaming signal |
| source |  volatile float32_t * const | address of the streaming signal source |



### Inputs
None

### Output
None

### return
status of signal initialization

| return value            | Type   | Description|
|-------------------|---------------|---------|
| True   |    bool     | The stream signal has been successfully initialized |
| False  |  bool | The stream signal has not been initialized |

### Building
For information about the build and compilation environment, please refer to the [Release notes](../../../release_notes.md).

### Contributing
Contributions are welcome, and bug reports are appreciated to help improve the API. If any issues are encountered or suggestions for enhancements arise, please don't hesitate to inform us.

## License
This API is distributed under specific license terms and conditions. Please review the [License Terms and Conditions](../../../Stream_Analyze_Terms_of_Use.pdf) for detailed information.

Please note that the use of this API implies acceptance of the license terms and conditions outlined in the provided document.