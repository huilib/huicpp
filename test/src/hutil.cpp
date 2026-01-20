


#include "hutil.h"

#include <unistd.h>
#include <time.h>


namespace HUICPP {


    HN HSleep(HN sec) {

        return HTO_INT(sleep(sec));

    }


    HN HSleep(HN sec, HN nMSec) {

        return usleep(sec * 1000000 + nMSec * 1000);

    }


    HN HSleep(HN sec, HN nMSec, HN nUSec) {

        return usleep(sec * 1000000 + nMSec * 1000 + nUSec);

    }


    HN HThreadSleep(HN sec, HN nMSec, HN nUSec) {

        struct timespec _ts{sec, nMSec * 1000 + nUSec};
        return nanosleep(&_ts, nullptr);

    }

    
    HSIZE Ceil2Pow2(HSIZE x) {        
        // From http://graphics.stanford.edu/~seander/bithacks.html#RoundUpPowerOf2
		--x;
		x |= x >> 1;
		x |= x >> 2;
		x |= x >> 4;
		for (HSIZE i = 1; i < sizeof(HSIZE); i <<= 1) {
			x |= x >> (i << 3);
		}
		++x;
		return x;
    }


}
