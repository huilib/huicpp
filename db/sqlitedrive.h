/*
 * Created Author: Joseph Hui
 * Created Date: Wed 11 May 2022
 * Description:
 *      Wrapper sqlite3 db drive.
*/



#ifndef __H_HUICPP_SQLITE_DRIVE_H__
#define __H_HUICPP_SQLITE_DRIVE_H__


#include <huicpp.h>
#include "hdbres.h"


namespace HUICPP {

namespace HUIDB {

namespace huidb_internal {


class sqlite_drive {
public:
    sqlite_drive();

    /*
        brief: Duplicate a existing connection.
    */  
    sqlite_drive(const sqlite_drive& conn);
   
    sqlite_drive(sqlite_drive&& conn)
        : m_st(conn.m_st) { conn.m_st = nullptr;  }

    virtual ~sqlite_drive ();

public:
    bool IsInited () const noexcept { return m_st != nullptr; }

    HRET Connect (HCSTRR strDbPath);

    void Disconnect ();

    HSTR GetClientVersion () const;

    HRET Execute (HCSTRR sql) const;

    HRET GetResult (HCSTRR sql, HDBRes & res) const;

    HRET ExecuteUnsafe(HCSTRR cmd) const;

private:
    HSTR get_dbfilepath() const;

private:
    HPTR m_st;
};

}

} //HUIDB

} //HUICPP

#endif //__H_HUICPP_SQLITE_DRIVE_H__
