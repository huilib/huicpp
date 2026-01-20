

#ifndef __H_IO_BUFFER_H__
#define __H_IO_BUFFER_H__

#include "huicpp.h"

using namespace HUICPP;

class HIOBuffer {
public:
    static constexpr const HN IO_DEFAULT_SIZE = 1024 * 1024;  // 1M.

public:
    HIOBuffer(HN size = IO_DEFAULT_SIZE);

    ~ HIOBuffer();

public:
    HUSZ PosWrite() noexcept { return m_buffer + m_pos; }
    HN AvalableLength() const noexcept { return m_avalableLength; }
    void SetPos(HN newPos) noexcept;

    HCUSZ PopData() const noexcept { return m_buffer; }
    HN DataLength() const noexcept { return m_pos; }
    void Reset() noexcept;

    void Append(HCSTRR str) noexcept;

private:
    HN m_length;                        // length of the data
    HN m_avalableLength;
    HUSZ m_buffer;      
    const HN m_size;                    // capacity of the buffer.
    HN m_pos;    
};


#endif // __H_IO_BUFFER_H__