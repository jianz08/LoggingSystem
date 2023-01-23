#include <iostream>
#include "server.hpp"

int main()
{
    MyOstreamImpl myOstreamImpl(std::cout);
    LogImpl logImpl(std::cin, myOstreamImpl);

    logImpl.process();

    return 0;
}