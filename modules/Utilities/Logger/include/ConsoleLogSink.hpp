#ifndef CONSOLE_LOG_SINK_HPP
#define CONSOLE_LOG_SINK_HPP

#include <unordered_set>
#include <initializer_list>
#include <string>
#include <cstdint>

#include "ILogSink.hpp"

namespace LogSys {

class ConsoleLogSink : public ILogSink {
public:
    explicit 
    ConsoleLogSink(const LogLevel& minLogLevel,
                    const std::initializer_list<std::string>& logChannels)
                    : minLevel(minLogLevel),
                    channels({std::move(logChannels)})
    {}

    auto getMinLevel() const -> LogLevel override {
        return minLevel;
    }

    auto acceptsChannel(const std::string& requestedChannel) const -> bool override {
        return channels.contains(requestedChannel) ||
            channels.contains("Main");
    }

    auto isConsoleLog() const -> bool override { return true; }

    auto execute(const std::string& finalLog) const -> void override {
        printOnScreen(finalLog);
    }

private:
    auto printOnScreen(const std::string& logMessage) const -> void {
        std::cout << "Printing: " << logMessage << "\n";
    }

    std::unordered_set<std::string> channels;
    LogLevel minLevel;
};

} // namespace LogSys

#endif // CONSOLE_LOG_SINK_HPP
