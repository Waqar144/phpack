#include <iostream>
#include "pack.h"
#include <cstring>

int main()
{
    init();

    std::cout << "Trying PHP pack() in c++\n";

    auto result = pack('v', 8889);

    std::cout << "RESULT: " << result << result << std::endl;

    int up = unpack('v', result);

    std::cout << "UNPACK RESULT: " << up << std::endl;

    return 0;
}
