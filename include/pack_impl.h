#ifndef __PACK_IMPL_H__
#define __PACK_IMPL_H__

#include <string>
#include <cstring>
#include <array>

namespace PhPacker {

namespace __phpack__detail {

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

static bool initialized = false;

void init() noexcept
{
    if (initialized)
        return;
    int machine_endian_check = 1;
    machine_little_endian = reinterpret_cast<char*>(&machine_endian_check)[0];

    if (machine_little_endian) {
        /* Where to get lo to hi bytes from */
        byte_map[0] = 0;

        for (int i = 0; i < static_cast<int>(sizeof(int)); ++i) {
            int_map[static_cast<size_t>(i)] = i;
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

        for (int i = 0; i < static_cast<int>(sizeof(int)); ++i) {
            int_map[static_cast<size_t>(i)] = size - (size - i);
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

void php_pack_impl(const char* v, size_t size, int* map, std::string& output) noexcept
{
    output.reserve(size);
    for (size_t i = 0; i < size; ++i) {
        output.push_back(v[map[i]]);
    }
}

template <typename T>
void php_pack(const T val, size_t size, int* map, std::string& output) noexcept
{
    const char* v = reinterpret_cast<const char*>(&val);
    php_pack_impl(v, size, map, output);
}

inline double ToDouble(int value)
{
    return static_cast<double>(value);
}

float php_pack_parse_float(int is_little_endian, const char* src) noexcept
{
    char out[4] = {};
    if (!is_little_endian) {
        out[0] = src[3];
        out[1] = src[2];
        out[2] = src[1];
        out[3] = src[0];
    } else {
        out[0] = src[0];
        out[1] = src[1];
        out[2] = src[2];
        out[3] = src[3];
    }
    float f {};
    memcpy(&f, out, sizeof(float));
    return f;
}

double php_pack_parse_double(int is_little_endian, const char* src) noexcept
{
    char out[8] = {};
    if (!is_little_endian) {
        out[0] = src[7];
        out[1] = src[6];
        out[2] = src[5];
        out[3] = src[4];
        out[4] = src[3];
        out[5] = src[2];
        out[6] = src[1];
        out[7] = src[0];
    } else {
        out[0] = src[0];
        out[1] = src[1];
        out[2] = src[2];
        out[3] = src[3];
        out[4] = src[4];
        out[5] = src[5];
        out[6] = src[6];
        out[7] = src[7];
    }
    double d {};
    memcpy(&d, out, sizeof(double));

    return d;
}

void php_pack_copy_float(int is_little_endian, float f, std::string& output) noexcept
{
    std::array<char, sizeof(float)> dst = {};
    char src[4] = {};
    memcpy(src, &f, sizeof(float));
    if (!is_little_endian) {
        dst[0] = src[3];
        dst[1] = src[2];
        dst[2] = src[1];
        dst[3] = src[0];
    } else {
        dst[0] = src[0];
        dst[1] = src[1];
        dst[2] = src[2];
        dst[3] = src[3];
    }
    output.assign(dst.begin(), dst.end());
}

void php_pack_copy_double(int is_little_endian, double d, std::string& output) noexcept
{
    std::array<char, sizeof(double)> dst = {};
    char src[8] = {};
    memcpy(src, &d, sizeof(double));
    if (!is_little_endian) {
        dst[0] = src[7];
        dst[1] = src[6];
        dst[2] = src[5];
        dst[3] = src[4];
        dst[4] = src[3];
        dst[5] = src[2];
        dst[6] = src[1];
        dst[7] = src[0];
    } else {
        dst[0] = src[0];
        dst[1] = src[1];
        dst[2] = src[2];
        dst[3] = src[3];
        dst[4] = src[4];
        dst[5] = src[5];
        dst[6] = src[6];
        dst[7] = src[7];
    }
    output.assign(dst.begin(), dst.end());
}

int64_t php_unpack_impl(const char* data, int size, bool issigned, int* map) noexcept
{
    int64_t result {};
    char* cresult = reinterpret_cast<char*>(&result);

    result = issigned ? -1 : 0;

    for (int i = 0; i < size; ++i) {
        cresult[map[i]] = *data++;
    }
    return result;
}


template <typename T>
T php_unpack(const char* data, int size, bool issigned, int* map) noexcept
{
    int64_t result = php_unpack_impl(data, size, issigned, map);
    return static_cast<T>(result);
}

void php_pack_machine_dependent_copy_float(float val, std::string& output)
{
    std::array<char, sizeof(float)> out = {};
    memcpy(out.data(), &val, sizeof(val));
    output.assign(out.begin(), out.end());
}

void php_pack_machine_dependent_copy_double(double val, std::string& output)
{
    std::array<char, sizeof(double)> out = {};
    memcpy(out.data(), &val, sizeof(val));
    output.assign(out.begin(), out.end());
}

/** Unapacking **/
inline signed char unpack_signed_char(const char* data)
{
    bool isSigned = (data[0] & 0x80);
    signed char v = php_unpack<signed char>(data, 1, isSigned, byte_map.data());
    return v;
}

inline short unpack_signed_short(const char* data)
{
    bool isSigned = data[(machine_little_endian ? 1 : 0)] & 0x80;
    /* return here for signed short */
    return php_unpack<short>(data, 2, isSigned, machine_endian_short_map.data());
}

inline int* get_unsigned_short_map(char format)
{
    int* map = machine_endian_short_map.data();
    if (format == 'n') {
        map = big_endian_short_map.data();
    } else if (format == 'v') {
        map = little_endian_short_map.data();
    }
    return map;
}


} // namespace __phpack__detail
} // namespace PhPacker

#endif // PACK_IMPL_H
