


#include "hmysqlconnection.h"
#include <hlog.h>

namespace HUICPP {

namespace HUIDB {


HMysqlConnection::HMysqlConnection () 
    : connection_base(), drive_base() {

}


HMysqlConnection::~HMysqlConnection() {

}


HRET HMysqlConnection::Query(HCSTRR sql, HDBRes& res) {    

    HNOTOK_MSG_RETURN(Execute(sql), "query mysql failed");

    HNOTOK_MSG_RETURN(StoreResult(res), "store query result failed");

    HRETURN_OK;
}


HRET HMysqlConnection::Execute(HCSTRR sql) {

    HASSERT_MSG_RETURN(IsInited(), "mysql db is not initialized", SRC_ERR);

    HASSERT_MSG_RETURN(IsConnected(), "mysql db is not connected", SRC_ERR);

    RET_T ret = drive_base::Execute(sql);
    IF_NOTOK(ret) {
        if (not Ping()) {
            IF_NOTOK(Reconnect()) {
                SLOG_ERROR("mysql disconnection");
                return ret;
            } else {
                return drive_base::Execute(sql);
            }
        }
    }

    return ret;
}


HRET HMysqlConnection::BeginTransaction () {

    return drive_base::Execute("start transaction");

}

HRET HMysqlConnection::CommitTransaction () {

    return drive_base::Execute("commit");

}


HRET HMysqlConnection::RollBack () {
    
    return drive_base::Execute("rollback");

}


}

}
