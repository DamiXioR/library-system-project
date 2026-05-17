#ifndef MOCK_IUUID_GENERATOR_HPP
#define MOCK_IUUID_GENERATOR_HPP

#include <gmock/gmock.h>

#include "IUuidGenerator.hpp"

class MockIUuidGenerator : public IUuidGenerator {
public:
    MOCK_METHOD((std::expected<std::string, UUID_ERROR_CODE>),
                generate,
                (),
                (override));
    MOCK_METHOD((std::size_t),
                getUuidsCount,
                (),
                (const override));
};

#endif // MOCK_IUUID_GENERATOR_HPP
