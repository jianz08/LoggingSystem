#include <sstream>
#include <fstream>
#include <vector>
#include "gtest/gtest.h"
#include "gmock/gmock.h"
#include "server.hpp"

class MockMyOstream : public Writer
{
public:
    //MOCK_METHOD(void, log, (std::string const &), (override));
    MOCK_METHOD(void, log, (LogInfo const &), (override));
};

LogInfo const logInfo1 = {
    23982565683,
    "tom",
    "INFO",
    "process started"
};

LogInfo const logInfo2 = {
    239825683,
    "tom",
    "INFO",
    "process ended"
};

TEST(serverTests, testInputOneLineMockMyOstream)
{
    std::string text = "23982565683###tom###INFO:process started";
    std::istringstream input(text);
    StreamReader streamReader(input);
    MockMyOstream mockMyOstream;
    EXPECT_CALL(mockMyOstream, log(logInfo1)).Times(1);

    LogImpl LogImpl(streamReader, mockMyOstream);
    LogImpl.process();
}

TEST(serverTests, testInputOneLineMockMyOstreamInvalid)
{
    std::string text = "23982565683###tom###INFO:process## started";
    std::istringstream input(text);
    StreamReader streamReader(input);
    MockMyOstream mockMyOstream;
    EXPECT_CALL(mockMyOstream, log(testing::_)).Times(0);

    LogImpl LogImpl(streamReader, mockMyOstream);
    LogImpl.process();
}

TEST(serverTests, testInputMultipleLinesMockMyOstream)
{
    testing::InSequence seq;
    std::string line1 = "23982565683###tom###INFO:process started";
    std::string line2 = "239825683###tom###INFO:process ended";
    std::string text = line1 + "\n" + line2;
    std::istringstream input(text);
    StreamReader streamReader(input);
    MockMyOstream mockMyOstream;
    EXPECT_CALL(mockMyOstream, log(logInfo1)).Times(1);
    EXPECT_CALL(mockMyOstream, log(logInfo2)).Times(1);

    LogImpl LogImpl(streamReader, mockMyOstream);
    LogImpl.process();
}

TEST(serverTests, testInputMultipleLines)
{
    std::string text = "23982565683###tom###INFO:process started\n239825683###tom###INFO:process ended\n";
    std::istringstream input(text);
    StreamReader streamReader(input);
    std::ostringstream output;
    SingleStreamWriter myOstreamImpl(output);
    LogImpl LogImpl(streamReader, myOstreamImpl);
    LogImpl.process();

    std::string line = output.str();
    EXPECT_EQ("23982565683\ttom\tINFO\tprocess started\n239825683\ttom\tINFO\tprocess ended\n", line);
}


TEST(serverTests, testValidateLog)
{
    LogInfo logInfo;
    std::string valid_line = "23982565683###tom###INFO:process started";
    EXPECT_TRUE(validateLog(valid_line, logInfo));
    EXPECT_EQ(logInfo.timestamp, 23982565683);
    EXPECT_EQ(logInfo.user, "tom");
    EXPECT_EQ(logInfo.infoType, "INFO");
    EXPECT_EQ(logInfo.info, "process started");
    valid_line = "239825683###tom###INFO:process ended";
    EXPECT_TRUE(validateLog(valid_line, logInfo));
    valid_line = "239825683###tom###INFO:process";
    EXPECT_TRUE(validateLog(valid_line, logInfo));
    valid_line = "239825683###tom###INFO:process end foo  bar";
    EXPECT_TRUE(validateLog(valid_line, logInfo));
    valid_line = "239825683###tom###INFO: ";
    EXPECT_TRUE(validateLog(valid_line, logInfo));

    std::string invalid_line = "239825683####tom###INFO:process ended";
    EXPECT_FALSE(validateLog(invalid_line, logInfo));
    invalid_line = "239825683###tom####INFO:process ended";
    EXPECT_FALSE(validateLog(invalid_line, logInfo));
    invalid_line = "239825683###tom###INFO:process# ended";
    EXPECT_FALSE(validateLog(invalid_line, logInfo));
    invalid_line = "239825683###tom###INFO process ended";
    EXPECT_FALSE(validateLog(invalid_line, logInfo));
    invalid_line = "239825683###tom###INFO:process# ended";
    EXPECT_FALSE(validateLog(invalid_line, logInfo));
    invalid_line = "239825683###tom###INFO:";
    EXPECT_FALSE(validateLog(invalid_line, logInfo));
}