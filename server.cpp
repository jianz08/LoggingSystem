#include <iostream>
#include <string>
#include <fstream>

using namespace std;

class LogImpl
{
public:
    LogImpl(std::string fileName)
    {
        _log_file.open(fileName);
    }
    ~LogImpl()
    {
        _log_file.close();
    }

    void process()
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

private:
    ofstream _log_file;
    
    std::string inputLog()
    {
        std::string line;
        std::getline(std::cin, line);
        return line;
    }

    void saveLog(const std::string &line)
    {
        _log_file << line << std::endl;
    }
};

int main()
{
    LogImpl logImpl("logs.txt");

    logImpl.process();

    return 0;
}