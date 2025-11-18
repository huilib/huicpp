/*
 * Created Author: Joseph Hui. josephhui1009@gmail.com
 * Created Date: Web 11 May 2022.
 * Description: 
 *      Provides a thin abstraction connection base class.
 *  This abstraction base class provides servaral public 
 *  common APIs for operating db. 
 *
*/


#ifndef __H_HUICPP_DBCONNECTIONBASE_H__
#define __H_HUICPP_DBCONNECTIONBASE_H__

#include <huicpp.h>
#include "hdbres.h"

namespace HUICPP {

namespace HUIDB {

namespace huidb_internal {


class dbconnectionbase {
public:
    static constexpr const table_data_t::size_type MAX_MULTI_INSERT_COUNT = 256;

public:
    dbconnectionbase() noexcept;

    virtual ~ dbconnectionbase() noexcept;

public:
    virtual HRET Query (HCSTRR sql, HDBRes& res) = 0;

    virtual HRET Execute (HCSTRR sql) = 0;

    virtual HRET BeginTransaction () = 0;

    virtual HRET CommitTransaction () = 0;

    virtual HRET RollBack () = 0;

    HRET MultiExec (HCVSTRR sqlss);

    HRET MultiInsert (HCSTRR strTable, const HDBRes& res);

protected:
    virtual HRET multi_insert_impl(HCSTRR strTabName, 
        const header_t& table, const table_data_t& data);

};

}

}

}

#endif //__H_HUICPP_DBCONNECTIONBASE_H__

