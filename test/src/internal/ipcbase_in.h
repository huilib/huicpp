/*
* Created: Joseph Hui Wed 29 Sep 2021
* Description:
    Wrapper for IPC Base.
*/


#ifndef __H_HUICPP_IPC_BASE_H__
#define __H_HUICPP_IPC_BASE_H__

#include "base_traits.h"
#include "error_in.h"

#include <sys/ipc.h>

namespace HUICPP {

namespace hc_internal {

template<class _Ty>
struct ipc_traits : public base_traits<_Ty> {
    using base = base_traits<_Ty>;
    using value_type = typename base::value_type;
    using id_type = value_type;
    using key_type = value_type;
    using flag_type = value_type;
    using cmd_type = value_type;
    static constexpr const key_type DEF_KEY_VAL = -1;
    static constexpr const id_type DEF_ID_VAL = -1;
    static constexpr const flag_type DEF_FLAG_VAL = 0;    
};


template<class _Ty, class _Traits = ipc_traits<_Ty> > 
class ipc_basic {
public:
    using traits_type = _Traits;
    using size_type = typename traits_type::size_type;
    using id_type = typename traits_type::id_type;
    using key_type = typename traits_type::key_type;
    using flag_type = typename traits_type::flag_type;
    using cmd_type = typename traits_type::cmd_type;
    static constexpr const flag_type DEFAULT_IPC_FLAG = IPC_CREAT | 0600;

public:
    constexpr ipc_basic() noexcept
        : m_key(traits_type::DEF_KEY_VAL), 
            m_id(traits_type::DEF_ID_VAL),
            m_flag(traits_type::DEF_FLAG_VAL) {  }

    constexpr ipc_basic(key_type key, flag_type flag) noexcept
        : m_key(key), m_id(traits_type::DEF_ID_VAL),
        m_flag(flag){  }

    virtual ~ ipc_basic() noexcept {  }

public:
    virtual HRET Init () {

        RET_T create_ret = create();
        HASSERT_RETURN(create_ret != ERR_NO(SYS_FAILED), SYS_FAILED);

        IF_NOTOK(create_ret) {
            HNOTOK_RETURN(open());
        }

        HRETURN_OK;

    }

    virtual HRET Remove() = 0;

protected:
    virtual HRET create () = 0;

    virtual HRET open () = 0;

public:
    HALWAYS_INLINE key_type GetKey() const noexcept { return m_key; }

    HALWAYS_INLINE id_type GetId() const noexcept { return m_id; }

    HALWAYS_INLINE flag_type GetFlag () const noexcept { return m_flag; }

    HALWAYS_INLINE bool HasInited() const noexcept { return m_id != traits_type::DEF_ID_VAL; }

    HALWAYS_INLINE void SetInvalid() noexcept { m_id = traits_type::DEF_ID_VAL; }

    HALWAYS_INLINE void SetKey (key_type key) noexcept { m_key = key; }

    HALWAYS_INLINE void SetId (id_type id) noexcept { m_id = id; } 

    HALWAYS_INLINE void SetFlag (flag_type flag) noexcept { m_flag = flag; }

protected:
    key_type m_key;
    id_type m_id;
    flag_type m_flag;    
};


}

}


#endif // __H_HUICPP_IPC_BASE_H__

