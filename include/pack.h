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

#include <limits>
#include <cstdint>

#include "pack_impl.h"

namespace PhPacker
{

/**
 * @brief pack
 * @param code
 * @param val
 * @return string
 */
template <typename T,
    typename std::enable_if<std::is_pod<T>::value, int>::type = 0>
std::string pack(char code, const T val) noexcept
{
    using namespace __phpack__detail;

    init();
    std::string output;


    switch (code) {
    case 'c':
    case 'C':
        php_pack(val, 1, byte_map.data(), output);
        break;
    case 's':
    case 'S':
    case 'n':
    case 'v': {
        auto map = machine_endian_short_map.data();
        T v = val;

        if (code == 'n') {
            map = big_endian_short_map.data();
        } else if (code == 'v') {
            map = little_endian_short_map.data();
        }

        php_pack(v, 2, map, output);
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
        T v = val;

        if (code == 'N') {
            map = big_endian_long_map;
        } else if (code == 'V') {
            map = little_endian_long_map;
        }

        php_pack(v, 4, map.data(), output);
        break;
    }
#if SIZEOF_LONG > 4
    case 'q': {
        int64_t v = static_cast<int64_t>(val);
        auto map = machine_endian_longlong_map;
        php_pack(v, 8, map.data(), output);
        break;
    }
    case 'Q':
    case 'J':
    case 'P': {
        uint64_t v = static_cast<uint64_t>(val);
        auto map = machine_endian_longlong_map;
        if (code == 'J') {
            v = static_cast<uint64_t>(val);
            map = big_endian_longlong_map;
        } else if (code == 'P') {
            v = static_cast<uint64_t>(val);
            map = little_endian_longlong_map;
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
template <typename T>
T unpack(char format, const std::string& data) noexcept
{
    using namespace __phpack__detail;
    init();

    /* Do actual unpacking */
        switch (format) {
        case 'c':
        case 'C': {
            bool isSigned = (format == 'c') ? (data[0] & 0x80) : 0;
            if (format == 'c') {
                signed char v = php_unpack<signed char>(data.c_str(), 1, isSigned, byte_map.data());
                return static_cast<T>(v);
            } else {
                unsigned char v = php_unpack<unsigned char>(data.c_str(), 1, isSigned, byte_map.data());
                return v;
            }
            break;
        }
        case 's':
        case 'S':
        case 'n':
        case 'v': {
            bool isSigned = false;
            int* map = machine_endian_short_map.data();

            if (format == 's') {
                isSigned = data[(machine_little_endian ? 1 : 0)] & 0x80;
                /* return here for signed short */
                auto v = php_unpack<short>(data.c_str(), 2, isSigned, map);
                return v;
            } else if (format == 'n') {
                map = big_endian_short_map.data();
            } else if (format == 'v') {
                map = little_endian_short_map.data();
            }

            auto v = php_unpack<unsigned short>(data.c_str(), 2, isSigned, map);
            return v;
        }
        case 'i':
        case 'I': {
            bool isSigned = false;
            if (format == 'i') {
                isSigned = data[(machine_little_endian ? (sizeof(int) - 1) : 0)] & 0x80;
                int v = php_unpack<int>(data.c_str(), sizeof(int), isSigned, int_map.data());
                return v;
            } else {
                unsigned int v = php_unpack<unsigned int>(data.c_str(), sizeof(int), isSigned, int_map.data());
                return v;
            }
        }
        case 'l': {
            int32_t v{};
            int *map = machine_endian_long_map.data();
            bool isSigned = data[machine_little_endian ? 3 : 0] & 0x80;
            if (SIZEOF_LONG > 4 && isSigned) {
                v = ~std::numeric_limits<int>::max();
            }
            v |= php_unpack<int32_t>(data.c_str(), 4, isSigned, map);
            if (SIZEOF_LONG > 4) {
                return static_cast<signed long>(v);
            }
            return v;
        }
        case 'L':
        case 'N':
        case 'V': {
            bool issigned = false;
            int* map = machine_endian_long_map.data();
            uint32_t v {};

            if (format == 'L') {
                issigned = data[machine_little_endian ? 3 : 0] & 0x80;
            } else if (format == 'N') {
                issigned = data[0] & 0x80;
                map = big_endian_long_map.data();
            } else if (format == 'V') {
                issigned = data[3] & 0x80;
                map = little_endian_long_map.data();
            }

            if (SIZEOF_LONG > 4 && issigned) {
                v = ~std::numeric_limits<int>::max();
            }

            v |= php_unpack<long>(data.c_str(), 4, issigned, map);
            if (SIZEOF_LONG > 4) {
                return static_cast<unsigned long>(v);
            }
            return v;
        }

#if SIZEOF_LONG > 4
        case 'q':
        case 'Q':
        case 'J':
        case 'P': {
            bool isSigned = 0;
            auto map = machine_endian_longlong_map;

            if (format == 'q' || format == 'Q') {
                isSigned = data[machine_little_endian ? 7 : 0] & 0x80;
            } else if (format == 'J') {
                isSigned = data[0] & 0x80;
                map = big_endian_longlong_map;
            } else if (format == 'P') {
                isSigned = data[7] & 0x80;
                map = little_endian_longlong_map;
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
            float v {};

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
            double v {};
            if (format == 'e') {
                v = php_pack_parse_double(1, data.data());
            } else if (format == 'E') {
                v = php_pack_parse_double(0, data.data());
            } else {
                memcpy(&v, data.data(), sizeof(double));
            }
            return v;
            break;
        }
    }
    //need a better way to exit,
    //control should never reach here ideally
    return static_cast<T>(-1);
}

} // namespace PhPacker

#endif /* PACK_H */
