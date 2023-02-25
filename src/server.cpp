#include <iostream>
#include <string>
#include <fstream>
#include <istream>
#include <regex>
#include "server.hpp"

SingleStreamWriter::SingleStreamWriter(std::ostream &output) : output_(output)
{
}

void logInfoToStr(LogInfo const &logInfo, std::string & outputStr) {
    outputStr = std::to_string(logInfo.timestamp) + "\t" +
                logInfo.user + "\t" +
                logInfo.infoType + "\t" +
                logInfo.info;
}

void SingleStreamWriter::log(LogInfo const &logInfo)
{
    std::string outputStr;
    logInfoToStr(logInfo, outputStr);
    output_ << outputStr << std::endl;
}

void MultipleStreamWriter::log(LogInfo const &logInfo)
{
    std::string outputStr;
    logInfoToStr(logInfo, outputStr);
    if (auto iter = umap.find(logInfo.user); iter != umap.end()) {
        iter->second << outputStr << std::endl;
        //*(iter->second) << outputStr << std::endl;
    } else {
        std::ofstream fout;
        fout.open(logInfo.user + ".txt", std::ios::app);
        fout << outputStr << std::endl;
        umap.insert({logInfo.user, std::move(fout)});

        /* std::unique_ptr<std::ofstream> fout = std::make_unique<std::ofstream>(logInfo.user + ".txt", std::ios::app);
        *fout << outputStr << std::endl;
        umap.insert({logInfo.user, std::move(fout)}); */

        //std::ofstream *fout = new std::ofstream;
        //fout->open(logInfo.user + ".txt", std::ios::app);
        //umap.insert({logInfo.user, fout});
    }
}

LogImpl::LogImpl(std::istream &input, Writer &output) : input_(input), output_(output)
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
    if (validateLog(line, logInfo))
    {
        output_.log(logInfo);
    }
    else
    {
        std::cerr << "Log format is invalid: " << line << std::endl;
    }
}

bool validateLog(const std::string &line, LogInfo &logInfo)
{
    // The input of the log needs to follow the format below:
    // {timestamp}###{user}###{type}:{info}
    std::regex pattern("^(\\d+)###(\\w+)###(\\w+):([^#]+)$");
    std::smatch match;

    if (!(std::regex_match(line, match, pattern) && match.size() == 5))
    {
        return false;
    }

    logInfo.timestamp = static_cast<int64_t>(std::stoll(match[1].str()));
    logInfo.user = match[2].str();
    logInfo.infoType = match[3].str();
    logInfo.info = match[4].str();
    return true;
}