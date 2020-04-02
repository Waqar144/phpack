// (c) 2020 Mediapark
// Author: Waqar Ahmed <waqar.17a@gmail.com>
// This code is licensed under MIT license (see LICENSE for details)

#ifndef PACK_H
#define PACK_H

#include <string>

namespace PhPacker
{
void init();
std::string pack(char code, long val);
long unpack(char format, const std::string &data);

} // namespace PhPacker

#endif /* PACK_H */
