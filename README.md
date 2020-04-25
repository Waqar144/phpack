# Brief

This library provides c++ alternatives to the php `pack()` and `unpack()` functions. Currently only one argument is supported, hence you can't do:
```cpp
pack("vV", 123, 3444);
```

## Supported formats

|Code| Description  |
|--|--|
|s | signed short (always 16 bit, machine byte order)  |
|S | unsigned short (always 16 bit, machine byte order)  |
|n | unsigned short (always 16 bit, big endian byte order)  |
|v | unsigned short (always 16 bit, little endian byte order)  |
|i | signed integer (machine dependent size and byte order)  |
|I | unsigned integer (machine dependent size and byte order)  |
|l | signed long (always 32 bit, machine byte order) |
|L | unsigned long (always 32 bit, machine byte order) |
|N | unsigned long (always 32 bit, big endian byte order)  |
|V | unsigned long (always 32 bit, little endian byte order) |
|q | signed long long (always 64 bit, machine byte order) |
|Q | unsigned long long (always 64 bit, machine byte order) |
|J | unsigned long long (always 64 bit, big endian byte order) |
|P | unsigned long long (always 64 bit, little endian byte order) |
|f | float (machine dependent size and representation) |
|g | float (machine dependent size, little endian byte order) |
|G | float (machine dependent size, big endian byte order) |
|d | double (machine dependent size and representation) |
|e | double (machine dependent size, little endian byte order) |
|E | double (machine dependent size, big endian byte order) |

## Usage

```cpp
#include "pack.h"

using namespace PhPacker;

std::string s = pack('v', 1902);
int num = unpack<int>('v', s);
```

## Build

```sh
mkdir build && cd build
cmake ..
make
```
