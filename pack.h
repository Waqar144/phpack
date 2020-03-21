// (c) 2020 Mediapark
// Author: Waqar Ahmed <waqar.17a@gmail.com>
// This code is licensed under MIT license (see LICENSE for details)
#ifndef PACK_H
#define PACK_H

#include <string>

void Initialize();
std::string pack(char format, int dec);
int unpack(char format, std::string data);


#endif /* PACK_H */
