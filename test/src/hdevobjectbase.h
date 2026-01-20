/*
 * Created Author: Joseph Hui
 * Created Date: Sun 18 May 2022
 * Description:
 *      Provdes the base base of device objects.
*/

#ifndef __H_HUICPP_DEVOBJECT_BASE_H__
#define __H_HUICPP_DEVOBJECT_BASE_H__

#include "huicpp.h"
#include <atomic>

namespace HUICPP {

class HDevObjectBase {
protected:
    HDevObjectBase(HCSTRR strName = HSTR("HDevObjectBase")) noexcept;

    HDevObjectBase(const HDevObjectBase&) = delete;

    HDevObjectBase(HDevObjectBase&&) = delete;

    virtual ~ HDevObjectBase() noexcept ;

public:
    virtual HRET Init () = 0;

    // 默认的uninit函数.
    virtual void uninit() { }

private:
    HSTR m_strDevName;

};


class HSourceDevBase : public HDevObjectBase {
public:
    using base = HDevObjectBase;

protected:
    HSourceDevBase(HCSTRR strName = HSTR("HSourceDevBase")) noexcept;

    virtual ~ HSourceDevBase() noexcept;

public:
    virtual HRET InComing(HPTR ptr) = 0;

};


class HFifoDevBase : public HDevObjectBase {
public:
    using base = HDevObjectBase;

protected:
    HFifoDevBase(HCSTRR strName = HSTR("HFifoDevBase")) noexcept;

    virtual ~ HFifoDevBase() noexcept;

public:
    virtual HRET Push(HCUSZ data, HN len) = 0;

    virtual HRET Pop(HUSZ data, HNR len) = 0;
};


}

#endif // __H_HUICPP_DEVOBJECT_BASE_H__

