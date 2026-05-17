#ifndef IUUID_GENERATOR_HPP
#define IUUID_GENERATOR_HPP

#include <string>
#include <unordered_set>
#include <expected>

enum class UUID_ERROR_CODE {
    DUPLICATE_LIMIT_REACHED,
    INVALID_UUID
};

class IUuidGenerator {
public:
    virtual ~IUuidGenerator() = default;

    [[nodiscard]]
    virtual auto generate() -> std::expected<std::string, UUID_ERROR_CODE> = 0;
    virtual auto getUuidsCount() const -> std::size_t = 0;
};

#endif // IUUID_GENERATOR_HPP
