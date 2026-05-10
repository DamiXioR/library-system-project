#ifndef THREAD_POOL_HPP
#define THREAD_POOL_HPP

#include <cstdint>
#include <queue>
#include <memory>
#include <atomic>

#include "ILogger.hpp"
#include "IThreadPool.hpp"

namespace ThPool {

class ThreadPool : public IThreadPool {
public:
    ThreadPool(const LogSys::ILogger& logger,
            uint16_t threadCount)
            : logger(logger), 
            stopThreadWorkers(false) {
        initializeThreads(std::move(threadCount));
    }
    ~ThreadPool() {
        shutdown();
    }

    auto addTask(std::function<void()> task) -> void override {
        if (stopThreadWorkers) {
            throw std::runtime_error("ThreadPool is shutting down");
        }
        {
            std::lock_guard<std::mutex> lockGuard {tpMtx};
            tasks.push(std::move(task));
        }
    }

private:
    void shutdown() {
        {
            std::lock_guard<std::mutex> lockGuard {tpMtx};
            stopThreadWorkers = true;
        }
        cv.notify_all();

        for(auto& worker : workers) {
            if (worker.joinable()){
                worker.join();
            }
        }
    }

    void workerLoop() {
        // TODO: Add logger, make tread safe logs
        static thread_local uint16_t id = [] {
            static std::atomic<uint16_t> counter{0};
            return counter++;
        }();

        while (true) {
            std::function<void()> task;
            {
                std::unique_lock<std::mutex> uniqLock {tpMtx};
                
                cv.wait(uniqLock, [this](){
                    return stopThreadWorkers || !tasks.empty();
                });

                if(stopThreadWorkers && tasks.empty()){
                    return;
                }

                task = tasks.front();
                tasks.pop();
            }
            task();
        }
    }

    void initializeThreads(uint16_t threadCount) {
        workers.reserve(threadCount);
        for (size_t i = 0; i < threadCount; ++i) {
            workers.emplace_back([this] {
                workerLoop();
            });
        }
    }

    const LogSys::ILogger& logger;
    std::queue<std::function<void()>> tasks;
    std::vector<std::thread> workers;
    std::atomic<bool> stopThreadWorkers;
    std::mutex tpMtx;
    std::condition_variable cv;
};

} // namespace ThPool

#endif // THREAD_POOL_HPP
