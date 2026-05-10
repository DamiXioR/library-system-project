#include <gtest/gtest.h>

#include "ThreadPool.hpp"
#include "MockILogger.hpp"

using namespace ::testing;
using namespace LogSys;
using namespace ThPool;

class ThreadPoolTests : public Test {
protected:
  void SetUp() override {
    const uint16_t countOfThreads {4};
    threadPool = std::make_unique<ThreadPool>(mockILogger, countOfThreads);
  }

  NiceMock<LogSys::MockILogger> mockILogger;
  std::unique_ptr<ThreadPool> threadPool;
};

TEST_F(ThreadPoolTests, ToDo) {
    bool taskExecuted {false};
    threadPool->addTask([&taskExecuted](){
        taskExecuted = true;
    });
    float waitForTaskInSec {0.1};
    sleep(waitForTaskInSec);
    ASSERT_TRUE(taskExecuted);
}
