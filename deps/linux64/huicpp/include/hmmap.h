/*
* Created: Joseph Hui Fri 06 May 2022
* Description:
    Wrapper for mmap shared memory.
*/

#ifndef __H_HUICPP_MMAP_H__
#define __H_HUICPP_MMAP_H__

#include "huicpp.h"

namespace HUICPP {


class HMmap {
public:
    static constexpr const HSIZE DEFAULT_SIZE = 128 * 4096; // 128 * 4K;
    static constexpr const SYS_T DEFAULT_PROT = 0;
    static constexpr const SYS_T DEFAULT_FLAGS = 0;
    static constexpr const HOFF DEFAULT_OFF = 0;

    HMmap(HSIZE size = DEFAULT_SIZE, HOFF off = DEFAULT_OFF);

    HMmap(HCSTRR strFileName, HSIZE size = DEFAULT_SIZE, HOFF off = DEFAULT_OFF);

    ~ HMmap();

public:
    HRET Init();

    void UnInit();

    bool IsNonNamed() const { return not m_named; }

    HPTR GetPtr() { return m_ptr; }

    HCPTR GetPtr() const { return m_ptr; }

private:
    HSTR m_strFileName;
    HSIZE m_len;
    HOFF m_off;
    SYS_T m_prot;
    SYS_T m_flags;
    bool m_named;
    HPTR m_ptr;
};



}

#endif //__H_HUICPP_MMAP_H__

