#include <gtest/gtest.h>
#include <gmock/gmock.h>

#include <chrono>
#include <future>
#include <memory>

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

  LogSys::MockILogger mockILogger;
  std::unique_ptr<ThreadPool> threadPool;
};

TEST_F(ThreadPoolTests, TaskShouldBeExecutedInThreadPool) {
    using namespace std::chrono_literals;
    std::promise<void> promise;
    auto future = promise.get_future();

    threadPool->addTask([&promise]() {
        promise.set_value();
    });

    ASSERT_EQ(future.wait_for(1s),
              std::future_status::ready);
}
