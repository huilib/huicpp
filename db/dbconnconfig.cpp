

#include "dbconnconfig.h"
#include <nlohmann/json.hpp>
#include <hfilestr.h>
#include <hlog.h>
#include "hsqliteconnection.h"

namespace HUICPP {

namespace HUIDB {


DbConnConfig::DbConnConfig() noexcept 
    : m_db_conns() {

}


DbConnConfig::~ DbConnConfig() {

    for (db_conns_map::iterator it = m_db_conns.begin(); it != m_db_conns.end(); ++it) {
        conns_t& conns = it->second;
        for (conns_t::iterator tt = conns.begin(); tt != conns.end(); ++tt) {
            conn_ptr ptr = *tt;
            HDELP(ptr);
        }
    }
    m_db_conns.clear();

}


HRET DbConnConfig::InitFromConfigFile(HCSTRR strFileName, HUN conn_count) {

    HASSERT_MSG_RETURN(not strFileName.empty(), "empty db config file name", INVL_PARA);
    if (conn_count < min_db_count) { conn_count = min_db_count; }
        
    HSTR strDbConfig;
    HNOTOK_MSG_RETURN(HFileStr(strFileName).ReadToString(strDbConfig), "open db config failed");
    LOG_NORMAL("db config:[%s]", strDbConfig.c_str());
    
    nlohmann::json config_js;
    try {
        config_js = nlohmann::json::parse(strDbConfig);
    } catch (...) {
        LOG_ERROR("parse db config[%s] failed", strDbConfig.c_str());
        HRETURN(INVL_PARA);
    }

    HASSERT_MSG_RETURN(not config_js.is_discarded(), "db config is not a json", ERR_STATUS);

    HASSERT_MSG_RETURN(config_js.contains("dbs"), "miss dbs", INVL_PARA);
    const nlohmann::json& db_js = config_js["dbs"];
    HASSERT_MSG_RETURN(db_js.is_array(), "dbs is not array", INVL_PARA);

    for (nlohmann::json::size_type i = 0; i < db_js.size(); ++i) {
        const nlohmann::json& db_item = db_js[i];
        HASSERT_MSG_RETURN(db_item.contains("dbtype"), "miss db type", INVL_PARA);
        HCSTRR strType = db_item["dbtype"];
        HASSERT_MSG_RETURN(db_item.contains("dbname"), "miss db name", INVL_PARA);
        HCSTRR strDbName = db_item["dbname"];

        if (strType == "SQLITE") {
            HASSERT_MSG_RETURN(db_item.contains("dbpath"), "miss dbpath", INVL_PARA);
            HSTR strDBPath = db_item["dbpath"];
            for (HUN jj = 0; jj < conn_count; ++jj) {
                HSqliteConnection* ptr = new HSqliteConnection();
                CHECK_NEWPOINT(ptr);
                HNOTOK_MSG_RETURN(ptr->Connect(strDBPath), "connect sqlite3 db failed");
                AddConn(strDbName, ptr);
            }

            continue;
        }

        /*
        TODO: 支持mysql
        if (strType == "MYSQL") {

        }*/

        HASSERT_MSG_RETURN(false, "unsupport db type", INVL_PARA);
    }

    HRETURN_OK;
}


void DbConnConfig::AddConn (HCSTRR str, conn_ptr ptr) {

    db_conns_map::iterator fit = m_db_conns.find(str);

    if (fit != m_db_conns.end()) {
        conns_t& conns = fit->second;
        conns.push_back(ptr);
    } else {
        conns_t conns;
        conns.push_back(ptr);
        m_db_conns.insert(db_conns_map::value_type(str, conns));
    }

}

DbConnConfig::conn_ptr DbConnConfig::GetConn (HCSTRR strName, HUN index) {

    db_conns_map::const_iterator fit = m_db_conns.find(strName);
    HASSERT_THROW(fit != m_db_conns.cend(), INVL_PARA);

    const conns_t& conns = fit->second;
    HASSERT_THROW(index < conns.size(), INVL_PARA);
    
    return conns[index];
    
}


HRET DbConnConfig::Query (HCSTRR strName, HUN index, HCSTRR sql, HDBRes& db_res) {

    const DbConnConfig::conn_ptr ptr = GetConn(strName, index);

    return ptr->Query(sql, db_res);

}


HRET DbConnConfig::Exec (HCSTRR strName, HUN index, HCSTRR sql)  {

    const DbConnConfig::conn_ptr ptr = GetConn(strName, index);

    return ptr->Execute(sql);

}


HRET DbConnConfig::Exec (HCSTRR strName, HUN index, HCVSTRR sqlss) {

    const DbConnConfig::conn_ptr ptr = GetConn(strName, index);

    return ptr->MultiExec(sqlss);

}


}

}