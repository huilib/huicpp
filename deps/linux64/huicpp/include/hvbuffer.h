

#ifndef __H_V_BUFFER_H__
#define __H_V_BUFFER_H__

#include "huicpp.h"

using namespace HUICPP;

class HVBuffer {
public:
    static constexpr const HN V_DEFAULT_SIZE = 1024 * 1024;  // 1M.
public:
    HVBuffer(HN size = V_DEFAULT_SIZE);

    ~ HVBuffer();

public:

private:
    HN m_length;                   // length of the data
    HN m_avalableLength;
    HUSZ m_buffer;      
    const HN m_size;               // capacity of the buffer.
    HN m_read_pos;
    HN m_write_pos;    
};


#endif // __H_V_BUFFER_H__