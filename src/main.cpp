#include <iostream>
#include "server.hpp"

int main()
{
    //SingleStreamWriter singleStreamWriter(std::cout);
    //LogImpl logImpl(std::cin, singleStreamWriter);
    std::ifstream input_file("testing_logs.txt", std::ios::in);

    MultipleStreamWriter multipleStreamWriter;
    //LogImpl logImpl(std::cin, multipleStreamWriter);
    LogImpl logImpl(input_file, multipleStreamWriter);

    logImpl.process();

    return 0;
}