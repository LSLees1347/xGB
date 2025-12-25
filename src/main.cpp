#include <iostream>
#include "xSystem.h"


int main(int argc, char* argv[])
{
    if (argc < 2)
    {
        std::cerr << "no rom found";
        return 1;
    }

    xgb* sys = new xgb();

    if (!sys->load(argv[1]))
    {
        std::cerr << "error loading rom";
        delete sys;
        return 1;
    }

    std::cout << "rom loaded\n";

    while (true)
    {
        sys->tick();
    }

    return 0;
}