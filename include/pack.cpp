/**
 * Copyright (c) 2020 Waqar Ahmed
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 * 3. Neither the name of the copyright holder nor the names of contributors
 *    may be used to endorse or promote products derived from this software
 *    without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE AUTHOR AND CONTRIBUTOR(S) ``AS IS'' AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED.  IN NO EVENT SHALL THE AUTHOR OR CONTRIBUTOR(S) BE LIABLE
 * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS
 * OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
 * HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
 * LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY
 * OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF
 * SUCH DAMAGE.
 */
#include "pack.h"

#include <string>
#include <cstring>
#include <array>

namespace PhPacker {

namespace __phpack__detail {

void php_pack_impl(const char* v, size_t size, int* map, std::string& output) noexcept
{
    output.reserve(size);
    for (size_t i = 0; i < size; ++i) {
        output.push_back(v[map[i]]);
    }
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

void php_pack_machine_dependent_copy_float(float val, std::string& output) noexcept
{
    std::array<char, sizeof(float)> out = {};
    memcpy(out.data(), &val, sizeof(val));
    output.assign(out.begin(), out.end());
}

void php_pack_machine_dependent_copy_double(double val, std::string& output) noexcept
{
    std::array<char, sizeof(double)> out = {};
    memcpy(out.data(), &val, sizeof(val));
    output.assign(out.begin(), out.end());
}

template <typename T>
T php_unpack(const char *data, int size, bool issigned, int *map) noexcept {
    int64_t result = php_unpack_impl(data, size, issigned, map);
    return static_cast<T>(result);
}

/** Unapacking **/
inline signed char unpack_signed_char(const char* data) noexcept
{
    bool isSigned = (data[0] & 0x80);
    auto map = byteMap();
    signed char v = php_unpack<signed char>(data, 1, isSigned, map.data());
    return v;
}

inline short unpack_signed_short(const char* data) noexcept
{
    bool isSigned = data[(is_little_endian() ? 1 : 0)] & 0x80;
    /* return here for signed short */
    auto map = shortMapME();
    return php_unpack<short>(data, 2, isSigned, map.data());
}

int* get_unsigned_short_map(char format) noexcept
{
    static auto map = shortMapME();
    if (format == 'n') {
        map = shortMapBE();
    } else if (format == 'v') {
        map = shortMapLE();
    }
    return map.data();
}
}

std::any unpack(char format, const std::string &data) {
    using namespace __phpack__detail;
    //   init();

    /* Do actual unpacking */
    switch (format) {
    case 'c':
        return unpack_signed_char(data.c_str());
    case 'C': {
        auto map = byteMap();
        return php_unpack<unsigned char>(data.c_str(), 1, false, map.data());
    }
    case 's':
        return unpack_signed_short(data.c_str());
    case 'S':
    case 'n':
    case 'v': {
        int *map = get_unsigned_short_map(format);
        return php_unpack<unsigned short>(data.c_str(), 2, false, map);
    }
    case 'i':
    case 'I': {
        bool isSigned = false;
        if (format == 'i') {
            isSigned = data[(is_little_endian() ? (sizeof(int) - 1) : 0)] & 0x80;
            auto map = intMap();
            int v = php_unpack<int>(data.c_str(), sizeof(int), isSigned, map.data());
            return v;
        } else {
            auto map = intMap();
            unsigned int v = php_unpack<unsigned int>(data.c_str(), sizeof(int),
                                                      isSigned, map.data());
            return v;
        }
    }
    case 'l': {
        int32_t v{};
        auto map = longMapME();
        bool isSigned = data[is_little_endian() ? 3 : 0] & 0x80;
        if (SIZEOF_LONG > 4 && isSigned) {
            v = ~std::numeric_limits<int>::max();
        }
        v |= php_unpack<int32_t>(data.c_str(), 4, isSigned, map.data());
//         if (SIZEOF_LONG > 4) {
//             return static_cast<signed long>(v);
//         }
        return v;
    }
    case 'L':
    case 'N':
    case 'V': {
        bool issigned = false;
        auto map = longMapME();
        uint32_t v{};

        if (format == 'L') {
            issigned = data[is_little_endian() ? 3 : 0] & 0x80;
        } else if (format == 'N') {
            issigned = data[0] & 0x80;
            map = longMapBE();
        } else if (format == 'V') {
            issigned = data[3] & 0x80;
            map = longMapLE();
        }

        if (SIZEOF_LONG > 4 && issigned) {
            v = ~std::numeric_limits<int>::max();
        }

        v |= php_unpack<long>(data.c_str(), 4, issigned, map.data());
//         if (SIZEOF_LONG > 4) {
//             return static_cast<unsigned long>(v);
//         }
        return v;
    }

#if SIZEOF_LONG > 4
    case 'q':
    case 'Q':
    case 'J':
    case 'P': {
        bool isSigned = 0;
        auto map = longlongMapME();

        if (format == 'q' || format == 'Q') {
            isSigned = data[is_little_endian() ? 7 : 0] & 0x80;
        } else if (format == 'J') {
            isSigned = data[0] & 0x80;
            map = longlongMapBE();
        } else if (format == 'P') {
            isSigned = data[7] & 0x80;
            map = longlongMapLE();
        }

        if (format == 'q') {
            auto v = php_unpack<int64_t>(data.data(), 8, isSigned, map.data());
            return v;
        } else {
            auto v = php_unpack<uint64_t>(data.data(), 8, isSigned, map.data());
            return v;
        }
    }
#endif
    case 'f': /* float */
    case 'g': /* little endian float*/
    case 'G': /* big endian float*/
    {
        float v{};

        if (format == 'g') {
            v = php_pack_parse_float(1, data.data());
        } else if (format == 'G') {
            v = php_pack_parse_float(0, data.data());
        } else {
            memcpy(&v, data.data(), sizeof(float));
        }
        return v;

        break;
    }

    case 'd': /* double */
    case 'e': /* little endian float */
    case 'E': /* big endian float */
    {
        double v{};
        if (format == 'e') {
            v = php_pack_parse_double(1, data.data());
        } else if (format == 'E') {
            v = php_pack_parse_double(0, data.data());
        } else {
            memcpy(&v, data.data(), sizeof(double));
        }
        return v;
    }
    }
    // need a better way to exit,
    // control should never reach here ideally
    return -1;
}

// namespace __phpack__detail
} // namespace PhPacker
