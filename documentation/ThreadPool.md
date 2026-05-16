# Utilities

# ThreadPool

A lightweight C++ thread pool implementation for executing asynchronous tasks using a fixed number of worker threads.

## Overview

`ThreadPool` provides a simple interface for scheduling tasks (`std::function<void()>`) to be executed by a pool of worker threads. It manages thread creation, task distribution, and graceful shutdown.

The implementation is located in the `ThPool` namespace and follows the `IThreadPool` interface.

## Features

- Fixed-size thread pool
- Thread-safe task queue
- Automatic worker thread lifecycle management
- Graceful shutdown on destruction
- Simple task submission API

## Usage

### Creating a ThreadPool

```cpp
LogSys::Logger logger;
ThPool::ThreadPool pool(logger, 4); // 4 worker threads
```

### Adding tasks

```cpp
pool.addTask([]() {
    std::cout << "Task executed on thread pool\n";
});
```

## Lifecycle

Threads are created during construction via `initializeThreads()`.

Each worker runs `workerLoop()` and waits for tasks using a condition variable.

The destructor calls `shutdown()`, which:

- Stops all workers
- Wakes all threads
- Joins them safely

## Thread Safety

- The task queue is protected by `std::mutex`
- `std::condition_variable` is used for efficient waiting
- `std::atomic<bool>` controls the shutdown state

## Additional information

- If `addTask()` is called after shutdown begins, it throws a `std::runtime_error`
- Each worker continuously processes tasks until shutdown and the queue becomes empty
- Thread IDs are generated using `thread_local` storage (currently unused for logging)
