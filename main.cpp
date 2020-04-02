#include "src/pack.h"
#include <cstring>
#include <iostream>

int main()
{
    std::cout << "Trying PHP pack() in c++\n";

    auto result = PhPacker::pack('V', 29123456);

    std::cout << "RESULT: " << result << result << std::endl;

    int up = PhPacker::unpack('V', result);

    std::cout << "UNPACK RESULT: " << up << std::endl;

    return 0;
}
