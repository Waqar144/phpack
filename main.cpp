#include <iostream>
#include "pack.h"
#include <cstring>

int main()
{
    PhPacker::init();

    std::cout << "Trying PHP pack() in c++\n";

    auto result = PhPacker::pack('n', 65535);

    std::cout << "RESULT: " << result << result << std::endl;

    int up = PhPacker::unpack('n', result);

    std::cout << "UNPACK RESULT: " << up << std::endl;

    return 0;
}
