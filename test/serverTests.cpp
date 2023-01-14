#include <sstream>
#include <fstream>
#include <vector>
#include <gtest/gtest.h>
#include "../server.hpp"

TEST(serverTests, testInputOneLine){
    std::string text = "23982565683###tom###INFO:process started";
    std::string fileName = "logsTest.txt";
    std::istringstream input(text);
    LogImpl LogImpl(input, fileName);
    LogImpl.process();

    std::ifstream file(fileName);
    std::string line;
    std::getline(file, line);
    EXPECT_EQ(text, line);
    std::getline(file, line);
    EXPECT_TRUE(line.empty());
}

TEST(serverTests, testInputMultipleLines){
    std::string text = "23982565683###tom###INFO:process started\n239825683###tom###INFO:process ended\n";
    std::string fileName = "logsTest.txt";
    std::istringstream input(text);
    LogImpl LogImpl(input, fileName);
    LogImpl.process();

    std::ifstream file(fileName);
    std::string output;
    std::string line;
    while (std::getline(file, line)) {
        output += line + "\n";
    }
    EXPECT_EQ(text, output);
    
}