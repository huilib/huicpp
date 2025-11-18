
#include <mysql.h>
#include "mysqldrive.h"
#include <hlog.h>

namespace HUICPP {

namespace HUIDB {

namespace huidb_internal {

#define CHECK_DBST HASSERT_RETURN(IsInited(), ERR_STATUS)
#define CHECK_DB_CONNECTION HASSERT_RETURN(IsInited() and IsConnected(), ERR_STATUS)
#define m_mysql_st static_cast<MYSQL*>(m_st)    

static void saveRes (MYSQL_RES* db_res, HDBRes& res);

mysql_drive::mysql_drive()
    : m_st(nullptr), m_bConnected(false){

    init();

}


mysql_drive::mysql_drive(const mysql_drive& conn) {

    uninit();    

    init();

    copy(conn);

}


mysql_drive::~mysql_drive() {

    Disconnect();

}


HRET mysql_drive::Connect(const CONN_INFO& conn_info, HUN nConnectTimeout, HUN nReadTimeout, bool bRec) {

    CHECK_DBST;

    if (nConnectTimeout != 0) {

        HNOTOK_RETURN(SetConnectTimeout(nConnectTimeout));

    }

    if (nReadTimeout != 0) {

        HNOTOK_RETURN(SetReadTimeout(nReadTimeout));

    }


    HNOTOK_RETURN(SetReconnect(bRec));

    m_st = static_cast<HPTR>(mysql_real_connect(m_mysql_st, conn_info.m_host.c_str(), 
        conn_info.m_user.c_str(), conn_info.m_pw.c_str(), conn_info.m_db.c_str(), 
        conn_info.m_port, nullptr, 0));
    
    if (m_st == nullptr) {
        HRETURN(DEP_ERROR);
    }

    m_bConnected = true;

    HRETURN_OK;
}


CONN_INFO mysql_drive::GetConnInfo (MYSQL_INNER_INFO* pInfo) const {

    HASSERT_THROW(IsInited() and IsConnected(), ERR_STATUS);

    if (pInfo != nullptr) {
        // reconnection, connect-timeout, read-timeout, charset-name.
        bool myval = 0;
        HASSERT_MSG_THROW(mysql_get_option(m_mysql_st, MYSQL_OPT_RECONNECT, &myval) == 0, 
            "mysql_get_option get reconnection flag failed", DEP_ERROR);

        pInfo->bRec = myval;

        HASSERT_MSG_THROW(mysql_get_option(m_mysql_st, MYSQL_OPT_CONNECT_TIMEOUT, &pInfo->nConnectTimeout) == 0, 
            "mysql_get_option get connect-timeout failed", DEP_ERROR);

        HASSERT_MSG_THROW(mysql_get_option(m_mysql_st, MYSQL_OPT_READ_TIMEOUT, &pInfo->nReadTimeout) == 0, 
            "mysql_get_option get read-timeout failed", DEP_ERROR);

        HSTRING_BUF buf = {0};
        HASSERT_MSG_THROW(mysql_get_option(m_mysql_st, MYSQL_SET_CHARSET_NAME, buf) == 0, 
            "mysql_get_option get charset name failed", DEP_ERROR);

        pInfo->strCharSet = buf;

    }

    CONN_INFO info;
    info.m_host = m_mysql_st->host;
    info.m_port = m_mysql_st->port;
    info.m_user = m_mysql_st->user;
    info.m_pw = m_mysql_st->passwd;
    info.m_db = m_mysql_st->db;

    return info;
}


void mysql_drive::Disconnect() {

    uninit();

}


HRET mysql_drive::Reconnect () {

    MYSQL_INNER_INFO inner_info;

    CONN_INFO conn_info = GetConnInfo(&inner_info);

    Disconnect();

    init();

    return Connect(conn_info, inner_info.nConnectTimeout, inner_info.nReadTimeout, inner_info.bRec);
}


HULL mysql_drive::GetAffectedRows() const {

    if (not IsInited() or not IsConnected()) {
        return 0;
    }

    return mysql_affected_rows(m_mysql_st);

}


HSTR mysql_drive::GetClientVersion() const {

    if(not IsInited()) {
        return "Error Mysql Client Version";
    }

    return mysql_get_client_info();

}


HULL mysql_drive::GetMysqlErrno () const {
    if(not IsInited()) {
        return -1;
    }

    return mysql_errno(m_mysql_st);
}


HRET mysql_drive::Execute(HCSTRR sql) const {

    CHECK_DB_CONNECTION;

    HASSERT_RETURN(mysql_real_query(m_mysql_st, sql.c_str(), sql.length()) == 0, DEP_ERROR);

    HRETURN_OK;

}


HRET mysql_drive::StoreResult(HDBRes& res) const {

    CHECK_DB_CONNECTION;

    MYSQL_RES* cb_res = mysql_store_result(m_mysql_st);
    if (cb_res == nullptr) {
        HRETURN_OK;
    }

    saveRes (cb_res, res);

    mysql_free_result(cb_res);

    HRETURN_OK;
}


HRET mysql_drive::UseResult(HDBRes& res) const {

    CHECK_DB_CONNECTION;

    MYSQL_RES* cb_res = mysql_use_result(m_mysql_st);
    if (cb_res == nullptr) {
        HRETURN_OK;
    }

    saveRes (cb_res, res);

    mysql_free_result(cb_res);

    HRETURN_OK;
}


HULL mysql_drive::GetNewId() const {

    CHECK_DB_CONNECTION;

    return (HULL) mysql_insert_id(m_mysql_st);

}


bool mysql_drive::Ping() const {

    if(not IsInited()) {
        return false;
    }

    return mysql_ping(m_mysql_st) == 0;

}


HRET mysql_drive::SetReconnect(bool b) const {

    CHECK_DBST;
    
    HASSERT_RETURN(mysql_options(m_mysql_st, MYSQL_OPT_RECONNECT, &b) == 0, DEP_ERROR);

    HRETURN_OK;
}


HRET mysql_drive::SetConnectTimeout(HUN nTimeout) const {

    CHECK_DBST;

    HASSERT_RETURN(mysql_options(m_mysql_st, MYSQL_OPT_CONNECT_TIMEOUT, &nTimeout) == 0, DEP_ERROR);

    HRETURN_OK;
}


HRET mysql_drive::SetReadTimeout(HUN nTimeout) const {

    CHECK_DBST;

    HASSERT_RETURN(mysql_options(m_mysql_st, MYSQL_OPT_READ_TIMEOUT, &nTimeout) == 0, DEP_ERROR);

    HRETURN_OK;

}


HRET mysql_drive::SetUtf8() const {

    CHECK_DBST;

    HASSERT_RETURN(mysql_options(m_mysql_st, MYSQL_SET_CHARSET_NAME, "utf8") == 0, DEP_ERROR);

    HRETURN_OK;

}


static void saveRes (MYSQL_RES* db_res, HDBRes& res) {

    res.Clear();

    HUN filed_num = mysql_num_fields (db_res);

    MYSQL_FIELD* field = nullptr;
    MYSQL_ROW _row;

    header_t names;

    while ((field = mysql_fetch_field(db_res)) != nullptr) {

        names.push_back(field->name);

    }

    res.SetHeader(names);

    while ((_row = mysql_fetch_row(db_res)) != nullptr) {
        
        row_t row;
        for (HUN i = 0; i < filed_num; ++i) {

            row.push_back (_row[i] ? _row[i] : "NULL");

        }

        res.AddRow(row);

    }

}


void mysql_drive::init() {

    m_st = static_cast<HPTR>(mysql_init (nullptr));
    HASSERT_MSG_THROW(m_st != nullptr, "mysql_init failed", DEP_ERROR);

}


void mysql_drive::uninit() {

    if (IsInited()) {

        mysql_close(m_mysql_st);

        m_st = nullptr;

        m_bConnected = false;

    }

}


#define other_mysql static_cast<MYSQL*>(conn.m_st)

void mysql_drive::copy (const mysql_drive& conn) {

    HASSERT_THROW(IsInited(), ERR_STATUS);
    if (not conn.IsInited()) {
        return ;
    }

    // reconnection, connect-timeout, read-timeout, charset-name.
    bool myval = 0;
    HASSERT_MSG_THROW(mysql_get_option(other_mysql, MYSQL_OPT_RECONNECT, &myval) == 0, 
        "mysql_get_option get reconnection flag failed", DEP_ERROR);
    if (myval) {
        HIGNORE_RETURN(SetReconnect(myval));
    }

    HUN n = 0;
    HASSERT_MSG_THROW(mysql_get_option(other_mysql, MYSQL_OPT_CONNECT_TIMEOUT, &n) == 0, 
        "mysql_get_option get connect-timeout failed", DEP_ERROR);
    HIGNORE_RETURN(SetConnectTimeout(n));

    n = 0;
    HASSERT_MSG_THROW(mysql_get_option(other_mysql, MYSQL_OPT_READ_TIMEOUT, &n) == 0, 
        "mysql_get_option get read-timeout failed", DEP_ERROR);
    HIGNORE_RETURN(SetReadTimeout(n));

    HSTRING_BUF buf = {0};
    HASSERT_MSG_THROW(mysql_get_option(other_mysql, MYSQL_SET_CHARSET_NAME, buf) == 0, 
        "mysql_get_option get charset name failed", DEP_ERROR);
    HASSERT_THROW(mysql_options(other_mysql, MYSQL_SET_CHARSET_NAME, buf) == 0, DEP_ERROR);

    if(conn.IsConnected()) {
        m_st = mysql_real_connect(m_mysql_st, 
            other_mysql->host,  other_mysql->user, other_mysql->passwd, 
            other_mysql->db,  other_mysql->port, nullptr, 0);
        HASSERT_MSG_THROW(m_st != nullptr, "copy connection mysql_real_connect failed", DEP_ERROR);      
    }

    m_bConnected = conn.m_bConnected;
    
}


class ___init_mysql_class {
public:
    ___init_mysql_class() {
        /*
        * In a non-multithreaded environment, mysql_init() invokes mysql_library_init() 
        * automatically as necessary. However, mysql_library_init() is not thread-safe 
        * in a multithreaded environment, and thus neither is mysql_init. 
        * Before calling mysql_init(), either call mysql_library_init prior to spawning 
        * any threads, or use a mutex to protect the mysql_library_init() call. 
        * This should be done prior to any other client library call.
        */
        // Before calling mysql_init(), call mysql_library_init prior.
        (void) mysql_library_init(0, nullptr, nullptr);
    }

    ~ ___init_mysql_class() {
        /*
         * To avoid memory leaks after the application is done using the library, 
         * be sure to call mysql_library_end() explicitly. 
         * This enables memory managment to be performed to clean up and free resources 
         * used by the library.
         */
        (void) mysql_library_end();
    }
};

static const ___init_mysql_class __init_mysql_object;

} // huidb_internal

} //HUIDB

} //HUICPP
