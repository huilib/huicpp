

#include "hnormalconf.h"
#include "hfilestr.h"
#include "adaptation/ada_sys.h"
#include "hstr.h"
#include <sstream>

#include <iostream>

namespace HUICPP {


HRET HNormalConfig::LoadConfig (HCSTRR strConfigFileName) {

    HASSERT_RETURN(not strConfigFileName.empty(), INVL_PARA);

    HSTR strConfigContent;
    HNOTOK_RETURN(HFileStr(strConfigFileName).ReadToString(strConfigContent));

    HASSERT_RETURN(strConfigContent.length() > MIN_CONTENT_LENGTH, SRC_ERR);

    RET_T ret = ERR_NO(OK);

	do {

	    HVSTR lines;
	    HCSTR key (adapt::line_br());

        ret = HStr(strConfigContent).Split(key, lines);
        IF_NOTOK(ret) {
            break;
        }

	    HCSTR skey("=");

	    for (HVSTR::size_type i = 0; i < lines.size(); ++i) {

            HCSTRR l = lines.at(i);

            if(isNoteLine(l)) {
                continue;
            }

            HVSTR vs;

            ret = HStr(l).Split(skey, vs);
            IF_NOTOK(ret) {
               continue;
            }

            if (vs.size() < 2) {
                continue;
            }

            HStr ikey(vs.at(0));
            ikey.Trim();
            HStr ival(vs.at(1));
            ival.Trim();

            m_conf.SetValue(ikey, ival);

	    }

	} while(0);
	
	return ret;

}


void HNormalConfig::Clear() {

    m_conf.clear();

}


HSTR HNormalConfig::ToJson() const {

    return m_conf.ToJson();

}


HCSTRR HNormalConfig::GetValue(HCSTRR strKey) const noexcept {

    return m_conf.GetValue(strKey);

}


HCSTRR HNormalConfig::GetValue(HCSTRR strKey, HCSTRR strDefVal) const noexcept{

    return m_conf.GetValue(strKey, strDefVal);

}


HN HNormalConfig::GetIntValue(HCSTRR strKey) const noexcept{

    return m_conf.GetInt(strKey);

}


HN HNormalConfig::GetIntValue(HCSTRR strKey, HN nDefVal) const noexcept{

    return m_conf.GetInt(strKey, nDefVal);

}


bool HNormalConfig::Exists(HCSTRR strKey) const {

    return m_conf.Exists(strKey);

}


void HNormalConfig::SetValue (HCSTRR key, HCSTRR val) noexcept {

    m_conf.SetValue(key, val);

}


bool HNormalConfig::isNoteLine (HCSTRR l) const {

    if (l.length() < 3) { 
        return true;
    }

    if (l.substr(0, 2) == ";;") {
        return true;
    }

    return false;

}


}