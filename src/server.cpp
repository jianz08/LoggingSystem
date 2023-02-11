#include <iostream>
#include <string>
#include <fstream>
#include <istream>
#include <regex>
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
    LogInfo logInfo;
    if (validateLog(line, logInfo)) {
        std::string outputStr;
        outputStr = std::to_string(logInfo.timestamp) + "\t" +
                    logInfo.user + "\t" +
                    logInfo.infoType + "\t" +
                    logInfo.info;
        output_.log(outputStr);
    } else {
        std::cerr<<"Log format is invalid: "<<line<<std::endl;
    }
}

bool validateLog(const std::string &line, LogInfo & logInfo)
{
    // The input of the log needs to follow the format below:
    // {timestamp}###{user}###{type}:{info}
    std::regex pattern("^(\\d+)###(\\w+)###(\\w+):([^#]+)$");
    std::smatch match;

    if (!(std::regex_match(line, match, pattern) && match.size() == 5)) {
        return false;
    }

    logInfo.timestamp = static_cast<int64_t>(std::stoll(match[1].str()));
    logInfo.user = match[2].str();
    logInfo.infoType = match[3].str();
    logInfo.info = match[4].str();
    return true;
}