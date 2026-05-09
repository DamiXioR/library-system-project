#ifndef ILOG_SINK_HPP
#define ILOG_SINK_HPP

#include <unordered_set>
#include <string>

#include "LoggerHelpers.hpp"

namespace LogSys {

constexpr std::string MAIN_CHANNEL {"*"};

class ILogSink {
public:
    virtual ~ILogSink() = default;

    virtual auto getMinLevel() const -> LogLevel = 0;
    virtual auto acceptsChannel(std::string_view requestedChannel) const -> bool = 0;
    virtual auto isConsoleLog() const -> bool { return false; }
    virtual auto execute(std::string_view finalLog) const -> void = 0;
};

} // namespace LogSys

#endif // ILOG_SINK_HPP
