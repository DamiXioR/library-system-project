#ifndef LOG_SINK_HPP
#define LOG_SINK_HPP

#include <unordered_set>
#include <initializer_list>
#include <string>
#include <cstdint>
#include <filesystem>
#include <fstream>

#include "ILogSink.hpp"
#include "LogSinkHelpers.hpp"
#include "EnvVarFetcher.hpp"

namespace LogSys {

namespace fs = std::filesystem;

class FileLogSink : public ILogSink {
public:
    explicit FileLogSink(const AppCfg::AppConfig& appConfiguration,
                    LogLevel minLogLevel,
                    const std::string& logFilePath,
                    const std::initializer_list<std::string>& logChannels)
                    : appCfg(appConfiguration),
                    minLevel(minLogLevel),
                    filePath(logFilePath),
                    channels({std::move(logChannels)})
    {}

    auto getMinLevel() const -> LogLevel override {
        return minLevel;
    }

    auto acceptsChannel(std::string_view requestedChannel) const -> bool override {
        return channels.contains(requestedChannel) ||
            channels.contains(MAIN_CHANNEL);
    }

    auto execute(std::string_view finalLog) const -> void override {
        try {
        writeToFile(finalLog);
        } catch (std::runtime_error& re){
            std::cerr << re.what() << "\n";
        }
    }

private:
    bool isInsideRepo(const fs::path& target) const {
        fs::path repo = fs::weakly_canonical(appCfg.get().repoRoot);
        fs::path path = fs::weakly_canonical(target);

        auto repoStr = repo.string();
        auto pathStr = path.string();

        return pathStr.starts_with(repoStr);
    }

    auto writeToFile(std::string_view logMessage) const -> void {
        if(!fs::exists(filePath)){
            if (!isInsideRepo(filePath) && !appCfg.get().allowOutsideLogs){
                throw std::runtime_error(
                    "Log file path is outside the repository. "
                    "If this behavior is expected, set ALLOW_OUTSIDE_LOG_PATHS=true in .env file."
                );
            }
            if (!fs::exists(filePath.parent_path())){
                fs::create_directories(filePath.parent_path());
            }
            if (!fs::exists(filePath)){
                auto logFile {std::ofstream(filePath.c_str())};
            }
        }
        auto logFile {std::ofstream(filePath.c_str(), std::ios::app)};
        logFile << logMessage << "\n";
    }

    std::reference_wrapper<const AppCfg::AppConfig> appCfg;
    std::unordered_set<std::string, TransparentHash, std::equal_to<>> channels;
    LogLevel minLevel;
    fs::path filePath;
};

} // namespace LogSys

#endif // LOG_SINK_HPP
