## SAEngine_NanocoreInitTask

### Table of Contents
- [SAEngine\_NanocoreInitTask](#saengine_nanocoreinittask)
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
void SAEngine_NanocoreInitTask(int stack_size, int stack_slack);
```
### Description
This function initializes the stack size and stack slack for the SA Engine Nanocore version on resource-constrained edge devices, specifically microcontrollers. 

### Preconditions


### Parameters


### Inputs
- **``stack_size``**: The desired size of the stack allocated for the SA Engine Nanocore version in bytes. It specifies the total memory reserved for storing execution context and variables.
- **``stack_slack``**: The amount of slack or additional buffer space to be added to the stack size in bytes. It ensures that the stack has some extra capacity beyond the immediate requirements of the SA Engine.

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