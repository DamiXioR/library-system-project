#ifndef ILOGGER_HPP
#define ILOGGER_HPP

#include <memory>
#include <string>
#include <source_location>

#include "ILogSink.hpp"
#include "LoggerHelpers.hpp"

namespace LogSys {

class ILogger {
public:
    virtual ~ILogger() = default;
    virtual auto addSink(std::unique_ptr<ILogSink> logSink) -> void = 0;
    virtual auto clearSinks() -> void = 0;
    virtual auto logFinalize(const FullLog& logMessage) -> void = 0;
    virtual auto log(const LogLevel& logLevel,
             const std::string& logChannel,
             const LogMessage& logText,
             std::source_location loc = std::source_location::current()) -> void = 0;
};

} // namespace LogSys

#endif // ILOGGER_HPP
