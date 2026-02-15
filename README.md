# Instalation

## CMake
The easiest way to install CMake is to fetch the newest script from official website:
`wget https://github.com/Kitware/CMake/releases/download/v4.2.3/cmake-4.2.3-linux-x86_64.sh`

Verify official CMake website to check the newest one version of CMake:
`http://cmake.org/download/`

```bash
chmod +x cmake-4.2.3-linux-x86_64.sh
./cmake-4.2.3-linux-x86_64.sh
```

Verify if binaries are properly installed:
`ls cmake-4.2.3-linux-x86_64/bin` especially `ls cmake-4.2.3-linux-x86_64/bin/cmake`

Add CMake binaries to your environment variables:
`export PATH=$PATH:/home/${USER}/good_job_project/cmake-4.2.3-linux-x86_64/bin`
This is important if you want to run CMake using the command `cmake` instead of specifying the full path.

## GTest
Build GTest locally

```bash
git clone https://github.com/google/googletest.git -b v1.17.0
cd googletest
mkdir build
cd build
cmake .. -DBUILD_SHARED_LIBS=ON
cmake --build . --config Release
```

For local installation:
```bash
cmake --install . --prefix /home/user/libs/googletest/build-install
```

Now you have everything what is needed for testing.
Shared libraries:
```bash
libgtest.so
libgtest_main.so
libgmock.so
libgmock_main.so
```
Headers:
```bash
googletest/include/gtest/...
googlemock/include/gmock/...
```