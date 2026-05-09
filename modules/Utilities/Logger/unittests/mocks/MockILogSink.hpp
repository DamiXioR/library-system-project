#ifndef MOCK_ILOG_SINK_HPP
#define MOCK_ILOG_SINK_HPP

#include <gmock/gmock.h>
#include "ILogSink.hpp"

namespace LogSys {

class MockILogSink : public ILogSink {
public:
    MOCK_METHOD(LogLevel,
                getMinLevel,
                (),
                (const, override));

    MOCK_METHOD(bool,
                acceptsChannel,
                (std::string_view requestedChannel),
                (const, override));

    MOCK_METHOD(bool,
                isConsoleLog,
                (),
                (const, override));

    MOCK_METHOD(void,
                execute,
                (std::string_view finalLog),
                (const, override));
};

} // namespace LogSys

#endif // MOCK_ILOG_SINK_HPP