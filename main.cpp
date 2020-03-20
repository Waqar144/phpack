#include <iostream>
#include "pack.h"
#include <cstring>

int main()
{
    Initialize();

    std::cout << "Trying PHP pack() in c++\n";

    auto result = pack('v', 2787);

    std::cout << "RESULT: " << result << result << std::endl;

    int up = unpack('v', result);

    std::cout << "UNPACK RESULT: " << up << std::endl;

    return 0;
}
