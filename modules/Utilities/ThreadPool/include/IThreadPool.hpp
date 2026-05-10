#ifndef ITHREAD_POOL_HPP
#define ITHREAD_POOL_HPP

#include <functional>

namespace ThPool {

class IThreadPool {
public:
    virtual ~IThreadPool() = default;

    virtual auto addTask(std::function<void()> task) -> void = 0;
};

} // namespace ThPool

#endif // ITHREAD_POOL_HPP
