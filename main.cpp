#include "src/pack.h"
#include <cstring>
#include <iostream>

int main()
{
    //    long x = 2912345623;
    //    char* s = (char*)&x;
    //    for (int i = 0; i < sizeof(long); ++i) {
    //        std::cout << (int)s[i];
    //    }
    //    return 0;
    std::cout << "Trying PHP pack() in c++\n";

    auto result = PhPacker::pack('P', 65535123424);

    std::cout << "RESULT: " << result << std::endl;

    unsigned long up = PhPacker::unpack('P', result);

    std::cout << "UNPACK RESULT: " << up << std::endl;

    return 0;
}
