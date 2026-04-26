#ifndef LOG_SINK_HPP
#define LOG_SINK_HPP

#include <unordered_set>
#include <initializer_list>
#include <string>
#include <cstdint>

#include "ILogSink.hpp"

namespace LogSys {

class FileLogSink : public ILogSink {
public:
    explicit FileLogSink(const LogLevel& minLogLevel,
                    const std::string& logFilePath,
                    const std::initializer_list<std::string>& logChannels)
                    : minLevel(minLogLevel),
                    filePath(std::move(logFilePath)),
                    channels({std::move(logChannels)})
    {}

    auto getMinLevel() const -> LogLevel override {
        return minLevel;
    }

    auto acceptsChannel(const std::string& requestedChannel) const -> bool override {
        // TODO: Name of the main channel should be fetched from env or json file
        return channels.contains(requestedChannel) ||
            channels.contains("Main");
    }

    auto execute(const std::string& finalLog) const -> void override {
        writeToFile(finalLog);
    }

private:
    auto getFilePath() const -> const std::string& {
        return filePath;
    }

    auto writeToFile(const std::string& logMessage) const -> void {
        // TODO: write to file
        std::cout << "Write to file: " << logMessage << "\n";
    }

    std::unordered_set<std::string> channels;
    LogLevel minLevel;
    std::string filePath;
};

} // namespace LogSys

#endif // LOG_SINK_HPP
