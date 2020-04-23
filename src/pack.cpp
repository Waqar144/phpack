// (c) 2020 Mediapark
// Author: Waqar Ahmed <waqar.17a@gmail.com>
// This code is licensed under MIT license (see LICENSE for details)

#include <array>
#include <climits>
#include <cstdint>
#include <cstdio>

#include "pack.h"

namespace PhPacker
{
#if defined(__x86_64__) || defined(__LP64__) || defined(_LP64) || defined(_WIN64)
#define ENABLE_LONG64 1
#endif

#ifdef ENABLE_LONG64
#define SIZEOF_LONG 8
#else
#define SIZEOF_LONG 4
#endif

/* Whether machine is little endian */
static char machine_little_endian;

/* Mapping of byte from char (8bit) to long for machine endian */
static std::array<int, 1> byte_map;
// static int byte_map[1];

/* Mappings of bytes from int (machine dependant) to int for machine endian */
static std::array<int, sizeof(int)> int_map;
// static int int_map[sizeof(int)];

/* Mappings of bytes from shorts (16bit) for all endian environments */
static std::array<int, 2> machine_endian_short_map;
static std::array<int, 2> big_endian_short_map;
static std::array<int, 2> little_endian_short_map;

/* Mappings of bytes from longs (32bit) for all endian environments */
static std::array<int, 4> machine_endian_long_map;
static std::array<int, 4> big_endian_long_map;
static std::array<int, 4> little_endian_long_map;

#if SIZEOF_LONG > 4
/* Mappings of bytes from quads (64bit) for all endian environments */
static std::array<int, 8> machine_endian_longlong_map;
static std::array<int, 8> big_endian_longlong_map;
static std::array<int, 8> little_endian_longlong_map;
#endif

static void php_pack(u_int32_t val, size_t size, int *map, std::string &output)
{
    char *v = reinterpret_cast<char *>(&val);

    output.reserve(size);
    for (size_t i = 0; i < size; ++i) {
        output.push_back(v[map[i]]);
    }
}

inline double ToDouble(int value)
{
    return static_cast<double>(value);
}

static inline uint32_t php_pack_reverse_int32(uint32_t arg)
{
    uint32_t result;
    result =
        ((arg & 0xFF) << 24) | ((arg & 0xFF00) << 8) | ((arg >> 8) & 0xFF00) | ((arg >> 24) & 0xFF);

    return result;
}

static inline uint64_t php_pack_reverse_int64(uint64_t arg)
{
    union Swap64 {
        uint64_t i;
        uint32_t ul[2];
    } tmp, result;
    tmp.i = arg;
    result.ul[0] = php_pack_reverse_int32(tmp.ul[1]);
    result.ul[1] = php_pack_reverse_int32(tmp.ul[0]);

    return result.i;
}

/**
 * @brief pack
 * @param code
 * @param val
 * @return string
 */
std::string pack(char code, long val)
{
    init();
    std::string output;

    switch (code) {
    case 's':
    case 'S':
    case 'n':
    case 'v': {
        auto map = machine_endian_short_map.data();

        if (code == 'n') {
            map = big_endian_short_map.data();
        } else if (code == 'v') {
            map = little_endian_short_map.data();
        }

        php_pack(val, 2, map, output);
        break;
    }
    case 'i':
    case 'I':
        php_pack(val, sizeof(int), int_map.data(), output);
        break;
    case 'l':
    case 'L':
    case 'N':
    case 'V': {
        auto map = machine_endian_long_map;

        if (code == 'N') {
            map = big_endian_long_map;
        } else if (code == 'V') {
            map = little_endian_long_map;
        }

        php_pack(val, 4, map.data(), output);
        break;
    }
    }
    return output;
}

static long php_unpack(const char *data, int size, bool issigned, int *map)
{
    long result;
    char *cresult = reinterpret_cast<char *>(&result);

    result = issigned ? -1 : 0;

    for (int i = 0; i < size; ++i) {
        cresult[map[i]] = *data++;
    }

    return result;
}

/**
 * @brief unpack
 * @param format
 * @param data
 * @return long
 */
long unpack(char format, const std::string &data)
{
    init();
    const char *input = data.c_str();
    int inputpos = 0;
    int size = 0;

    const int inputlen = data.length();

    switch (format) {
    /* Never use any input */
    case 's':
    case 'S':
    case 'n':
    case 'v':
        size = 2;
        break;
    case 'i':
    case 'I':
        size = sizeof(int);
        break;
    /* Use 4 bytes of input */
    case 'l':
    case 'L':
    case 'N':
    case 'V':
        size = 4;
        break;
    default:
        break;
    }

    /* Do actual unpacking */
    if ((inputpos + size) <= inputlen) {
        switch (format) {
        case 's':
        case 'S':
        case 'n':
        case 'v': {
            long v = 0;
            bool isSigned = false;
            int* map = machine_endian_short_map.data();

            if (format == 's') {
                isSigned = input[inputpos + (machine_little_endian ? 1 : 0)] & 0x80;
            } else if (format == 'n') {
                map = big_endian_short_map.data();
            } else if (format == 'v') {
                map = little_endian_short_map.data();
            }

            v = php_unpack(&input[inputpos], 2, isSigned, map);
            return v;
        }
        case 'i':
        case 'I': {
            long v{};
            bool isSigned = false;
            if (format == 'i') {
                isSigned = input[inputpos + (machine_little_endian ? (sizeof(int) - 1) : 0)] & 0x80;
            }
            v = php_unpack(&input[inputpos], sizeof(int), isSigned, int_map.data());
            return static_cast<int>(v);
            break;
        }
        case 'l':
        case 'L':
        case 'N':
        case 'V': {
            bool issigned = false;
            int* map = machine_endian_long_map.data();
            long v = 0;

            if (format == 'l' || format == 'L') {
                issigned = input[inputpos + (machine_little_endian ? 3 : 0)] & 0x80;
            } else if (format == 'N') {
                issigned = input[inputpos] & 0x80;
                map = big_endian_long_map.data();
            } else if (format == 'V') {
                issigned = input[inputpos + 3] & 0x80;
                map = little_endian_long_map.data();
            }

            if (SIZEOF_LONG > 4 && issigned) {
                v = ~INT_MAX;
            }

            v |= php_unpack(&input[inputpos], 4, issigned, map);
            if (SIZEOF_LONG > 4) {
                if (format == 'l') {
                    v = (signed int)v;
                } else {
                    v = (unsigned int)v;
                }
            }
            return v;
        }
        }

        inputpos += size;
        if (inputpos < 0) {
            if (size != -1) { /* only print warning if not working with * */
                printf("Type %c: outside of string", format);
            }
            inputpos = 0;
        }
    }
}

void init()
{
    static bool initialized = false;
    if (initialized)
        return;
    int machine_endian_check = 1;

    machine_little_endian = ((char *)&machine_endian_check)[0];

    if (machine_little_endian) {
        /* Where to get lo to hi bytes from */
        byte_map[0] = 0;

        for (int i = 0; i < (int)sizeof(int); ++i) {
            int_map[i] = i;
        }

        machine_endian_short_map[0] = 0;
        machine_endian_short_map[1] = 1;
        big_endian_short_map[0] = 1;
        big_endian_short_map[1] = 0;
        little_endian_short_map[0] = 0;
        little_endian_short_map[1] = 1;

        machine_endian_long_map[0] = 0;
        machine_endian_long_map[1] = 1;
        machine_endian_long_map[2] = 2;
        machine_endian_long_map[3] = 3;
        big_endian_long_map[0] = 3;
        big_endian_long_map[1] = 2;
        big_endian_long_map[2] = 1;
        big_endian_long_map[3] = 0;
        little_endian_long_map[0] = 0;
        little_endian_long_map[1] = 1;
        little_endian_long_map[2] = 2;
        little_endian_long_map[3] = 3;
#if SIZEOF_LONG > 4
        machine_endian_longlong_map[0] = 0;
        machine_endian_longlong_map[1] = 1;
        machine_endian_longlong_map[2] = 2;
        machine_endian_longlong_map[3] = 3;
        machine_endian_longlong_map[4] = 4;
        machine_endian_longlong_map[5] = 5;
        machine_endian_longlong_map[6] = 6;
        machine_endian_longlong_map[7] = 7;
        big_endian_longlong_map[0] = 7;
        big_endian_longlong_map[1] = 6;
        big_endian_longlong_map[2] = 5;
        big_endian_longlong_map[3] = 4;
        big_endian_longlong_map[4] = 3;
        big_endian_longlong_map[5] = 2;
        big_endian_longlong_map[6] = 1;
        big_endian_longlong_map[7] = 0;
        little_endian_longlong_map[0] = 0;
        little_endian_longlong_map[1] = 1;
        little_endian_longlong_map[2] = 2;
        little_endian_longlong_map[3] = 3;
        little_endian_longlong_map[4] = 4;
        little_endian_longlong_map[5] = 5;
        little_endian_longlong_map[6] = 6;
        little_endian_longlong_map[7] = 7;
#endif

    } else {
        int val;
        int size = sizeof((val));

        /* Where to get hi to lo bytes from */
        byte_map[0] = size - 1;

        for (int i = 0; i < (int)sizeof(int); ++i) {
            int_map[i] = size - (sizeof(int) - i);
        }

        machine_endian_short_map[0] = size - 2;
        machine_endian_short_map[1] = size - 1;
        big_endian_short_map[0] = size - 2;
        big_endian_short_map[1] = size - 1;
        little_endian_short_map[0] = size - 1;
        little_endian_short_map[1] = size - 2;

        machine_endian_long_map[0] = size - 4;
        machine_endian_long_map[1] = size - 3;
        machine_endian_long_map[2] = size - 2;
        machine_endian_long_map[3] = size - 1;
        big_endian_long_map[0] = size - 4;
        big_endian_long_map[1] = size - 3;
        big_endian_long_map[2] = size - 2;
        big_endian_long_map[3] = size - 1;
        little_endian_long_map[0] = size - 1;
        little_endian_long_map[1] = size - 2;
        little_endian_long_map[2] = size - 3;
        little_endian_long_map[3] = size - 4;
#if SIZEOF_LONG > 4
        machine_endian_longlong_map[0] = size - 8;
        machine_endian_longlong_map[1] = size - 7;
        machine_endian_longlong_map[2] = size - 6;
        machine_endian_longlong_map[3] = size - 5;
        machine_endian_longlong_map[4] = size - 4;
        machine_endian_longlong_map[5] = size - 3;
        machine_endian_longlong_map[6] = size - 2;
        machine_endian_longlong_map[7] = size - 1;
        big_endian_longlong_map[0] = size - 8;
        big_endian_longlong_map[1] = size - 7;
        big_endian_longlong_map[2] = size - 6;
        big_endian_longlong_map[3] = size - 5;
        big_endian_longlong_map[4] = size - 4;
        big_endian_longlong_map[5] = size - 3;
        big_endian_longlong_map[6] = size - 2;
        big_endian_longlong_map[7] = size - 1;
        little_endian_longlong_map[0] = size - 1;
        little_endian_longlong_map[1] = size - 2;
        little_endian_longlong_map[2] = size - 3;
        little_endian_longlong_map[3] = size - 4;
        little_endian_longlong_map[4] = size - 5;
        little_endian_longlong_map[5] = size - 6;
        little_endian_longlong_map[6] = size - 7;
        little_endian_longlong_map[7] = size - 8;
#endif
    }
    initialized = true;
}
} // namespace PhPacker
