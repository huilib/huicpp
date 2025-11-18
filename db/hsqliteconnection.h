/*
 * Created Author: Joseph Hui. josephhui1009@gmail.com
 * Created Date: Wed 11 May 2022
 * Description:
 *      Wrapper sqlite3 db connection.
*/


#ifndef __H_HUICPP_SQLITE_CONNECTION_H__
#define __H_HUICPP_SQLITE_CONNECTION_H__

#include "dbconnectionbase.h"
#include "sqlitedrive.h"


namespace HUICPP {

namespace HUIDB {

class HSqliteConnection : public huidb_internal::dbconnectionbase,
    public huidb_internal::sqlite_drive {
public:
    using connection_base = huidb_internal::dbconnectionbase;
    using drive_base = huidb_internal::sqlite_drive;

public:
    HSqliteConnection ();

    ~ HSqliteConnection();

public:
    HRET Query (HCSTRR sql, HDBRes& res);

    HRET Execute (HCSTRR sql);

    HRET BeginTransaction ();

    HRET CommitTransaction ();

    HRET RollBack ();

};

}

}



#endif //__HCONNECTION_H__
