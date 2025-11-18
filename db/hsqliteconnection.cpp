


#include "hsqliteconnection.h"
#include <hlog.h>

namespace HUICPP {

namespace HUIDB {


HSqliteConnection::HSqliteConnection () 
    : connection_base(), drive_base() {

}


HSqliteConnection::~HSqliteConnection() {

}


HRET HSqliteConnection::Query(HCSTRR sql, HDBRes& res) {    

    return drive_base::GetResult(sql, res);

}


HRET HSqliteConnection::Execute(HCSTRR sql) {

    return drive_base::Execute(sql);

}


HRET HSqliteConnection::BeginTransaction () {

    return drive_base::ExecuteUnsafe("BEGIN;");

}

HRET HSqliteConnection::CommitTransaction () {

    return drive_base::ExecuteUnsafe("COMMIT;");

}


HRET HSqliteConnection::RollBack () {
    
    return drive_base::ExecuteUnsafe("ROLLBACK;");

}


}

}
