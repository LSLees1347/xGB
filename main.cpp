#include <iostream>
#include "central.h"

class memory mem;

int main()
{
    std::cout << "running...\n";

    init(&mem);
    run();

    return 0;
}