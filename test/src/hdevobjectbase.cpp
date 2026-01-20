

#include "hdevobjectbase.h"


namespace HUICPP {


HDevObjectBase::HDevObjectBase(HCSTRR strName) noexcept
    : m_strDevName(strName) { 

}


HDevObjectBase::~ HDevObjectBase() noexcept { }


HSourceDevBase::HSourceDevBase(HCSTRR strName) noexcept
    : base(strName) {  }


HSourceDevBase::~ HSourceDevBase() noexcept { }


HFifoDevBase::HFifoDevBase(HCSTRR strName) noexcept
    : base(strName) {  }


HFifoDevBase::~ HFifoDevBase() noexcept { }


}
