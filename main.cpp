#include "include/pack.h"
#include <cstring>
#include <iostream>

int main()
{
    std::cout << "Trying PHP pack() in c++\n";

    long long x = 65;
    auto result = PhPacker::pack('q', x);

    std::cout << "RESULT: " << result << std::endl;

    auto up = PhPacker::unpack<long long>('q', result);

    std::cout << "UNPACK RESULT: " << up << std::endl;

    return 0;
}
