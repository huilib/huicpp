

#include "hdbres.h"
#include <hlog.h>
#include <algorithm>

namespace HUICPP {

namespace HUIDB {

HDBRes::HDBRes() noexcept
    : m_header(), m_data() {

}

void HDBRes::Clear() noexcept {
    m_header.clear();
    m_data.clear();
}


HCSTRR HDBRes::GetValue(HCSTRR strKey, HUN _row) const {
    
    HASSERT_MSG_THROW(_row < m_data.size(), "HDBRes::GetValue row number overflow", INVL_PARA);

    header_t::const_iterator fit = std::find(m_header.cbegin(), m_header.cend(), strKey);
    HASSERT_MSG_THROW(fit != m_header.cend(), "HDBRes::GetValue key column is not found", INVL_PARA);

    header_t::difference_type j = std::distance(m_header.cbegin(), fit);

    const row_t& _r = m_data[_row];

    return _r[j];
}


HCSTRR HDBRes::GetValue (HUN i, HUN j) const {

    HASSERT_MSG_THROW(i < m_data.size(), "HDBRes::GetValue row number overflow", INVL_PARA);

    HASSERT_MSG_THROW(j < m_header.size(), "HDBRes::GetValue column number overflow", INVL_PARA);

    const row_t& _r = m_data[i];
    HASSERT_MSG_THROW(_r.size() <= j, "HDBRes row source is invalid", SRC_ERR);
    return _r[j];

}


} // HUIDB

} // HUICPP
