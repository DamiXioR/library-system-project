#ifndef DATA_HPP
#define DATA_HPP

#include <string>

class Data {
private:
    Data() = default;
    ~Data() = default;

    Data(const Data&) = delete;
    Data& operator=(const Data&) = delete;

public:
    std::string welcome {"I am singleton"};
    static Data& GetInstance();
};

#endif // DATA_HPP