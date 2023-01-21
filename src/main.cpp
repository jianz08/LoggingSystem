#include <iostream>
#include "server.hpp"

int main()
{
    LogImpl logImpl(std::cin, "logs.txt");

    logImpl.process();

    return 0;
}