

#include "hiobuffer.h"


HIOBuffer::HIOBuffer(HN size) 
    : m_length(0), m_avalableLength(size), m_buffer(nullptr), m_size(size), 
    m_pos(0) {

    m_buffer = static_cast<HUSZ>(malloc(m_size));

}


HIOBuffer::~HIOBuffer() {

    HFREE(m_buffer);

}


void HIOBuffer::SetPos(HN newPos) noexcept{

    m_avalableLength -= newPos;
    m_pos += newPos;

}


void HIOBuffer::Reset() noexcept {
    
    m_pos = 0; 
    m_avalableLength = m_size; 

    memset(m_buffer, 0, m_size);

}


void HIOBuffer::Append(HCSTRR str) noexcept {

    if (HTO_INT(str.length()) + m_pos > m_avalableLength) {
        return ;
    }
    
    memmove(m_buffer + m_pos, str.c_str(), str.length());

    m_avalableLength -= str.length();
    m_pos += str.length();


}