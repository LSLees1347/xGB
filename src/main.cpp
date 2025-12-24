#include <iostream>
#include "xSystem.h"


int main(int argc, char* argv[])
{
    std::cout << "loading...\n";

    if (argc < 2)
    {
        std::cerr << "no rom found";
        return 1;
    }

    xgb* sys = new xgb();

    if (!sys->loadROM(argv[1]))
    {
        std::cerr << "error loading rom";
        delete sys;
        return 1;
    }

    std::cout << "rom loaded";

    while (true)
    {
        sys->tick();
    }

    return 0;
}