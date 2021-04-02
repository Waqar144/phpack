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
#ifndef PACK_H
#define PACK_H

#include <any>
#include <array>
#include <cstdint>
#include <cstring>
#include <limits>
#include <string>

namespace PhPacker {

namespace __phpack__detail {
void php_pack_impl(const char *v, size_t size, int *map,
                   std::string &output) noexcept;

template <typename T>
void php_pack(const T val, size_t size, int *map,
              std::string &output) noexcept {
    const char *v = reinterpret_cast<const char *>(&val);
    php_pack_impl(v, size, map, output);
}

float php_pack_parse_float(int is_little_endian, const char *src) noexcept;
double php_pack_parse_double(int is_little_endian, const char *src) noexcept;
void php_pack_copy_float(int is_little_endian, float f,
                         std::string &output) noexcept;
void php_pack_copy_double(int is_little_endian, double d,
                          std::string &output) noexcept;

void php_pack_machine_dependent_copy_float(float val,
                                           std::string &output) noexcept;
void php_pack_machine_dependent_copy_double(double val,
                                            std::string &output) noexcept;

#if defined(__x86_64__) || defined(__LP64__) || defined(_LP64) ||              \
    defined(_WIN64)
#define ENABLE_LONG64 1
#endif

#ifdef ENABLE_LONG64
#define SIZEOF_LONG 8
#else
#define SIZEOF_LONG 4
#endif

constexpr bool is_little_endian() {
#ifdef _WIN32
    return true;
#endif

#if BYTE_ORDER == LITTLE_ENDIAN
    return true;
#else
    return false;
#endif
}

constexpr std::array<int, 1> byteMap() {
    if constexpr (is_little_endian())
            return {0};
    else
    return {sizeof(int) - 1};
}

constexpr std::array<int, 2> shortMapLE() {
    constexpr int size = sizeof(int);
    if constexpr (is_little_endian()) {
        return {0, 1};
    } else {
        return {size - 1, size - 2};
    }
}

constexpr std::array<int, 2> shortMapME() {
    constexpr int size = sizeof(int);
    if constexpr (is_little_endian()) {
        return {0, 1};
    } else {
        return {size - 2, size - 1};
    }
}

constexpr std::array<int, 2> shortMapBE() {
    constexpr int size = sizeof(int);
    if constexpr (is_little_endian()) {
        return {1, 0};
    } else {
        return {size - 2, size - 1};
    }
}

inline std::array<int, sizeof(int)> intMap() {
    std::array<int, sizeof(int)> int_map;
    constexpr int size = sizeof(int);
    for (int i = 0; i < static_cast<int>(sizeof(int)); ++i) {
        int_map[static_cast<size_t>(i)] = size - (size - i);
    }
    return int_map;
}

constexpr std::array<int, 4> longMapLE() {
    constexpr int size = sizeof(int);
    if constexpr (is_little_endian()) {
        return {0, 1, 2, 3};
    } else {
        return {size - 1, size - 2, size - 3, size - 4};
    }
}

constexpr std::array<int, 4> longMapME() {
    constexpr int size = sizeof(int);
    if constexpr (is_little_endian()) {
        return {0, 1, 2, 3};
    } else {
        return {size - 4, size - 3, size - 2, size - 1};
    }
}

constexpr std::array<int, 4> longMapBE() {
    constexpr int size = sizeof(int);
    if constexpr (is_little_endian()) {
        return {3, 2, 1, 0};
    } else {
        return {size - 4, size - 3, size - 2, size - 1};
    }
}

constexpr std::array<int, 8> longlongMapLE() {
    constexpr int size = sizeof(int);
    if constexpr (is_little_endian()) {
        return {0, 1, 2, 3, 4, 5, 6, 7};
    } else {
        return {size - 1, size - 2, size - 3, size - 4,
                    size - 5, size - 6, size - 7, size - 8};
    }
}

constexpr std::array<int, 8> longlongMapME() {
    constexpr int size = sizeof(int);
    if constexpr (is_little_endian()) {
        return {0, 1, 2, 3, 4, 5, 6, 7};
    } else {
        return {size - 8, size - 7, size - 6, size - 5,
                    size - 4, size - 3, size - 2, size - 1};
    }
}

constexpr std::array<int, 8> longlongMapBE() {
    constexpr int size = sizeof(int);
    if constexpr (is_little_endian()) {
        return {7, 6, 5, 4, 3, 2, 1, 0};
    } else {
        return {size - 8, size - 7, size - 6, size - 5,
                    size - 4, size - 3, size - 2, size - 1};
    }
}

} // namespace __phpack__detail

/**
 * @brief pack
 * @param code
 * @param val
 * @return string
 */
template <typename T, typename std::enable_if<std::is_fundamental<T>::value,
                                              int>::type = 0>
std::string pack(char code, const T val) noexcept {
    using namespace __phpack__detail;

    //   init();
    std::string output;

    switch (code) {
    case 'c':
    case 'C': {
        auto map = byteMap();
        php_pack(val, 1, map.data(), output);
        break;
    }
    case 's':
    case 'S':
    case 'n':
    case 'v': {
        auto map = shortMapME();
        T v = val;

        if (code == 'n') {
            map = shortMapBE();
        } else if (code == 'v') {
            map = shortMapLE();
        }

        php_pack(v, 2, map.data(), output);
        break;
    }
    case 'i':
    case 'I': {
        auto map = intMap();
        php_pack(val, sizeof(int), map.data(), output);
        break;
    }
    case 'l':
    case 'L':
    case 'N':
    case 'V': {
        auto map = longMapME();
        T v = val;

        if (code == 'N') {
            map = longMapBE();
        } else if (code == 'V') {
            map = longMapLE();
        }

        php_pack(v, 4, map.data(), output);
        break;
    }
#if SIZEOF_LONG > 4
    case 'q': {
        int64_t v = static_cast<int64_t>(val);
        auto map = longlongMapME();
        php_pack(v, 8, map.data(), output);
        break;
    }
    case 'Q':
    case 'J':
    case 'P': {
        uint64_t v = static_cast<uint64_t>(val);
        auto map = longlongMapME();
        if (code == 'J') {
            v = static_cast<uint64_t>(val);
            map = longlongMapBE();
        } else if (code == 'P') {
            v = static_cast<uint64_t>(val);
            map = longlongMapLE();
        }

        php_pack(v, 8, map.data(), output);
        break;
    }
#endif
    case 'f': {
        /* pack machine endian float */
        php_pack_machine_dependent_copy_float(static_cast<float>(val), output);
        break;
    }
    case 'g': {
        /* pack little endian float */
        php_pack_copy_float(1, static_cast<float>(val), output);
        break;
    }
    case 'G': {
        /* pack big endian float */
        php_pack_copy_float(0, static_cast<float>(val), output);
        break;
    }
    case 'd': {
        php_pack_machine_dependent_copy_double(static_cast<double>(val), output);
    } break;

    case 'e': {
        /* pack little endian double */
        php_pack_copy_double(1, static_cast<double>(val), output);
        break;
    }

    case 'E': {
        /* pack big endian double */
        php_pack_copy_double(0, static_cast<double>(val), output);
        break;
    }
    }
    return output;
}

/**
 * @brief unpack
 * @param format
 * @param data
 * @return long
 */
std::any unpack(char format, const std::string &data);

} // namespace PhPacker

#endif /* PACK_H */
