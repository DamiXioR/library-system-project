#ifndef MOCK_ILOG_SINK_HPP
#define MOCK_ILOG_SINK_HPP

#include <gmock/gmock.h>
#include "ILogSink.hpp"

namespace LogSys {

class MockLogSink : public ILogSink {
public:
    MOCK_METHOD(LogLevel,
                getMinLevel,
                (),
                (const, override));

    MOCK_METHOD(bool,
                acceptsChannel,
                (const std::string& requestedChannel),
                (const, override));

    MOCK_METHOD(bool,
                isConsoleLog,
                (),
                (const, override));

    MOCK_METHOD(void,
                execute,
                (const std::string& finalLog),
                (const, override));
};

} // namespace LogSys

#endif // MOCK_ILOG_SINK_HPP