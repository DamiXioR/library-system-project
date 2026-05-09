#ifndef LOG_SINK_HELPERS_HPP
#define LOG_SINK_HELPERS_HPP

#include <string_view>
#include <string>

namespace LogSys {

struct TransparentHash {
    using is_transparent = void;

    size_t operator()(std::string_view sv) const noexcept {
        return std::hash<std::string_view>{}(sv);
    }

    size_t operator()(const std::string& s) const noexcept {
        return std::hash<std::string>{}(s);
    }

    size_t operator()(const char* cstr) const noexcept {
        return std::hash<std::string_view>{}(cstr);
    }
};

} // namespace LogSys

#endif // LOG_SINK_HELPERS_HPP
