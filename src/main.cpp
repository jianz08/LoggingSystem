#include <iostream>
#include "server.hpp"

int main()
{
    //SingleStreamWriter singleStreamWriter(std::cout);
    //LogImpl logImpl(std::cin, singleStreamWriter);
    
    //std::ifstream input_file("testing_logs.txt", std::ios::in);
    //StreamReader streamReader(input_file);
    SocketReader socketReader(12345);

    MultipleStreamWriter multipleStreamWriter;
    //LogImpl logImpl(std::cin, multipleStreamWriter);
    LogImpl logImpl(socketReader, multipleStreamWriter);

    logImpl.process();

    return 0;
}