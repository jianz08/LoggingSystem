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

inline bool operator == (const LogInfo &lhs, const LogInfo &rhs){
    return lhs.timestamp == rhs.timestamp
            && lhs.user == rhs.user
            && lhs.infoType == rhs.infoType
            && lhs.info == rhs.info;
}

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
    //MultipleStreamWriter();
    void log(LogInfo const &logInfo) override;

private:
    std::unordered_map<std::string, std::ofstream> umap;
    //std::unordered_map<std::string, std::unique_ptr<std::ofstream>> umap;

    //std::unordered_map<std::string, std::ofstream *> umap;
};

class LogImpl
{
public:
    explicit LogImpl(std::istream &input, Writer &output);
    ~LogImpl() = default;
    void process();

private:
    std::string inputLog();
    void saveLog(const std::string &line);
    

    std::istream &input_;
    Writer &output_;
};

bool validateLog(const std::string &line, LogInfo & logInfo);
#endif