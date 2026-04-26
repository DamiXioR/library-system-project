#include <gtest/gtest.h>

#include <memory>

#include "ConsoleLogSink.hpp"
#include "FileLogSink.hpp"

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
      minLogLevel = LogLevel::Info;
      logFilePath = {"/path/to/logs/program.log"};
      std::initializer_list<std::string> acceptedChannels = {"Book", "DataBase"};
      fileLogSink = std::make_unique<FileLogSink>(minLogLevel, logFilePath, acceptedChannels);
  }

  std::unique_ptr<FileLogSink> fileLogSink;
  LogLevel minLogLevel;
  std::string logFilePath;
  std::initializer_list<std::string> acceptedChannels;
};

TEST_F(FileLogSinkTests, AcceptsChannelShouldReturnTrueIfChannelExistsForRequestedSinkOrFalseOtherwise) {
  auto bookChannel {"Book"};
  auto dataBaseChannel {"DataBase"};
  ASSERT_TRUE(fileLogSink->acceptsChannel(bookChannel));
  ASSERT_TRUE(fileLogSink->acceptsChannel(dataBaseChannel));

  auto notExistedChannel {"HelloWorld"};
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
  fileLogSink->execute("test");
}
