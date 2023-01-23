#include <string>
#include <fstream>
#include <istream>

class MyOstream
{
public:
    virtual void log(std::string const &) = 0;
};

class MyOstreamImpl : public MyOstream
{
public:
    MyOstreamImpl(std::ostream &output);
    void log(std::string const &line) override;

private:
    std::ostream &output_;
};

class LogImpl
{
public:
    explicit LogImpl(std::istream &input, MyOstream &output);
    ~LogImpl() = default;
    void process();

private:
    std::string inputLog();
    void saveLog(const std::string &line);

    std::istream &input_;
    MyOstream &output_;
};