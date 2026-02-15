#include <iostream>
#include <Data.hpp>

int main(int nArgs, char** args){
    std::cout << "START\n";
    Data& data = Data::GetInstance();
    

    std::cout << "STOP\n";
    return 0;
}
