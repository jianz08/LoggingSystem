#include <iostream>
#include <string>
#include <fstream>
#include <istream>
#include "server.hpp"

MyOstreamImpl::MyOstreamImpl(std::ostream &output) : output_(output)
{
}

void MyOstreamImpl::log(std::string const &line)
{
    output_ << line << std::endl;
}

LogImpl::LogImpl(std::istream &input, MyOstream &output) : input_(input), output_(output)
{
}

void LogImpl::process()
{
    while (true)
    {
        std::string line = inputLog();
        if (line.empty())
            break;
        saveLog(line);
    }
}

std::string LogImpl::inputLog()
{
    std::string line;
    std::getline(input_, line);
    return line;
}

void LogImpl::saveLog(const std::string &line)
{
    output_.log(line);
}
