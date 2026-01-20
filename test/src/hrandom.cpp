


#include "hrandom.h"
#include <sys/time.h>


namespace HUICPP {

HRandom::rdr_type HRandom::Rd_random () {

    return rd();

}


HRandom::mtr_type HRandom::MT_random () {

    return mt();

}


HRandom::uidr_type HRandom::Dist_random(HN a, HN b) {

    static std::uniform_int_distribution<> dis(a, b);
    return dis(mt);

}


HUN HRandom::Simple_random (HUN n) {

    struct timeval tv = {0,0};

    (void)gettimeofday(&tv, nullptr);

    (void)srand((unsigned int)tv.tv_usec);

    HUN res = rand();

    return n ? res % n : res;

}


std::random_device HRandom::rd;

std::mt19937_64 HRandom::mt(rd());

}
