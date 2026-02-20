#ifndef UUID_GENERATOR_HPP
#define UUID_GENERATOR_HPP

#include <random>
#include <sstream>
#include <iomanip>
#include <cstdint>
#include <array>
#include <string>
#include <string_view>
#include <unordered_set>
#include <expected>

enum class UUID_ERROR_CODE {
    DUPLICATE_LIMIT_REACHED,
    INVALID_UUID
};

template <class UuidForType>
class UuidGenerator {
public:
    using uuidUnorderedSet = std::unordered_set<std::string>;

    [[nodiscard]]
    static auto createUuid() -> std::expected<std::string, UUID_ERROR_CODE> {
        bool successfullyInserted {false};
        uint16_t remainingAttempts  {10};
        std::string createdUuid {};

        do {
            auto uuid = generateUUID();
            auto [itToInsertedElement, isSuccess] = uuids.emplace(std::string(uuid));
            if (isSuccess){
                successfullyInserted = isSuccess;
                createdUuid = *itToInsertedElement;
            }
            --remainingAttempts ;
        } while(!successfullyInserted && remainingAttempts > 0);

        if(!successfullyInserted){
            return std::unexpected{UUID_ERROR_CODE::DUPLICATE_LIMIT_REACHED};
        }
        return createdUuid;
    }

    static auto getUuids() -> uuidUnorderedSet const {
        return uuids;
    }

private:
    static constexpr uint16_t MULTIPLICATION_OF_32_BITS {4};
    static constexpr uint32_t CLEAR_VERSION_BITS_MASK {0xFFFF0FFF};
    static constexpr uint32_t VERSION_4_BITS {0x00004000};
    static constexpr uint32_t CLEAR_VARIANT_BITS_MASK {0x3FFFFFFF};
    static constexpr uint32_t RFC4122_VARIANT_BITS {0x80000000};

    using uuidBuffor = std::array<uint32_t, MULTIPLICATION_OF_32_BITS>;

    static auto toStringRFC4122(uuidBuffor& data) -> std::string {
        std::stringstream ss;

        ss << std::hex << std::setfill('0');
        ss << std::setw(8) << data[0] << "-";
        ss << std::setw(4) << ((data[1] >> 16) & 0xFFFF) << "-";
        ss << std::setw(4) << (data[1] & 0xFFFF) << "-";
        ss << std::setw(4) << ((data[2] >> 16) & 0xFFFF) << "-";
        ss << std::setw(4) << (data[2] & 0xFFFF);
        ss << std::setw(8) << data[3];

        return ss.str();
    }

    static auto generateUUID() -> std::string {
        static thread_local std::mt19937 gen(std::random_device{}());
        std::uniform_int_distribution<uint32_t> dist(0, 0xFFFFFFFF);

        uuidBuffor data;
        for (int part {0}; part < MULTIPLICATION_OF_32_BITS; ++part) {
            data[part] = dist(gen);
        }

        data[1] = (data[1] & CLEAR_VERSION_BITS_MASK) | VERSION_4_BITS;
        data[2] = (data[2] & CLEAR_VARIANT_BITS_MASK) | RFC4122_VARIANT_BITS;

        return toStringRFC4122(data);
    }

    inline static uuidUnorderedSet uuids;
};

#endif // UUID_GENERATOR_HPP
