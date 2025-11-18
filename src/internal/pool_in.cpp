

#include "pool_in.h"


namespace HUICPP {

namespace hc_internal {

Pool::Pool () noexcept
    : m_nChildCount (0) {  }


Pool::Pool (HN nChildNum) noexcept
    : m_nChildCount (nChildNum) {  }


Pool::~Pool() { }


}

}