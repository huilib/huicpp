

#include "houtputpacketbuffer.h"

#include <cstdlib>
#include <memory.h>
#include <arpa/inet.h>
#include <cstdio>

namespace HUICPP {

HOutputPacketBuffer::HOutputPacketBuffer(HUN length)
    : m_length(length), 
    m_current_pos(0), m_buf(nullptr) {

    m_buf = static_cast<HUSZ>(malloc(m_length));
    memset(m_buf, 0, m_length);

    Reset();

}


HOutputPacketBuffer::~HOutputPacketBuffer() {

    free(m_buf);

}


void HOutputPacketBuffer::Enqueue(HCUSZ data, HUN len) {

    memmove(CurPos(), data, len);

    increPos(len);

}


void HOutputPacketBuffer::EnqueueWord(dword_t word) {

    dword_t val = htonl(word);

    Enqueue(reinterpret_cast<HCUSZ>(&val), sizeof(dword_t));

}


HOutputPacketBuffer::dword_t HOutputPacketBuffer::ExtractWord(size_type position) {

    dword_t word;
    memmove((HUSZ)&word, &m_buf[position], 4);
    return ntohl(word);

}


void HOutputPacketBuffer::InsertWord(dword_t word, size_type toPosition) {

    dword_t nWord = htonl(word);

    memmove(&m_buf[toPosition], (HCUSZ)&nWord, 4);

}


HIOOutputBuffer::HIOOutputBuffer() noexcept
    : m_header(HEADER_LENGTH), m_index(1), m_vec() {

    Reset();

}


HIOOutputBuffer::~HIOOutputBuffer() noexcept{

}


bool HIOOutputBuffer::AddIo(HCPTR ptr, size_type length) {

    if (m_index >= IO_COUNT)
        return false;

    m_vec[m_index].iov_base = const_cast<HPTR>(ptr);
    m_vec[m_index].iov_len = length;
    ++ m_index;

    return true;

}


HSocket::size_type HIOOutputBuffer::WriteIo(const HSocket& sock) {

    m_vec[0].iov_base = reinterpret_cast<void*>(const_cast<unsigned char*>(m_header.Data()));
    m_vec[0].iov_len = m_header.DataLength();

    return sock.WriteV(m_vec, m_index);

}


HIOOutputBuffer::size_type HIOOutputBuffer::GetTotalLength() const {

    return m_header.DataLength() + GetDataLength();

}


HIOOutputBuffer::size_type HIOOutputBuffer::GetDataLength() const {

    size_type res = 0;

    for (size_type i = 0; i < m_index; ++i) {

        res += m_vec[i].iov_len;
        
    }

    return res;

}


void HIOOutputBuffer::Reset() {

    m_index = 1;
    m_header.Reset();
    memset(m_vec, 0, sizeof(struct iovec) * IO_COUNT);

}

}
