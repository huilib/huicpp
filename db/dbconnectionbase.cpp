

#include "dbconnectionbase.h"
#include <hstr.h>

namespace HUICPP {

namespace HUIDB {

namespace huidb_internal {    


dbconnectionbase::dbconnectionbase() noexcept { }


dbconnectionbase::~dbconnectionbase() noexcept {  }


HRET dbconnectionbase::MultiExec (HCVSTRR sqlss) {

    HNOTOK_RETURN(BeginTransaction());

    for (HVSTR::size_type i = 0; i < sqlss.size(); ++i) {

        HCSTRR str = sqlss[i];

        IF_NOTOK(Execute(str)) {
            
            HIGNORE_RETURN(RollBack());

            HRETURN(DEP_ERROR);

        }

    }

    return CommitTransaction();

}


HRET dbconnectionbase::MultiInsert (HCSTRR strTable, const HDBRes& res) {

    const header_t& ch = res.GetHeader();
    const table_data_t& src_data = res.GetData();

    table_data_t item_data;
    for (table_data_t::size_type i = 0; i < src_data.size(); ++i) {
        const row_t& row = src_data[i];
        item_data.push_back(row);

        if (item_data.size() == MAX_MULTI_INSERT_COUNT) {
            HNOTOK_RETURN(multi_insert_impl(strTable, ch, item_data));
            item_data.clear();
            continue;
        }
    }

    if (not item_data.empty()) {
        HNOTOK_RETURN(multi_insert_impl(strTable, ch, item_data));
        item_data.clear();
    }

    HRETURN_OK;
}


HRET dbconnectionbase::multi_insert_impl(HCSTRR strTabName, 
        const header_t& header, const table_data_t& data) {

    HASSERT_RETURN(not strTabName.empty(), INVL_PARA);

    HSTR filds("( ");
    for (header_t::size_type i = 0; i < header.size(); ++i) {
        const HCSTRR _name = header[i];
        filds += _name + ",";
    }
    filds = filds.substr(0, filds.length() - 1);
    filds += ")";

    HSTR values(" ");
    for (table_data_t::size_type i = 0; i < data.size(); ++i) {
        const row_t& _row = data[i];

        HSTR _row_val ("( ");
        for (row_t::size_type j = 0; j < _row.size(); ++j) {
            HCSTRR val = _row[j];
            _row_val += HSTR("'") + val + "',";
        }
        _row_val = _row_val.substr(0, _row_val.length() - 1);
        _row_val += ")";
        values += _row_val + ",";

    }
    values = values.substr(0, values.length() - 1);

    HSTR sql = HStr::Format("INSERT INTO `%s` %s VALUES %s", 
        strTabName.c_str(), filds.c_str(), values.c_str());

    return Execute(sql);

}

}

}

}

