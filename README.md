# Fetch the repository

Clone the repository with hooks: `git clone ...`

For this instruction, let’s assume you keep the repository in this directory:
`cd /home/${USER}/workspace/good_job_project`

# Installing External Dependencies
good_job_project uses external dependencies like CMake, GTest and GMock framework.

## CMake
The easiest way to install CMake is to fetch the newest script from the official website.
Check the latest version here: `http://cmake.org/download/`.

First go to your root repository path: `cd ~/workspace/good_job_project`

As of today, the current version is v.4.2.3. To get the script use the following command:
`wget https://github.com/Kitware/CMake/releases/download/v4.2.3/cmake-4.2.3-linux-x86_64.sh`

Once downloaded, add execution permissions to the script and run it from the root repository path.
```bash
chmod +x cmake-4.2.3-linux-x86_64.sh
./cmake-4.2.3-linux-x86_64.sh
```

Verify if binaries are correctly installed:
`ls cmake-4.2.3-linux-x86_64/bin` especially `ls cmake-4.2.3-linux-x86_64/bin/cmake`

Add CMake binaries to your environment variables:
`export PATH=$PATH:/home/${USER}/good_job_project/cmake-4.2.3-linux-x86_64/bin`
This is important if you want to run CMake using the command `cmake` instead of specifying the full path.

## GTest and GMock [Section should be updated due to change an approach]
Build googletest framework locally. The project uses GTest and GMock shared libraries.

```bash
git clone https://github.com/google/googletest.git -b v1.17.0
```

Alternatively, for a local installation:
```bash
cmake --install . --prefix /home/user/libs/googletest/build-install
```

Now you have everything what is needed for testing.
GTest and GMock shared libraries:
```bash
libgtest.so
libgtest_main.so
libgmock.so
libgmock_main.so
```
GTest and GMock headers:
```bash
googletest/include/gtest/...
googlemock/include/gmock/...
```

# Build program with CMake
Our build system allows the user to build the entire program with unit tests or only a specific module.
This approach was chosen because orchestration is always beneficial:
it helps keep the code clean and responsibilities well separated.

## Build everything
To build the entire program with unit tests use the following commands:
```bash
cd ~/workspace/good_job_project/
cmake -S . -B build
cmake --build build
ctest --test-dir build --verbose
```
or alternatively and quickly:
```bash
cd ~/workspace/good_job_project/build
cmake .. && make && ctest --verbose
```

## Build only specyfied modules
### Configure the entire program
Configure with the same command like mentioned above:
```bash
cmake -S . -B build
```

Building a specific module or running unit tests for that module can be done using the following commands:

To build the module shared library use:
```bash
cmake --build build --target Repository
```

To build the unit tests for the specific module use:
```bash
cmake --build build --target RepositoryTests
```

To run the tests for the specific module use:
```bash
ctest --test-dir build -R RepositoryTests --verbose
```

If a unit test name is unique among all unit tests, you can use just the first letter of the test name, or the first few letters until they distinguish the test from the others. For example, for __DataModuleTest__ and __DataSuperModuleTest__, you can use __DataM__ and __DataS__.

### Exclude all tests or specyfic tests from configuration
You can exclude all unit tests from configuration by using a dedicated flag:
```bash
cmake -S . -B build -DBUILD_TESTING=OFF
```

If you do not need to configure specific unit tests, you can exclude them from the build configuration by using a specific flag:
```bash
cmake -S . -B build -DBUILD_EXAMPLEMODULE_TESTS=OFF
```
You can also specify multiple flags in the same command.

## Debugging and Dynamic Analysis

For debugging and dynamic analysis the project should be built **without compiler optimizations and with debug symbols enabled**.
If the project is compiled with optimizations (e.g. Release mode), debuggers may behave unpredictably and tools such as Valgrind may not report the **exact source line where an error occurs**.

Therefore the project should be configured in **Debug mode**, which enables debug flags (e.g. `-g`) and disables aggressive optimizations.

### Build configuration

Configure the project with CMake:

```bash
cd ~/workspace/good_job_project/
cmake -DCMAKE_BUILD_TYPE=Debug -S . -B build
```

Build the project:

```bash
cmake --build build
```

### Running memory checks

Run tests with Valgrind through CTest:

```bash
ctest --test-dir build -T memcheck
```
