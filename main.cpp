#include "src/pack.h"
#include <cstring>
#include <iostream>

int main()
{
    PhPacker::init();

    std::cout << "Trying PHP pack() in c++\n";

    auto result = PhPacker::pack('v', 258);

    std::cout << "RESULT: " << result << result << std::endl;

    int up = PhPacker::unpack('v', result);

    std::cout << "UNPACK RESULT: " << up << std::endl;

    return 0;
}
