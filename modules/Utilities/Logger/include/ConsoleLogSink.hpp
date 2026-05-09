#ifndef CONSOLE_LOG_SINK_HPP
#define CONSOLE_LOG_SINK_HPP

#include <unordered_set>
#include <initializer_list>
#include <string>
#include <cstdint>

#include "ILogSink.hpp"
#include "LogSinkHelpers.hpp"

namespace LogSys {

class ConsoleLogSink : public ILogSink {
public:
    explicit 
    ConsoleLogSink(LogLevel minLogLevel,
                    const std::initializer_list<std::string>& logChannels)
                    : minLevel(minLogLevel),
                    channels({std::move(logChannels)})
    {}
    ConsoleLogSink(const ConsoleLogSink& other) = delete;
    ConsoleLogSink(ConsoleLogSink&& other) = default;

    ConsoleLogSink& operator=(const ConsoleLogSink& other) = delete;
    ConsoleLogSink& operator=(ConsoleLogSink&& other) = default;

    auto getMinLevel() const -> LogLevel override {
        return minLevel;
    }

    auto acceptsChannel(std::string_view requestedChannel) const -> bool override {
        return channels.contains(requestedChannel) ||
            channels.contains(MAIN_CHANNEL);
    }

    auto isConsoleLog() const -> bool override { return true; }

    auto execute(std::string_view finalLog) const -> void override {
        printOnScreen(finalLog);
    }

private:
    auto printOnScreen(std::string_view logMessage) const -> void {
        std::cout << logMessage << "\n";
    }

    std::unordered_set<std::string, TransparentHash, std::equal_to<>> channels;
    LogLevel minLevel;
};

} // namespace LogSys

#endif // CONSOLE_LOG_SINK_HPP
