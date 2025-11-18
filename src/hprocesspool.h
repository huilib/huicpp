
#ifndef __H_HUICPP_PROCESSPOOL_H__
#define __H_HUICPP_PROCESSPOOL_H__

#include "internal/pool_in.h"
#include "hsharememory.h"

namespace HUICPP {

namespace hc_internal_pool {

using pool_t = hc_internal::Pool;

class ProcessCntl {
public:
    using status_t = pool_t::ChildStatus;

    using status_val_t = HN;

    typedef struct {
        time_t startat;
        status_val_t status;
        time_t at;
        pid_t pid;
    } cntl_info;
    typedef cntl_info* cntl_info_ptr;

    static constexpr const HN DEFAULT_MEM_KEY = 28888;
    static constexpr const HN PARENT_DEFAULT_INDEX = -1;

public:
    ProcessCntl(HN nCount, HN nCacheSize, HN nKey = DEFAULT_MEM_KEY) noexcept;

    ProcessCntl(const ProcessCntl&) = delete;

    ProcessCntl(ProcessCntl&&) = delete;

    ~ ProcessCntl();

public:
    ProcessCntl& operator= (const ProcessCntl&) = delete;
    ProcessCntl& operator= (ProcessCntl&&) = delete;

public:
    HRET Init();

    RET_T Uninit (bool isSon = true);

    void ResetInfo(HN index);

    void SetStartInfo(HN index, pid_t pid);

    void TouchInfo (HN index);

    void SetIdleInfo(HN index);

    void UnuseInfo (HN index);    

public:
    time_t GetStartAt(HN index) const;

    time_t GetLastAccessTime (HN index) const;

    pid_t GetPid (HN index) const;

    status_t GetStatus (HN index) const;

    HCPTR GetCachePtr () const noexcept { return m_cache_ptr; }

    HPTR GetCachePtr() noexcept { return m_cache_ptr; }

private:
    void set_info_as_status(HN index, status_t status);

private:
    HN m_nCount;
    HN m_nCacheSize;
    HShareMemory m_mem;
    cntl_info_ptr m_pcntl_info;
    HPTR m_cache_ptr;
};

}


class HProcessPool : public hc_internal_pool::pool_t {
public:
    using base = hc_internal_pool::pool_t;
    using process_cntl_t = hc_internal_pool::ProcessCntl;
    using status_t = process_cntl_t::status_t;
    static constexpr const HN AFTER_SIGTERM_ST = 2;
    static constexpr const time_t MAX_HEAR_TIME = 30;        
    static constexpr const HN PARENT_DEFAULT_INDEX = process_cntl_t::PARENT_DEFAULT_INDEX;
    static constexpr const HN DEFAULT_CACHE_SIZE = 1024 * 1024 * 2; // 2M
public:
    HProcessPool(HN nChildCount, HN nCacheSize = DEFAULT_CACHE_SIZE, HN nKey = process_cntl_t::DEFAULT_MEM_KEY) noexcept;

    virtual ~ HProcessPool();

public:
    virtual HRET Init () override;

    void ManagePool ();

    HN GetProcessIndex() const noexcept { return m_index; }

    bool IsSon () const noexcept { return m_index != PARENT_DEFAULT_INDEX; }

    bool HasInited() const noexcept { return m_bInited; }

    HCPTR GetCachePtr () const noexcept { return m_cntl.GetCachePtr(); }

    HPTR GetCachePtr() noexcept { return m_cntl.GetCachePtr(); }

protected:
    virtual RET_T childProcessRun () = 0;

    status_t getStatus(HN index) const { return m_cntl.GetStatus(index); }

    void touchSon(HN index) { m_cntl.TouchInfo(index); }

    time_t getSonLastAccessTime(HN index) const { return m_cntl.GetLastAccessTime(index); }

    time_t getSonStartAt(HN index) const { return m_cntl.GetStartAt(index); }

    virtual void prepare_end_son(HN index);

private:
    void create_child (HN index);

    void kill_child(HN index);

    void parent_touch ();

    void son_touch ();

private:
    bool m_bInited;
    process_cntl_t m_cntl;
    HN m_index;
};


}


#endif //__H_HUICPP_PROCESSPOOL_H__
