
#include "hrefbuffer.h"


namespace HUICPP {

HRefBuffer::HRefBuffer() noexcept
    : m_ptr(nullptr), m_length(0) {

}


HRefBuffer::HRefBuffer(HCPTR ptr, HN length) noexcept
    : m_ptr(ptr), m_length(length) {

}


HRefBuffer::HRefBuffer(const HRefBuffer& refbuffer) noexcept 
    : m_ptr(refbuffer.m_ptr), m_length(refbuffer.m_length) {

}


HRefBuffer::~HRefBuffer() {

}


void HRefBuffer::Reset() noexcept {

    m_ptr = nullptr;

    m_length = 0;

}


void HRefBuffer::Assign(HCPTR data, HN len) noexcept {

    m_ptr = data;

    m_length = len;

}


HVRefBuffer::HVRefBuffer() noexcept
    : m_buffers() {

}


HVRefBuffer::HVRefBuffer(const HVRefBuffer& other) noexcept
    : m_buffers(other.m_buffers) {

}


HVRefBuffer::~HVRefBuffer() noexcept{

}


void HVRefBuffer::Reset() noexcept {

    m_buffers.clear();

}


void HVRefBuffer::Add(HCPTR data, HN len) noexcept {

    m_buffers.push_back({data, len});

}


HN HVRefBuffer::Count() const noexcept {

    return HTO_INT(m_buffers.size());

}


HCPTR HVRefBuffer::Data(HN index) const noexcept {

    if (index >= Count()) {
        return nullptr;
    }

    return m_buffers[index].Data();

}
    
    
HN HVRefBuffer::GetLength(HN index) const noexcept {

    if (index >= Count()) {
        return -1;
    }

    return m_buffers[index].GetLength();

}


}