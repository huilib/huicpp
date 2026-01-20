

#include "hsyncbuffer.h"

namespace HUICPP {

HSyncBuffer::HSyncBuffer()
    : m_locker(), 
    m_datas() {
}


HSyncBuffer::~ HSyncBuffer() {
}


void HSyncBuffer::PushData(HCUSZ cptr, HN len) {
    HMEM mem(cptr, cptr + len);
    PushData(mem);
}


void HSyncBuffer::PushData(HCMEMR mem) {
    {
        HSCOPE_MUTEX(m_locker);
        m_datas.push(std::move(mem));
    }
    HNOTOK_THROW(m_locker.Signal());
}


bool HSyncBuffer::PopData(HUSZ ptr, HNR len) noexcept(false) {

    HMEM mem;
    if (not PopData(mem)) {
        return false;
    }
    HASSERT_THROW(HTO_INT(mem.size()) <= len, INVL_PARA);
    HASSERT_THROW(ptr != nullptr, INVL_PARA);

    memcpy(ptr, &(mem[0]), mem.size());
    len = HTO_INT(mem.size());
    return true;
}


bool HSyncBuffer::PopData(HMEMR mem) {
    HSCOPE_MUTEX(m_locker);

    while(m_datas.empty()) {
        HNOTOK_THROW(m_locker.Wait());
    }

    mem = std::move(m_datas.front());
    m_datas.pop();
    return true;
}


void HSyncBuffer::PopAllData(std::vector<HMEM>& datas) {

    HSCOPE_MUTEX(m_locker);

    while(m_datas.empty()) {
        HNOTOK_THROW(m_locker.Wait());
    }

    while (not m_datas.empty()) {
        datas.emplace_back(std::move(m_datas.front()));
        m_datas.pop();
    }

}


bool HSyncBuffer::HasData() const {
    HSCOPE_MUTEX(m_locker);
    return m_datas.empty();
}


}
