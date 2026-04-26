#ifndef LOGGER_HPP
#define LOGGER_HPP

#include <string_view>
#include <string>
#include <format>
#include <chrono>
#include <source_location>
#include <iostream>

#include "ILogger.hpp"
#include "ILogSink.hpp"
#include "LoggerHelpers.hpp"

namespace LogSys {

class Logger : public ILogger {
public:
    Logger() = default;
    Logger(const Logger& logger) = delete;
    Logger(Logger&& logger) = default;

    Logger& operator=(const Logger& logger) = delete;
    Logger& operator=(Logger&& logger) = default;

    auto addSink(std::unique_ptr<ILogSink> logSink) -> void {
        sinks.emplace_back(std::move(logSink));
    }

    auto clearSinks() -> void {
        sinks.clear();
    }

    auto log(const LogLevel& logLevel,
             const std::string& logChannel,
             const LogMessage& logText,
             std::source_location loc = std::source_location::current()) -> void {
        
        const FullLog logMessage {
            .level = logLevel,
            .channel = logChannel,
            .timestamp = getCurrentTime(),
            .sourceLoc = loc,
            .message = logText.getLogText()
        };

        logFinalize(logMessage);
    }

private:
    auto logFinalize(const FullLog& logMessage) -> void {
        const auto finalLog = LogFormatter::format(logMessage);

        bool screenUsed = false;

        for (const auto& sink : sinks) {
            if (!(sink->acceptsChannel(logMessage.channel))) {
                continue;
            }

            if (logMessage.level > sink->getMinLevel()) {
                continue;
            }

            if (sink->isConsoleLog()) {
                if (!screenUsed) {
                    sink->execute(finalLog);
                    screenUsed = true;
                }
            } else {
                sink->execute(finalLog);
            }
        }
    }

    auto getCurrentTime() -> std::string {
        using namespace std::chrono;
        auto now = system_clock::now();
        auto local = current_zone()->to_local(now);

        return std::format("{:%d-%m-%Y %H:%M:%S}", local);
    }

    std::vector<std::unique_ptr<ILogSink>> sinks;
};

} // namespace LogSys

#endif // LOGGER_HPP
