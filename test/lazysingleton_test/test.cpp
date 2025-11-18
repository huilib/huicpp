

#include "hlazysingleton.h"
#include <iostream>

using namespace HUICPP;

struct mytag{};

using my_singleton = HLazySingleton<int>;

int main(int argc, const char* argv[]) {

    std::cout <<  *my_singleton::Get() << std::endl;

    return 0;
}