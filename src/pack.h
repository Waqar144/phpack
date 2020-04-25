// (c) 2020 Mediapark
// Author: Waqar Ahmed <waqar.17a@gmail.com>
// This code is licensed under MIT license (see LICENSE for details)

#ifndef PACK_H
#define PACK_H

#include <string>

namespace PhPacker
{
void init();
template <typename T, typename = typename std::enable_if<std::is_pod<T>::value>::type>
std::string pack(char code, const T val);

template <typename T>
T unpack(char format, const std::string& data);

} // namespace PhPacker

#endif /* PACK_H */
