

#include "hmmap.h"
#include "htodoholder.h"
#include <fcntl.h>
#include <unistd.h>
#include <sys/mman.h>

namespace HUICPP{

HMmap::HMmap(HSIZE size, HLN off)
    : m_strFileName(), 
    m_len(size), m_off(off),
    m_prot(DEFAULT_PROT), m_flags(DEFAULT_FLAGS),
    m_named(true),
    m_ptr(nullptr) {

}


HMmap::HMmap(HCSTRR strFileName, HSIZE size, HLN off)
    : m_strFileName(strFileName), 
    m_len(size), m_off(off),
    m_prot(DEFAULT_PROT), m_flags(DEFAULT_FLAGS),
    m_named(true),
    m_ptr(nullptr) {

}


HMmap::~ HMmap() {

    UnInit();

}


HRET HMmap::Init() {
    
    FD_T fd = -1;
    m_named = false;

    // 1. open file by filename.
    if (not m_strFileName.empty()) {
        fd = open(m_strFileName.c_str(), O_RDWR);
        HASSERT_RETURN(fd > 0, INVL_PARA);
        m_named = true;
    }

    // close fd.
    HSCOPE_EXIT {
        close(fd);
    };

    // 2. check file length.
    HOFF flen = lseek(fd, m_off, SEEK_END);
    HASSERT_RETURN(flen >= static_cast<HOFF>(m_len), INVL_PARA);

    // 3. map file to virtual memory.
    m_ptr = mmap(nullptr, m_len, m_prot, m_flags, fd, m_off);
    HASSERT_RETURN(m_ptr != MAP_FAILED, SYS_FAILED);

    HRETURN_OK;

}


void HMmap::UnInit() {

    if (m_ptr != nullptr or m_ptr != MAP_FAILED) {

        munmap(m_ptr, m_len);
        m_ptr = nullptr;

    }

}

}
