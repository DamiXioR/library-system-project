#include "Data.hpp"

Data& Data::GetInstance(){
    static Data instance;
    return instance;
}