

#include "hsinglefastqueue.h"
#include <iostream>

using namespace HUICPP;


using fastqueue = HSingleFastQueue<int*>;

int main(int argc, const char* argv[]) {

    fastqueue myqueu(128);

    //std::cout <<  *my_singleton::Get() << std::endl;

    return 0;
}