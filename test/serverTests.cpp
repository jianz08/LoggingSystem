#include <sstream>
#include <fstream>
#include <vector>
#include "gtest/gtest.h"
#include "gmock/gmock.h"
#include "server.hpp"

class MockMyOstream : public MyOstream
{
public:
    MOCK_METHOD(void, log, (std::string const &), (override));
};

TEST(serverTests, testInputOneLineMockMyOstream)
{
    std::string text = "23982565683###tom###INFO:process started";
    std::istringstream input(text);
    MockMyOstream mockMyOstream;
    EXPECT_CALL(mockMyOstream, log(text)).Times(1);

    LogImpl LogImpl(input, mockMyOstream);
    LogImpl.process();
}

TEST(serverTests, testInputMultipleLinesMockMyOstream)
{
    testing::InSequence seq;
    std::string line1 = "23982565683###tom###INFO:process started";
    std::string line2 = "239825683###tom###INFO:process ended";
    std::string text = line1 + "\n" + line2;
    std::istringstream input(text);
    MockMyOstream mockMyOstream;
    EXPECT_CALL(mockMyOstream, log(line1)).Times(1);
    EXPECT_CALL(mockMyOstream, log(line2)).Times(1);

    LogImpl LogImpl(input, mockMyOstream);
    LogImpl.process();
}

TEST(serverTests, testInputMultipleLines)
{
    std::string text = "23982565683###tom###INFO:process started\n239825683###tom###INFO:process ended\n";
    std::string fileName = "logsTest.txt";
    std::istringstream input(text);
    std::ostringstream output;
    MyOstreamImpl myOstreamImpl(output);
    LogImpl LogImpl(input, myOstreamImpl);
    LogImpl.process();

    std::string line = output.str();
    EXPECT_EQ(text, line);
}