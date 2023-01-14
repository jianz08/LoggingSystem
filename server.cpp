#include <iostream>
#include <string>
#include <fstream>
#include <istream>
#include "server.hpp"

LogImpl::LogImpl(std::istream &input, std::string fileName) : _input(input)
{
    _log_file.open(fileName);
}
LogImpl::~LogImpl()
{
    _log_file.close();
}
void LogImpl::process()
{
    while (true)
    {
        std::string line = inputLog();
        if (line.empty())
            break;
        std::cout << line << std::endl;
        saveLog(line);
    }
}

std::string LogImpl::inputLog()
{
    std::string line;
    std::getline(_input, line);
    return line;
}

void LogImpl::saveLog(const std::string &line)
{
    _log_file << line << std::endl;
}

/* int main()
{
    LogImpl logImpl(std::cin, "logs.txt");

    logImpl.process();

    return 0;
} */