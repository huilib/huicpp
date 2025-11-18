/*****************************************************************************
 *
 * FileName: hdbres.h
 *
 * Author: Joseph Hui, josephhui1009@gmail.com
 *
 * Create Date: Sun Jul 29 17:25 2018
 *
 * Brief:
 *       Wrapper the result from db.
 *
 *****************************************************************************/

#ifndef _H_HUICPP_DB_DBRES_H__
#define _H_HUICPP_DB_DBRES_H__

#include <huicpp.h>

namespace HUICPP {

namespace HUIDB {

using header_t = HVSTR;
using row_t = HVSTR;
using table_data_t = std::vector<row_t>;

class HDBRes {
public:
    HDBRes () noexcept;

public:
    void Clear() noexcept;

    void SetHeader(HCVSTRR header) noexcept { m_header = header; }

    void SetData (const table_data_t& data) { m_data = data; }

    void AddRow (HCVSTRR row) noexcept { m_data.emplace_back(row); }

    bool IsValid () const noexcept { return not m_header.empty() and not m_data.empty(); }

    const header_t& GetHeader () const noexcept { return m_header; }

    const table_data_t& GetData () const noexcept { return m_data; }

    HCSTRR GetValue(HCSTRR strKey, HUN _row = 0) const;

    HCSTRR GetValue (HUN i, HUN j) const; 

    HSIZE GetRowCount () const noexcept { return m_data.size(); }

    HSIZE GetColumnCount () const noexcept { return m_header.size(); }

private:
    header_t m_header;
    table_data_t m_data;

};



} // HUIDB

} // HUICPP


#endif //_H_HUICPP_DB_DBRES_H__
