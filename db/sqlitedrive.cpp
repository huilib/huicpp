

#include "sqlitedrive.h"
#include <sqlite3.h>
#include <hstr.h>
#include <hlog.h>
#include <htodoholder.h>

namespace HUICPP {

namespace HUIDB {

namespace huidb_internal {

#define m_sqlite_st static_cast<sqlite3*>(m_st)



sqlite_drive::sqlite_drive()
    : m_st(nullptr) {  }


sqlite_drive::sqlite_drive(const sqlite_drive& conn) 
    :m_st(nullptr) {

    HSTR strDBPath = conn.get_dbfilepath();
    HIGNORE_RETURN(Connect(strDBPath));

}


sqlite_drive::~sqlite_drive () {

    Disconnect();

}


HRET sqlite_drive::Connect (HCSTRR strDbPath) {

    HASSERT_RETURN(m_st == nullptr, SRC_ERR);
    sqlite3* sl3 = static_cast<sqlite3*>(m_st);

    HN ret = sqlite3_open(strDbPath.c_str(), &sl3);

    HASSERT_MSG_RETURN(ret == SQLITE_OK, "open sqlite3 db file failed", DEP_ERROR);

    m_st = static_cast<HPTR>(sl3);

    HRETURN_OK;

}


void sqlite_drive::Disconnect () {

    if (m_st != nullptr) {
        sqlite3_close(m_sqlite_st);
        m_st = nullptr;
    }

}


HSTR sqlite_drive::GetClientVersion () const {

    return HStr::Format("%s-%s-%s", SQLITE_VERSION, SQLITE_VERSION_NUMBER, SQLITE_SOURCE_ID);

}


HRET sqlite_drive::Execute (HCSTRR sql) const {

    HASSERT_RETURN(m_st != nullptr, SRC_ERR);

    sqlite3_stmt* stmt = nullptr;
    HN ret = sqlite3_prepare(m_sqlite_st, sql.c_str(), sql.length(), &stmt, nullptr);
    HASSERT_MSG_RETURN(ret == SQLITE_OK, "sqlite execute sql error", INVL_PARA);

    HSCOPE_EXIT {
        sqlite3_finalize(stmt);
    };

    HASSERT_RETURN(sqlite3_step(stmt) == SQLITE_OK, DEP_ERROR);

    HRETURN_OK;
}


HRET sqlite_drive::GetResult (HCSTRR sql, HDBRes & res) const {

    HASSERT_RETURN(m_st != nullptr, SRC_ERR);

    res.Clear();

    sqlite3_stmt* stmt = nullptr;
    HN ret = sqlite3_prepare(m_sqlite_st, sql.c_str(), sql.length(), &stmt, nullptr);
    HASSERT_MSG_RETURN(ret == SQLITE_OK, "sqlite execute sql error", INVL_PARA);

    HSCOPE_EXIT {
        sqlite3_finalize(stmt);
    };

    header_t header;
    table_data_t data;

    while(sqlite3_step(stmt) == SQLITE_ROW) {

        HN cc = sqlite3_column_count(stmt);

        // fill column name
        if (header.empty()) {
            for (HN i = 0; i < cc; ++i) {
                header.push_back(sqlite3_column_name(stmt, i));
            }
        }

        // fill column value
        row_t row;
        for (HN i = 0; i < cc; ++i) {            
            row.push_back(reinterpret_cast<const char*>(sqlite3_column_text(stmt, i)));
        }
        data.emplace_back(row);
    }   

    res.SetHeader(header);
    res.SetData(data);

    HRETURN_OK;

}


HRET sqlite_drive::ExecuteUnsafe(HCSTRR cmd) const {

    HASSERT_RETURN(m_st != nullptr, SRC_ERR);

    HASSERT_RETURN(sqlite3_exec(m_sqlite_st, cmd.c_str(), nullptr, nullptr, nullptr)
        == SQLITE_OK, INVL_PARA);

    HRETURN_OK;

}


HSTR sqlite_drive::get_dbfilepath() const {

    HASSERT_THROW(m_st != nullptr, SRC_ERR);

    HPATH_BUF buf = {0};
    HSTR res(sqlite3_db_filename(m_sqlite_st, buf));
    return res;

}


}

}

}

