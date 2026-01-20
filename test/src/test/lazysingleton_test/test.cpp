

#include "hlazysingleton.h"
#include <iostream>

using namespace HUICPP;

struct mytag{};

using my_singleton = HLazySingleton<int>;

using singleton_my = HLazySingleton<int,mytag>;

int main(int argc, const char* argv[]) {

    std::cout <<  *my_singleton::Get() << std::endl;

    *my_singleton::Get() = 200;
    *singleton_my::Get() = 300;

    std::cout <<  *my_singleton::Get() << std::endl;
    std::cout <<  *singleton_my::Get() << std::endl;

    return 0;
}