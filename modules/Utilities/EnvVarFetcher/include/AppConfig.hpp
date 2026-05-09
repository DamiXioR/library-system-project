#ifndef APP_CONFIG_HPP
#define APP_CONFIG_HPP

#include <string>

namespace AppCfg {

struct AppConfig {
    std::string repoRoot;
    std::string appPath;

    std::string logPath;
    std::string testLogPath;

    bool allowOutsideLogs;
};

} // namespace AppCfg

#endif // APP_CONFIG_HPP
