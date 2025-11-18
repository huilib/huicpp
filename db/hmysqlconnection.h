/*****************************************************************************
 *
 * FileName: simpledb.h
 *
 * Author: Tom Hui, tomhui1009@yahoo.com, 8613760232170
 *
 * Create Date: Sat Aug 04 13:21 2018
 *
 * Brief:
 *   Provides a simple mysql connection class base the mysql connection drive.
 *   This connection class only use to short connection application.
 *
 *****************************************************************************/


#ifndef __H_HUICPP_MYSQL_CONNECTION_H__
#define __H_HUICPP_MYSQL_CONNECTION_H__

#include "dbconnectionbase.h"
#include "mysqldrive.h"


namespace HUICPP {

namespace HUIDB {

class HMysqlConnection : public huidb_internal::dbconnectionbase,
    public huidb_internal::mysql_drive {
public:
    using connection_base = huidb_internal::dbconnectionbase;
    using drive_base = huidb_internal::mysql_drive;

public:
    HMysqlConnection ();

    ~ HMysqlConnection();

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
