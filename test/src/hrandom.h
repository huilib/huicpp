/*
* Created: Joseph Hui Wed 29 Sep 2021
* Description:
    Wrapper for message queue.
*/

#ifndef __H_HUICPP_RANDOM_H__
#define __H_HUICPP_RANDOM_H__

#include "huicpp.h"
#include <random>

namespace HUICPP {

class HRandom {
public:
    typedef std::random_device::result_type rdr_type;
    typedef std::mt19937_64::result_type mtr_type;
    typedef std::uniform_int_distribution<>::result_type uidr_type;

    static rdr_type Rd_random ();

    static mtr_type MT_random ();

    static uidr_type Dist_random (HN a, HN b);

    static HUN Simple_random (HUN n = 0);


private:
    static std::random_device rd;

    static std::mt19937_64 mt;

};

}

#endif //__H_HUICPP_RANDOM_H__
