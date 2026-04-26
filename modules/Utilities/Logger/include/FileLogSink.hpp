#ifndef LOG_SINK_HPP
#define LOG_SINK_HPP

#include <unordered_set>
#include <initializer_list>
#include <string>
#include <cstdint>
#include <filesystem>

#include "ILogSink.hpp"

namespace LogSys {

class FileLogSink : public ILogSink {
public:
    explicit FileLogSink(LogLevel minLogLevel,
                    const std::string& logFilePath,
                    const std::initializer_list<std::string>& logChannels)
                    : minLevel(minLogLevel),
                    filePath(logFilePath),
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
    auto writeToFile(const std::string& logMessage) const -> void {
        // TODO: Should write to file
        std::cout << "Write to file: " << logMessage << "\n";
        std::cout << "Filepath: " << filePath.string() << "\n";
    }

    std::unordered_set<std::string> channels;
    LogLevel minLevel;
    std::filesystem::path filePath;
};

} // namespace LogSys

#endif // LOG_SINK_HPP
