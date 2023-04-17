#include <iostream>
#include <string>
#include <fstream>
#include <istream>
#include <regex>
#include <cstring>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
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

StreamReader::StreamReader(std::istream &input) : input_(input)
{

}
std::string StreamReader::getline()
{
    std::string line;
    std::getline(input_, line);
    return line;
}
SocketReader::SocketReader(uint16_t portNum) : portNum_(portNum)
{
    int ret = socketInit();
    std::cout<< "ret is " << ret << "\n";
    socketAccept();
}
int SocketReader::socketInit()
{
    // Create a socket for listening on incoming connections
    listenSocket_ = socket(AF_INET, SOCK_STREAM, 0);
    if (listenSocket_ < 0)
    {
        std::cerr << "Failed to create socket\n";
        return 1;
    }

    // Set socket options
    int opt = 1;
    if (setsockopt(listenSocket_, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt)) < 0)
    {
        std::cerr << "Failed to set socket options\n";
        return 1;
    }

    // Bind the socket to a local address and port
    sockaddr_in serverAddress;
    serverAddress.sin_family = AF_INET;
    serverAddress.sin_addr.s_addr = INADDR_ANY;
    serverAddress.sin_port = htons(portNum_);
    if (bind(listenSocket_, (sockaddr *)&serverAddress, sizeof(serverAddress)) < 0)
    {
        std::cerr << "Failed to bind socket\n";
        return 1;
    }

    // Start listening for incoming connections
    if (listen(listenSocket_, 5) < 0)
    {
        std::cerr << "Failed to listen on socket\n";
        return 1;
    }

    std::cout << "Listening on port 12345\n";

    
    return 0;
}
void SocketReader::socketAccept()
{
    while (true)
    {
        // Accept incoming connection
        sockaddr_in clientAddress;
        socklen_t clientAddressLength = sizeof(clientAddress);
        clientSocket_ = accept(listenSocket_, (sockaddr *)&clientAddress, &clientAddressLength);
        if (clientSocket_ < 0)
        {
            std::cerr << "Failed to accept incoming connection\n";
            continue;
        } else {
            // Print client address and port
            char clientAddressStr[INET_ADDRSTRLEN];
            inet_ntop(AF_INET, &clientAddress.sin_addr, clientAddressStr, INET_ADDRSTRLEN);
            std::cout << "Accepted connection from " << clientAddressStr << ":" << ntohs(clientAddress.sin_port) << "\n";
            break;
        }
    }
}
std::string SocketReader::getline()
{
    while (true) {
        // Receive and print incoming messages
        char buffer[1024];
        ssize_t bytesRead;
        bytesRead = recv(clientSocket_, buffer, sizeof(buffer), 0);
        std::cout << "bytesRead " << bytesRead << "\n";
        if (bytesRead > 0)
        {
            buffer[bytesRead] = '\0';
            std::cout << "Received message. \n";
            return std::string(buffer);
        }

        // Close client socket
        close(clientSocket_);
        std::cout << "Closed connection. \n";
        socketAccept();
    }
}

LogImpl::LogImpl(Reader &reader, Writer &writer) : reader_(reader), writer_(writer)
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
    //std::string line;
    //std::getline(input_, line);
    return reader_.getline();
}

void LogImpl::saveLog(const std::string &line)
{
    LogInfo logInfo;
    if (validateLog(line, logInfo))
    {
        writer_.log(logInfo);
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