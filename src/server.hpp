#ifndef SERVER_H
#define SERVER_H
#include <string>
#include <fstream>
#include <istream>
#include <unordered_map>

struct LogInfo
{
    int64_t timestamp;
    std::string user;
    std::string infoType;
    std::string info;
};

inline bool operator==(const LogInfo &lhs, const LogInfo &rhs)
{
    return lhs.timestamp == rhs.timestamp && lhs.user == rhs.user && lhs.infoType == rhs.infoType && lhs.info == rhs.info;
}
class Reader
{
public:
    virtual std::string getline() = 0;
};

class StreamReader : public Reader
{
public:
    StreamReader(std::istream &input);
    std::string getline() override;
private:
    std::istream &input_;
};

class SocketReader : public Reader
{
public:
    SocketReader(uint16_t portNum);
    std::string getline() override;
private:
    int socketInit();
    void socketAccept();
    uint16_t portNum_;
    int listenSocket_;
    int clientSocket_;
};

class Writer
{
public:
    virtual void log(LogInfo const &) = 0;
};

class SingleStreamWriter : public Writer
{
public:
    SingleStreamWriter(std::ostream &output);
    void log(LogInfo const &logInfo) override;

private:
    std::ostream &output_;
};

class MultipleStreamWriter : public Writer
{
public:
    // MultipleStreamWriter();
    void log(LogInfo const &logInfo) override;

private:
    std::unordered_map<std::string, std::ofstream> umap;
    // std::unordered_map<std::string, std::unique_ptr<std::ofstream>> umap;

    // std::unordered_map<std::string, std::ofstream *> umap;
};

class LogImpl
{
public:
    explicit LogImpl(Reader &input, Writer &output);
    ~LogImpl() = default;
    void process();

private:
    std::string inputLog();
    void saveLog(const std::string &line);

    Reader &reader_;
    Writer &writer_;
};

bool validateLog(const std::string &line, LogInfo &logInfo);
#endif