

template<class _Ty, class _Traits>
time_basic<_Ty, _Traits>& time_basic<_Ty, _Traits>::
    FromStr(HCSTRR strTime, HCSZ szFormat) {

    if(strTime.empty() or szFormat == nullptr) {
        return *this;
    }

    tm_t atm = {0};
    memset(&atm, 0, sizeof(atm));

    HCSZ pFormatTag = szFormat;
    HCSZ pDataPart = strTime.c_str();
    bool bValidData = true;

    for(;*pFormatTag != '\0' and bValidData;) {

        for (;*pFormatTag != '%' and *pFormatTag != '\0';) {   

            HCH ch = *pFormatTag;
            HCH ch2 = *(pFormatTag + 1);

            ++pFormatTag;
            ++pDataPart;
            if ((ch == '\t' or ch == '\x20') and (ch2 != '\t' and ch2 != '\x20')) {

                for (;(*pDataPart == '\x20' or *pDataPart == '\t') 
                    and (*pDataPart != '\0'); ++pDataPart);

            } else if (static_cast<HUCH>(ch) > 0x80) {   

                ++pFormatTag;
                ++pDataPart;

            }
        }

        if (*pFormatTag == '\0')
            break;

        ++pFormatTag;
        itype width = -1;
        HSTR strTemp;
        for (;*pFormatTag >= '0' and *pFormatTag <= '9'; 
            strTemp += *pFormatTag, ++pFormatTag) {
        }

        if (not strTemp.empty()) {
            width = atoi(strTemp.c_str());
        }

        HCH flag = *pFormatTag++;    
        if (flag == '%') {
            bValidData = (*pDataPart++ == '%');            
            continue;
        }

        strTemp = "";
        for (;*pDataPart >= '0' and *pDataPart <= '9' and 
            (width == -1 or (int)strTemp.length() < width) and *pDataPart != '\0'; 
                strTemp += *pDataPart, ++pDataPart);

        if (strTemp.empty())  {
            bValidData = false;
            continue;
        }

        itype val = atoi(strTemp.c_str());
        switch(flag) {
            case 'y': {  
                atm.tm_year = val + 100; // 2000 + val - 1900
                break;
            }

            case 'Y': {
                atm.tm_year = val - 1900;
                break;
            }

            case 'm': {
                atm.tm_mon = val - 1;
                break;
            }

            case 'd': {
                atm.tm_mday = val;
                break;
            }

            case 'H': {
                atm.tm_hour = val;
                break;
            }

            case 'M': {
                atm.tm_min = val;
                break;
            }

            case 'S': {
                atm.tm_sec = val;
                break;
            }

            default:
                bValidData = false;
                break;
        }
    }    

    if (bValidData) {
        m_time = traits_type::make_me(atm);    
    }

    return *this;

}



template<class _Ty, class _Traits>
typename time_basic<_Ty, _Traits>::tm_t* 
    time_basic<_Ty, _Traits>::GetGmtTm(tm_t& tm) const {
        
    HIGNORE_RETURN(adapt::hgmtime(&m_time, &tm));

    return &tm;
        
}


template<class _Ty, class _Traits>
typename time_basic<_Ty, _Traits>::tm_t* 
time_basic<_Ty, _Traits>::GetLocalTm(tm_t& tm) const {

    HIGNORE_RETURN(adapt::hlocaltime(&m_time, &tm));

    return &tm;

}


template<class _Ty, class _Traits>
HSTR time_basic<_Ty, _Traits>::ToString(HCSTRR strFormat) const {    

    if (m_time == traits_type::GetInitValue()) {
	    return "invalid time";
    }

    if(strFormat.empty()) {
        return "";   
    } 

    tm_t tmt;
    ZERO_MEM(&tmt, sizeof(tm_t));
    HIGNORE_RETURN(GetLocalTm(tmt));

    HCH buf[TIME_BUF_LEN] = {0}; 
    HIGNORE_RETURN(strftime(buf, TIME_BUF_LEN, strFormat.c_str(), &tmt));

    return buf;
    
}



template<class _Ty, class _Traits>
time_basic<_Ty, _Traits> time_basic<_Ty, _Traits>::GetCurrentTime() {

    time_basic tb;
    tb.SetAsNow();
    return tb;

}


template<class _Ty, class _Traits>
HRET time_basic<_Ty, _Traits>::GetNow(time_buf_t buf) {

    traits_type::reset_time_buf(buf);

    time_basic tb;
    tb.SetAsNow();
    
    tm_t _tm_;
    ZERO_MEM(&_tm_, sizeof(tm_t));
    HIGNORE_RETURN(tb.GetLocalTm(_tm_));

    snprintf(buf, TIME_BUF_LEN, "%04d-%02d-%02d %02d:%02d:%02d",
            _tm_.tm_year + 1900, _tm_.tm_mon + 1,
            _tm_.tm_mday, _tm_.tm_hour, _tm_.tm_min,
            _tm_.tm_sec);

    HRETURN_OK;

}


template<class _Ty, class _Traits>
HSTR time_basic<_Ty, _Traits>::GetNow() {

    time_buf_t buf = {0};
    HIGNORE_RETURN(GetNow(buf));
    return HSTR(buf);

}


template<class _Ty, class _Traits>
HRET time_basic<_Ty, _Traits>::GetDate(time_buf_t buf) {

    traits_type::reset_time_buf(buf);

    time_basic tb;
    tb.SetAsNow();
    
    tm_t _tm_ = {0};
    HIGNORE_RETURN(tb.GetLocalTm(_tm_));

    snprintf(buf, TIME_BUF_LEN, "%04d-%02d-%02d",
            _tm_.tm_year + 1900, _tm_.tm_mon + 1,
            _tm_.tm_mday);

    HRETURN_OK;

}


template<class _Ty, class _Traits>
HSTR time_basic<_Ty, _Traits>::GetDate() {

    time_buf_t buf = {0};
    HIGNORE_RETURN(GetDate(buf));
    return HSTR(buf);

}




template<class _Ty, class _Traits>
HSTR time_basic<_Ty, _Traits>::GetTimeGroup () {
	
	time_basic tb;
    tb.SetAsNow();

	return GetTimeGroup(tb);

}



template<class _Ty, class _Traits>
HSTR time_basic<_Ty, _Traits>::GetTimeGroup (const time_basic<_Ty, _Traits>& nt) {

	HSTR strNg = nt.ToString("%Y%m%d%H");

	if (nt.GetMinute() > 29) {

		strNg.append("30");

	} else {

		strNg.append("00");

	}

	return strNg;
}



template<class _Ty, class _Traits>
HSTR time_basic<_Ty, _Traits>::GetPrevTimeGroup() {

	time_basic tb;
    tb.SetAsNow();
	timespan_t ts(0,0,30); // 30 minutes
	tb -= ts;

	return GetTimeGroup(tb);

}