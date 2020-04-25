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

    auto result = PhPacker::pack('q', 65);

    std::cout << "RESULT: " << result << std::endl;

    auto up = PhPacker::unpack<long long>('q', result);

    std::cout << "UNPACK RESULT: " << up << std::endl;

    return 0;
}
