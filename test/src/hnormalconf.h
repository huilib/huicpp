/*
 * Created: Joseph Mar-29-2021
 * Description: 
  		Wrapper for huicpp normal config. 
*/


#ifndef __H_HUICPP_NORMAL_CONFIG_H__
#define __H_HUICPP_NORMAL_CONFIG_H__

#include "hmstr.h"

namespace HUICPP {

class HNormalConfig {
public:
    using cfg_map_t = HMstr;

    static constexpr const HN MIN_CONTENT_LENGTH = 10;

public:
    HNormalConfig() noexcept { }

    virtual ~ HNormalConfig () noexcept { }

public: 
    /*
        load config from config-file.
    */
    HRET LoadConfig (HCSTRR strConfigFileName);

    void Clear();

    HSTR ToJson() const;

public:
    HCSTRR GetValue(HCSTRR strKey) const noexcept;

    HCSTRR GetValue(HCSTRR strKey, HCSTRR strDefVal) const noexcept;

    HN GetIntValue(HCSTRR strKey) const noexcept;

    HN GetIntValue(HCSTRR strKey, HN nDefVal) const noexcept;

    bool Exists(HCSTRR strKey) const;

    void SetValue (HCSTRR key, HCSTRR val) noexcept;

private:
    bool isNoteLine (HCSTRR l) const;

private:
    cfg_map_t m_conf;
};

}


#endif // __H_HUICPP_NORMAL_CONFIG_H__

