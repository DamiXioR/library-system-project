#ifndef ENV_VAR_FETCHER_HPP
#define ENV_VAR_FETCHER_HPP

#include <stdexcept>
#include <cstdlib>
#include <algorithm>
#include <ranges>
#include <string>

#include "AppConfig.hpp"

namespace EnvVar {

using namespace AppCfg;

class EnvVarFetcher {
public:
    static std::string requireEnv(const char* key) {
        const char* value = std::getenv(key);

        if (!value) {
            throw std::runtime_error(
                std::string("Missing required env var: ") + key
            );
        }

        return value;
    }

    static std::string optionalEnv(const char* key, const char* defaultValue = "") {
        const char* value = std::getenv(key);
        return value ? std::string(value) : std::string(defaultValue);
    }

    static AppConfig loadOrThrow() {

        AppConfig cfg;

        cfg.repoRoot = requireEnv("REPO_ROOT");
        cfg.appPath = cfg.repoRoot + "/" + requireEnv("APP_DIR_NAME");

        cfg.logPath =
            cfg.repoRoot + "/logs/" +
            requireEnv("MAIN_LOGS_FILE_NAME");

        cfg.testLogPath =
            cfg.repoRoot + "/logs-tests/" +
            requireEnv("MAIN_LOGS_FILE_NAME");

        std::string allowLogs = optionalEnv("ALLOW_OUTSIDE_LOG_PATHS", "false");
        cfg.allowOutsideLogs = (allowLogs == "true");

        return cfg;
    }
};

} // namespace EnvVar

#endif // ENV_VAR_FETCHER_HPP
