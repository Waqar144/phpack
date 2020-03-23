# Brief

This library provides c++ alternatives to the php `pack()` and `unpack()` functions.

## Supported formats

|Code| Description  |
|--|--|
|v | unsigned short (little endian byte order)  |


Note: Currently only `'v'` is supported. More options are being added

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


