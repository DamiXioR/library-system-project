#include <gtest/gtest.h>

#include <chrono>
#include <memory>
#include <string_view>

#include "ConsoleLogSink.hpp"
#include "FileLogSink.hpp"
#include "EnvVarFetcher.hpp"

using namespace ::testing;
using namespace LogSys;

class ConsoleLogSinkTests : public Test {
protected:
  void SetUp() override {
      minLogLevel = LogLevel::Info;
      std::initializer_list<std::string> acceptedChannels = {"Book", "DataBase"};
      consoleLogSink = std::make_unique<ConsoleLogSink>(minLogLevel, acceptedChannels);
  }

  std::unique_ptr<ConsoleLogSink> consoleLogSink;
  LogLevel minLogLevel;
  std::initializer_list<std::string> acceptedChannels;
};

TEST_F(ConsoleLogSinkTests, AcceptsChannelShouldReturnTrueIfChannelExistsForRequestedSinkOrFalseOtherwise) {
  auto bookChannel {"Book"};
  auto dataBaseChannel {"DataBase"};
  ASSERT_TRUE(consoleLogSink->acceptsChannel(bookChannel));
  ASSERT_TRUE(consoleLogSink->acceptsChannel(dataBaseChannel));

  auto notExistedChannel {"HelloWorld"};
  ASSERT_FALSE(consoleLogSink->acceptsChannel(notExistedChannel));
}

TEST_F(ConsoleLogSinkTests, getMinLevelShouldReturnMinimumLevelOfLogWhichIsSupportedBySink) {
  LogLevel otherLogLevel {LogLevel::Debug};
  ASSERT_EQ(consoleLogSink->getMinLevel(), minLogLevel);
  ASSERT_NE(consoleLogSink->getMinLevel(), otherLogLevel);
}

TEST_F(ConsoleLogSinkTests, isConsoleLogShouldReturnTrueOnlyForConsoleLogSink) {
  ASSERT_TRUE(consoleLogSink->isConsoleLog());
}

class FileLogSinkTests : public Test {
protected:
  void SetUp() override {
      appCfg = EnvVar::EnvVarFetcher::loadOrThrow();

      minLogLevel = LogLevel::Info;
      std::initializer_list<std::string> acceptedChannels = {"Book", "DataBase"};
      fileLogSink = std::make_unique<FileLogSink>(appCfg, minLogLevel, appCfg.testLogPath, acceptedChannels);
  }

  std::string getLastLine(std::string pathToFile) {
    std::string getLastLineCmd {"tail -n 1 " + pathToFile};
    FILE* pipe = popen(getLastLineCmd.c_str(), "r");
    
    uint16_t noOfBytesInBuffer {128};
    char buffer[noOfBytesInBuffer];
    std::string result;

    while (fgets(buffer, sizeof(buffer), pipe)) {
        result += buffer;
    }

    pclose(pipe);

        while (!result.empty() &&
          (result.back() == '\n' || result.back() == '\r')) {
        result.pop_back();
    }

    return result;
  }

  std::string getCurrentLocalTime() {
    const auto now = std::chrono::system_clock::now();
    const auto locTime = std::chrono::current_zone()->to_local(now);
    return std::format("{:%d-%m-%Y %T}", locTime);
  }

  AppCfg::AppConfig appCfg;
  std::unique_ptr<FileLogSink> fileLogSink;
  LogLevel minLogLevel;
  std::string logFilePath;
  std::initializer_list<std::string> acceptedChannels;
};

TEST_F(FileLogSinkTests, AcceptsChannelShouldReturnTrueIfChannelExistsForRequestedSinkOrFalseOtherwise) {
  const auto bookChannel {"Book"};
  const auto dataBaseChannel {"DataBase"};
  ASSERT_TRUE(fileLogSink->acceptsChannel(bookChannel));
  ASSERT_TRUE(fileLogSink->acceptsChannel(dataBaseChannel));

  const auto notExistedChannel {"HelloWorld"};
  ASSERT_FALSE(fileLogSink->acceptsChannel(notExistedChannel));
}

TEST_F(FileLogSinkTests, getMinLevelShouldReturnMinimumLevelOfLogWhichIsSupportedBySink) {
  LogLevel otherLogLevel {LogLevel::Debug};
  ASSERT_EQ(fileLogSink->getMinLevel(), minLogLevel);
  ASSERT_NE(fileLogSink->getMinLevel(), otherLogLevel);
}

TEST_F(FileLogSinkTests, isConsoleLogShouldReturnTrueOnlyForConsoleLogSink) {
  ASSERT_FALSE(fileLogSink->isConsoleLog());
}

TEST_F(FileLogSinkTests, executeShouldWriteLogIntoFilePath) {
    const auto timeOfLatestLog {getCurrentLocalTime()};
    std::string exampleLog {"[INFO][Book][" + timeOfLatestLog + "][./app/main.cpp:777] Hello World!"};

    fileLogSink->execute(exampleLog);
    std::string lastLog {getLastLine(appCfg.testLogPath)};

    ASSERT_EQ(lastLog, exampleLog);
}

TEST_F(FileLogSinkTests, executeShouldHandleExternalPathGracefully) {
  const std::string exampleExtPath {"/home/user/some/place/for/logs/log.log"};
  const auto fileLogSinkWithExternalLogPath = std::make_unique<FileLogSink>(appCfg, minLogLevel, exampleExtPath, acceptedChannels);
  std::string exampleLog {"test"};
  ASSERT_NO_THROW(fileLogSinkWithExternalLogPath->execute(exampleLog));
}
