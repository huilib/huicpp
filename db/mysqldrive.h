/*******************************************************
 *
 * FileName: mysqldrive.h
 *
 * Author: Joseph Hui, josephhui1009@gmail.com
 *
 * Create Date: Sat Jul 28 15:45 2018
 *
 * Brief:
 *   Provides a thin abstraction layer over the underlying
 *   database client library.
 *   This class does as little as possible to adapt between
 *   its public interface and the interface require by
 *   underlying C API. That is, in fact, its only mission.
 *
 *******************************************************/



#ifndef __H_HUICPP_MYSQL_DRIVE_H__
#define __H_HUICPP_MYSQL_DRIVE_H__


#include <huicpp.h>
#include "hdbres.h"


namespace HUICPP {

namespace HUIDB {

namespace huidb_internal {

typedef struct {
    HSTR m_host;
    HN m_port;
    HSTR m_user;
    HSTR m_pw;
    HSTR m_db;
} CONN_INFO;

class mysql_drive {
public:
    typedef struct {
        HUN nConnectTimeout;
        HUN nReadTimeout;
        bool bRec;
        HSTR strCharSet;
    } MYSQL_INNER_INFO;

public:
    mysql_drive();

    /*
        brief: Duplicate a existing connection.
    */  
    mysql_drive(const mysql_drive& conn);
   
    mysql_drive(mysql_drive&& conn)
        : m_st(conn.m_st) { conn.m_st = nullptr;  }

    virtual ~mysql_drive ();

public:
    bool IsInited () const noexcept { return m_st != nullptr; }

    bool IsConnected () const noexcept { return m_bConnected; }

    HRET Connect (const CONN_INFO& conn_info, HUN nConnectTimeout = 2, 
        HUN nReadTimeout = 2, bool bRec = false);

    CONN_INFO GetConnInfo (MYSQL_INNER_INFO* pInfo = nullptr) const;

    void Disconnect ();

    HRET Reconnect ();

    HULL GetAffectedRows () const;

    HSTR GetClientVersion () const;

    HULL GetMysqlErrno () const;

    HRET Execute (HCSTRR sql) const;

    HRET StoreResult (HDBRes & res) const;

    HRET UseResult (HDBRes& res) const;

    HULL GetNewId () const;

    bool Ping () const;

    HRET SetReconnect (bool b = true) const;

    HRET SetConnectTimeout (HUN nTimeout = 1) const;

    HRET SetReadTimeout (HUN nTimeout = 1) const;

    HRET SetUtf8 () const;

private:
    void init ();

    void uninit ();

    void copy (const mysql_drive& conn);


private:
    mutable HPTR m_st;

    bool m_bConnected;
};

}

} //HUIDB

} //HUICPP

#endif //__H_HUICPP_MYSQL_DRIVE_H__
