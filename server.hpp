#include <string>
#include <fstream>
#include <istream>

class LogImpl
{
public:
    explicit LogImpl(std::istream &input, std::string fileName);
    ~LogImpl();
    void process();

private:
    std::string inputLog();
    void saveLog(const std::string &line);

    std::ofstream _log_file;
    std::istream &_input;
};