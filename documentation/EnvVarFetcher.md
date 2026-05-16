# Utilities

# EnvVarFetcher

A small utility class for loading application configuration from environment variables and building a structured `AppConfig` object.

## Overview

`EnvVarFetcher` provides helper functions for reading environment variables safely and consistently. It supports both required and optional variables and constructs an `AppConfig` instance used by the application.

The class is located in the `EnvVar` namespace and works closely with `AppCfg::AppConfig`.

## Features

- Safe retrieval of environment variables
- Clear separation between required and optional variables
- Automatic configuration assembly (`AppConfig`)
- Throws exceptions when required variables are missing

## API

### `requireEnv(const char* key)`

Retrieves a required environment variable.

- Throws `std::runtime_error` if the variable is not set
- Returns the value as `std::string`

```cpp
std::string repoRoot = EnvVar::EnvVarFetcher::requireEnv("REPO_ROOT");
```

## `optionalEnv(const char* key, const char* defaultValue = "")`

Retrieves an optional environment variable.

- Returns the variable value if it exists  
- Returns `defaultValue` otherwise  

```cpp
std::string mode = EnvVar::EnvVarFetcher::optionalEnv("MODE", "debug");
```

## `loadOrThrow()`

Builds and returns a fully initialized `AppConfig` object.

It expects the following environment variables:

- `REPO_ROOT`
- `APP_DIR_NAME`
- `MAIN_LOGS_FILE_NAME`
- `ALLOW_OUTSIDE_LOG_PATHS` (optional, defaults to `"false"`)

---

## Example configuration logic

- `repoRoot` → `REPO_ROOT`
- `appPath` → `REPO_ROOT + "/" + APP_DIR_NAME`
- `logPath` → `REPO_ROOT + "/logs/" + MAIN_LOGS_FILE_NAME`
- `testLogPath` → `REPO_ROOT + "/logs-tests/" + MAIN_LOGS_FILE_NAME`
- `allowOutsideLogs` → parsed from `ALLOW_OUTSIDE_LOG_PATHS`

```cpp
AppCfg::AppConfig cfg = EnvVar::EnvVarFetcher::loadOrThrow();
```

## Additional information

- Missing required environment variables will immediately throw an exception
- Boolean configuration (ALLOW_OUTSIDE_LOG_PATHS) is interpreted as "true" or "false"
- Designed for early application initialization
