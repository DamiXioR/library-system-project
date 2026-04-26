#ifndef LOGGER_HELPERS_HPP
#define LOGGER_HELPERS_HPP

#include <unordered_set>
#include <initializer_list>
#include <string>
#include <cstdint>

namespace LogSys {

enum class LogLevel {
    Info,
    Warning,
    Error,
    Exception,
    Debug,
};

inline std::string_view logLevelToString(LogLevel level) {
    switch (level) {
        case LogLevel::Info:        return "INFO";
        case LogLevel::Warning:     return "WARN";
        case LogLevel::Error:       return "ERROR";
        case LogLevel::Exception:   return "EXCEPTION";
        case LogLevel::Debug:       return "DEBUG";
        default:                    return "UNKNOWN";
    }
}

enum class LogMode {
    FileOnly,
    ScreenOnly,
    ScreenAndFile
};

class LogMessage {
public:
    template <typename... Args>
    LogMessage(std::string_view formatString, Args&&... args)
        : text(std::vformat(formatString, std::make_format_args(args...)))
    {}

    auto getLogText() const -> const std::string& {
        return text;
    }

private:
    std::string text;
};

struct FullLog {
    LogLevel level;
    std::string channel;
    std::string timestamp;
    std::source_location sourceLoc;
    std::string message;
};

class LogFormatter {
public:
    static auto format(const FullLog& msg) -> std::string {
        return std::format("[{}][{}][{}][{}:{}] {}",
            logLevelToString(msg.level),
            msg.channel,
            msg.timestamp,
            msg.sourceLoc.file_name(),
            msg.sourceLoc.line(),
            msg.message
        );
    }
};

} // namespace LogSys

#endif // LOGGER_HELPERS_HPP
