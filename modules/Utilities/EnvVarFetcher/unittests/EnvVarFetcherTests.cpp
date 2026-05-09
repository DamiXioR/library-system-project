#include <gtest/gtest.h>

#include "EnvVarFetcher.hpp"

using namespace ::testing;
using namespace EnvVar;

class EnvVarFetcherTests : public Test {};

TEST_F(EnvVarFetcherTests, nothingThrownWhenEnvVarsAreSourced) {
    ASSERT_NO_THROW(EnvVarFetcher::loadOrThrow());
}

TEST_F(EnvVarFetcherTests, runtimeErrorIsThrownWhenEnvVarsAreMissed) {
    const auto repoRoot {getenv("REPO_ROOT")};
    unsetenv("REPO_ROOT");
    ASSERT_THROW(EnvVarFetcher::loadOrThrow(), std::runtime_error);
    setenv("REPO_ROOT", repoRoot, 1);
}
