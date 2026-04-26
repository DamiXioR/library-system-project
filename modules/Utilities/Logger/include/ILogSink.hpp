#ifndef ILOG_SINK_HPP
#define ILOG_SINK_HPP

#include <unordered_set>
#include <string>

#include "LoggerHelpers.hpp"

namespace LogSys {

class ILogSink {
public:
    virtual ~ILogSink() = default;

    virtual auto getMinLevel() const -> LogLevel = 0;
    virtual auto acceptsChannel(const std::string& requestedChannel) const -> bool = 0;
    virtual auto isConsoleLog() const -> bool { return false; }
    virtual auto execute(const std::string& finalLog) const -> void = 0;
};

} // namespace LogSys

#endif // ILOG_SINK_HPP
