

#ifndef __H_HUICPP_REFBUFFER_H__
#define __H_HUICPP_REFBUFFER_H__

#include "huicpp.h"

namespace HUICPP {

class HRefBuffer {
public:
    HRefBuffer() noexcept;

    HRefBuffer(HCPTR ptr, HN length) noexcept;

    HRefBuffer(const HRefBuffer& refbuffer) noexcept;

    ~HRefBuffer() noexcept;

public:
    void Reset() noexcept;

    void Assign(HCPTR data, HN len) noexcept;

public:
    HCPTR Data() const noexcept { return m_ptr; }
    HN GetLength() const noexcept { return m_length; }

private:
    HCPTR m_ptr;
    HN m_length;
};


class HVRefBuffer{
public:
    HVRefBuffer() noexcept;
    HVRefBuffer(const HVRefBuffer& refbuffer) noexcept;
    ~HVRefBuffer() noexcept;

public:
    void Reset() noexcept;

    void Add(HCPTR data, HN len) noexcept;

    HN Count() const noexcept;

    HCPTR Data(HN) const noexcept;
    HN GetLength(HN) const noexcept;

private:
    std::vector<HRefBuffer> m_buffers;
};


}


#endif // __H_HUICPP_REFBUFFER_H__