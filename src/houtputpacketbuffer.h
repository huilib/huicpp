



#ifndef __H_HUICPP_OUTPUTPACKETBUFFER_H__
#define __H_HUICPP_OUTPUTPACKETBUFFER_H__

#include <sys/types.h>
#include <sys/uio.h>
#include "huicpp.h"
#include "hsocket.h"


namespace HUICPP {

class HOutputPacketBuffer {
private:
    static constexpr HUN const MAX_BUFFER_LENGTH = 1536;
    using size_type = HUN;
    using dword_t = u_int32_t;

public:
    HOutputPacketBuffer(HUN length = MAX_BUFFER_LENGTH);

    ~ HOutputPacketBuffer();

public:
    void Enqueue(HCUSZ data, HUN len);

    void EnqueueWord(dword_t word);

    dword_t ExtractWord(size_type position);

    void InsertWord(dword_t word, size_type toPosition);

public:
    void Reset() noexcept { m_current_pos = 0; }

    HUSZ CurPos() noexcept { return  m_buf + m_current_pos; }

    HCUSZ CurPos() const noexcept { return  m_buf + m_current_pos; }

    HCUSZ Data() const noexcept { return m_buf; }

    HUN DataLength() const noexcept { return m_current_pos; }

private:
    void increPos(HUN n) noexcept { m_current_pos += n; }

private:
    const HUN m_length;
    HUN m_current_pos;
    HUSZ m_buf;
};

// implemente in template later.
class HIOOutputBuffer {
public:
    using size_type = HUN;

public:
    HIOOutputBuffer() noexcept;
    ~ HIOOutputBuffer() noexcept;

public:
    HOutputPacketBuffer& GetHeader() noexcept { return m_header; }

    bool AddIo(HCPTR ptr, size_type length);  

    HSocket::size_type WriteIo(const HSocket& sock);

    size_type GetTotalLength() const;

    size_type GetDataLength() const;

    void Reset();

private:
    static const constexpr size_type IO_COUNT = 10;
    static const constexpr size_type HEADER_LENGTH = 128;

private:
    HOutputPacketBuffer m_header;
    size_type m_index;
    struct iovec m_vec[IO_COUNT];
    
};

}

#endif // __H_HUICPP_OUTPUTPACKETBUFFER_H__