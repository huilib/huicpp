

template<class _Ty, class _Traits>
HSTR timespan<_Ty, _Traits>::tostr(HCSZ pFormat /*= "%D/%H/%M/%S"*/) const {

	if (pFormat == nullptr)
		return "";        

	HSTR str;
	HCH ch = 0;

	while ((ch = *pFormat++) != '\0') {
		if (ch == '%') {
			HCH tempBuff[8] = {0};
			switch (ch = *pFormat++){
				case '%': {
					str.push_back(ch);
					break;
				}
				case 'D': {
					sprintf(tempBuff, "%d", GetDays());
					str.append(tempBuff);
					break;
				}
				case 'H': {
					sprintf(tempBuff, "%02d", GetHours());
					str.append(tempBuff);          
					break;
				}
				case 'M': {
					sprintf(tempBuff, "%02d", GetMinutes());
					str.append(tempBuff);
					break;
				}
				case 'S': {
					sprintf(tempBuff, "%02d", GetSeconds());
					str.append(tempBuff);               
					break;        
				}
			}
		}
		else {
			str.push_back(ch);
			if (static_cast<HUCH>(ch) > 0x80) {
				str.push_back(*pFormat++);
			}
		}
	}

	return str;
}

