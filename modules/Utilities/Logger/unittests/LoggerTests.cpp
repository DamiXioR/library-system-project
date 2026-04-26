#include <gtest/gtest.h>
#include <gmock/gmock.h>

#include <memory>

#include "Logger.hpp"
#include "MockILogSink.hpp"


using namespace LogSys;

class LoggerTests : public ::testing::Test {
public:
  std::unique_ptr<Logger> logger;

protected:
  void SetUp() override {
    logger = std::make_unique<Logger>();
  }
};

TEST_F(LoggerTests, LogMessageContainsLogLevelTimeAndSourceLocationInformation) {
  logger->log(LogLevel::Info, "Book", LogMessage("Message no {}", 123));
}
