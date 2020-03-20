#ifndef PACK_H
#define PACK_H

#include <string>

void Initialize();
std::string pack(char format, int dec);
int unpack(char format, std::string data);


#endif /* PACK_H */
