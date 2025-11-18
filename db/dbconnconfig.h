/*
 * Created Author: Joseph Hui. josephhui1009@gmail.com
 * Created Date: Mon 23 May 2022.
 * Description: 
 *      Provides db connection config map. 
 *
*/


#ifndef __H_HUICPP_DBCONNCONFIG_H__
#define __H_HUICPP_DBCONNCONFIG_H__

#include "dbconnectionbase.h"
#include "hsingleton.h"

namespace HUICPP {

namespace HUIDB {

class DbConnConfig {
public:
    using conn_ptr = huidb_internal::dbconnectionbase*;
    using conns_t = std::vector<conn_ptr>;
    using db_conns_map = std::map<HSTR, conns_t>;

    static constexpr const HN min_db_count = 1;

public:
    DbConnConfig() noexcept;

    DbConnConfig(const DbConnConfig&) = delete;

    DbConnConfig(DbConnConfig&&) = delete;

    ~ DbConnConfig();

public:
    HRET InitFromConfigFile(HCSTRR strFileName, HUN conn_count = min_db_count);

    void AddConn (HCSTRR str, conn_ptr ptr);

    conn_ptr GetConn (HCSTRR strName, HUN index);

    HRET Query (HCSTRR strName, HUN index, HCSTRR sql, HDBRes& db_res);

    HRET Exec (HCSTRR strName, HUN index, HCSTRR sql);

    HRET Exec (HCSTRR strName, HUN index, HCVSTRR sqlss);

private:
    db_conns_map m_db_conns; 
};

using db_config = HSingleton<DbConnConfig>;

}

}



#endif // __H_HUICPP_DBCONNCONFIG_H__

