/*
 * Created: Joseph Jun-17-2022
 * Description: 
  		Wrapper for huicpp buffer.
*/


#ifndef __H_HUICPP_SYNCBUFFER_H__
#define __H_HUICPP_SYNCBUFFER_H__

#include "huicpp.h"
#include "hmutex.h"
#include <queue>

namespace HUICPP {

class HSyncBuffer {
public:
    using data_t = HMEM;
    using datas_t = std::queue<data_t>;

public:
    HSyncBuffer();
    ~ HSyncBuffer();

public:
    void PushData(HCUSZ, HN);

    void PushData(HCMEMR);

    bool PopData(HUSZ, HNR) noexcept(false);

    bool PopData(HMEMR);

    void PopAllData(std::vector<HMEM>& datas);

    bool HasData() const;

private:
    mutable HConLock m_locker;
    datas_t m_datas;
};

}

#endif //__H_HUICPP_SYNCBUFFER_H__

