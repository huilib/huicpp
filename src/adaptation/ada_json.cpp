

#include "ada_json.h"
#include "../hfilestr.h"

namespace HUICPP {

namespace adapt {

HRET jsonconfig_impl::load_json_file(HCSTRR strFileName) {

    HASSERT_RETURN(not strFileName.empty(), INVL_PARA);

    HSTR strContent;
    HNOTOK_RETURN(HFileStr(strFileName).ReadToString(strContent));

    HASSERT_RETURN(not strContent.empty(), INVL_PARA);
    
    try {
        m_json = json_t::parse(strContent);
    } catch (...) {
        HRETURN(ERR_STATUS);
    }

    HASSERT_RETURN(not m_json.is_discarded(), ERR_STATUS);

    HRETURN_OK;

}


HCSTRR jsonconfig_impl::get_value(HCSTRR key) const {

    HASSERT_THROW(m_json.contains(key), NOT_EXISTS);

    return m_json[key].get_ref<HCSTRR>();

}


HCSTRR jsonconfig_impl::get_value(HCSTRR key, HCSTRR defVal) const noexcept{

    if (not m_json.contains(key)) {
        return defVal;
    }

    return m_json[key].get_ref<HCSTRR>();

}


HN jsonconfig_impl::get_int_value(HCSTRR key) const {

    HASSERT_THROW(m_json.contains(key), NOT_EXISTS);

    HASSERT_THROW(m_json[key].is_number_integer(), SRC_ERR);

    return m_json[key].get<HN>();

}


HN jsonconfig_impl::get_int_value(HCSTRR key, HN defVal) const noexcept{

    if (not m_json.contains(key)) {
        return defVal;
    }

    if (not m_json[key].is_number_integer()) {
        return defVal;
    }

    return m_json[key].get<HN>();

}


bool jsonconfig_impl::exists(HCSTRR key) const {

    return m_json.contains(key);

}

void jsonconfig_impl::clear() {

    m_json.clear();

}


void jsonconfig_impl::set_value(HCSTRR key, HCSTRR val) noexcept {

    m_json[key] = val;

}


}

}