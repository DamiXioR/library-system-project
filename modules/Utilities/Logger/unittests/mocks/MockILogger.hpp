#ifndef MOCK_ILOGGER_HPP
#define MOCK_ILOGGER_HPP

#include <gmock/gmock.h>

#include <memory>
#include <string>
#include <source_location>

#include "ILogger.hpp"

namespace LogSys {

class MockILogger : public ILogger {
public:
    MOCK_METHOD(void,
                addSink,
                (std::unique_ptr<ILogSink> logSink),
                (override));

    MOCK_METHOD(void,
                clearSinks,
                (),
                (override));

    MOCK_METHOD(void,
                logFinalize,
                (const FullLog& logMessage),
                (override));

    MOCK_METHOD(void,
                log,
                (const LogLevel& logLevel,
                 const std::string& logChannel,
                 const LogMessage& logText,
                 std::source_location loc),
                (override));
};

} // namespace LogSys

#endif // MOCK_ILOGGER_HPP