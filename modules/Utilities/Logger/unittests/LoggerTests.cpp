#include <gtest/gtest.h>
#include <gmock/gmock.h>

#include <memory>

#include "Logger.hpp"
#include "MockILogSink.hpp"

/* 
  Mocks are managed via std::unique_ptr and moved into Logger.
  Memcheck reports may include false positives due to GoogleTest/GoogleMock
  internal allocations and teardown behavior.
*/

using namespace ::testing;
using namespace LogSys;

class LoggerTests : public Test {
public:
  std::unique_ptr<Logger> logger;
  std::unique_ptr<StrictMock<MockILogSink>> mockILogSink;
  MockILogSink* rawMockILogSink {nullptr};

protected:
  void SetUp() override {
    logger = std::make_unique<Logger>();
    mockILogSink = std::make_unique<StrictMock<MockILogSink>>();
    rawMockILogSink = mockILogSink.get();
    logger->addSink(std::move(mockILogSink));
  }
};

TEST_F(LoggerTests, LogMessageIsOmittedWhenChannelDoesNotExist) {
  EXPECT_CALL(*rawMockILogSink, acceptsChannel(_)).WillOnce(Return(false));
  logger->log(LogLevel::Debug, "Book", LogMessage("Test message with {} placeholder and {} placeholder", "the first", "the second"));
}

TEST_F(LoggerTests, LogMessageIsOmittedWhenRequestedLogLevelIsSmallerThanSinkMinLogLevel) {
  EXPECT_CALL(*rawMockILogSink, acceptsChannel(_)).WillOnce(Return(true));
  EXPECT_CALL(*rawMockILogSink, getMinLevel()).WillOnce(Return(LogLevel::Info));
  logger->log(LogLevel::Debug, "Book", LogMessage("Test message with {} placeholder and {} placeholder", "the first", "the second"));
}

TEST_F(LoggerTests, PrintsToConsoleOnlyOnceWhenMultipleSinksAcceptSameChannel) {
  auto mockILogSink2 {std::make_unique<StrictMock<MockILogSink>>()};
  auto rawMockILogSink2 = mockILogSink2.get();
  logger->addSink(std::move(mockILogSink2));

  EXPECT_CALL(*rawMockILogSink, acceptsChannel(_)).WillOnce(Return(true));
  EXPECT_CALL(*rawMockILogSink2, acceptsChannel(_)).WillOnce(Return(true));

  EXPECT_CALL(*rawMockILogSink, getMinLevel()).WillOnce(Return(LogLevel::Debug));
  EXPECT_CALL(*rawMockILogSink2, getMinLevel()).WillOnce(Return(LogLevel::Debug));

  EXPECT_CALL(*rawMockILogSink, isConsoleLog()).WillRepeatedly(Return(true));
  EXPECT_CALL(*rawMockILogSink2, isConsoleLog()).WillRepeatedly(Return(true));

  EXPECT_CALL(*rawMockILogSink, execute(_)).Times(1);
  EXPECT_CALL(*rawMockILogSink2, execute(_)).Times(0);

  logger->log(LogLevel::Info, "Book", LogMessage("Test message with {} placeholder and {} placeholder", "the first", "the second"));
}
