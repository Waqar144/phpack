# Brief

This library provides c++ alternatives to the php `pack()` and `unpack()` functions.

## Supported formats

|Code| Description  |
|--|--|
|s | signed short (always 16 bit, machine byte order)  |
|S | unsigned short (always 16 bit, machine byte order)  |
|n | unsigned short (always 16 bit, big endian byte order)  |
|v | unsigned short (always 16 bit, little endian byte order)  |


Note: Currently only `'v', 's', 'S', 's'` are supported. More options are being added

## Usage

```cpp
#include "pack.h"

using namespace PhPacker;

std::string s = pack('v', 1902);
int num = unpack('v', s);
```

## Build

```sh
mkdir build && cd build
cmake ..
make
```


