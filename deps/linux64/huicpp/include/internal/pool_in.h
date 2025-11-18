/*
 * Created: Joseph Hui Mon Nov 08 2019
 * Description: 
  		Wrapper for abstract pool.
*/


#ifndef __H_HUICPP_INTERNAL_POOL_IN_H__
#define __H_HUICPP_INTERNAL_POOL_IN_H__

#include "basictypes_in.h"
#include "commonmacro_in.h"

namespace HUICPP {

namespace hc_internal {


class Pool {
public:
    enum class ChildStatus {
        CS_UNUSED,
        CS_IDLE,
        CS_BUSY,
        CS_UNKNOWN
    };

public:
    Pool () noexcept;

    explicit Pool (HN nChildNum) noexcept;

    virtual ~ Pool ();

public:
    virtual HRET Init () = 0;

public:
    virtual void SetChildCount(HN nChildCount) { m_nChildCount = nChildCount; }

    HN GetChildCount () const { return m_nChildCount; }

private:
    HN m_nChildCount;

};

}

}

#endif // __H_HUICPP_INTERNAL_POOL_H__
