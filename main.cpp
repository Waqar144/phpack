#include <iostream>
#include "p.h"
#include <cstring>

int main()
{
    Initialize();

    std::cout << "Trying PHP pack() in c++\n";

    auto result = pack('v', 65534);

    std::cout << "RESULT: " << result << std::strlen(result) << std::endl;

    int up = unpack('v', result);

    std::cout << "UNPACK RESULT: " << up << std::endl;

    return 0;
}
