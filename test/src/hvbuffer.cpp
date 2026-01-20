

#include "hvbuffer.h"


HVBuffer::HVBuffer(HN size) 
    : m_length(0), m_avalableLength(size), m_buffer(nullptr), m_size(size), 
    m_read_pos(0), m_write_pos(0) {

    m_buffer = static_cast<HUSZ>(malloc(m_size));

}


HVBuffer::~HVBuffer() {

    HFREE(m_buffer);

}