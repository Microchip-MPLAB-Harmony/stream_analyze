## SAEngine_SensorSampleGet

### Table of Contents
- [SAEngine\_SensorSampleGet](#saengine_sensorsampleget)
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
float SAEngine_SensorSampleGet( int16_t identifier )
```
### Description
Return the stream signal value corresponding to the stream signal identifier

### Preconditions
- The SA Engine is properly initialized, and the [SAEngine_Initialize()](SAEngine_Initialize.md) has been successfully called.

### Parameters

| Parameters            | Type   | Description|
|-------------------|---------------|---------|
| identifier  |    int16_t     | Unique ID assigned to the streaming signal |

### Inputs
None

### Output
None

### return
returns stream signal value 

### Building
For information about the build and compilation environment, please refer to the [Release notes](../../../release_notes.md).

### Contributing
Contributions are welcome, and bug reports are appreciated to help improve the API. If any issues are encountered or suggestions for enhancements arise, please don't hesitate to inform us.

## License
This API is distributed under specific license terms and conditions. Please review the [License Terms and Conditions](../../../Stream_Analyze_Terms_of_Use.pdf) for detailed information.

Please note that the use of this API implies acceptance of the license terms and conditions outlined in the provided document.