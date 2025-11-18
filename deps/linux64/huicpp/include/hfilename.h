/*
 * Created: Joseph Hui Sun Sep 26 2021
 * Description: 
  		Wrapper for filename, directory name.
*/


#ifndef __H_HUICPP_FILENAME_H__
#define __H_HUICPP_FILENAME_H__

#include "huicpp.h"
#include "hstr.h"

#ifndef HWIN
#include "detail/fsname_detail.h"
#endif 

namespace HUICPP {

using file_base = detail::fsname;

class HFileNameBase : public file_base {
public:
	using base_class = file_base;
	using value_type = base_class::value_type;
	using string_type = base_class::string_type;
	using filesize_t = base_class::filesize_t;

public:
	enum class filename_type {
		fn_base,
		fn_dic,
		fn_file,
		fn_end,
	};

public:
	HFileNameBase () noexcept {}

	explicit HFileNameBase (const string_type& str) noexcept : base_class(str) {}

	HFileNameBase(const HFileNameBase& fn) noexcept
		: base_class(fn.GetName()) {  }

	HFileNameBase(HFileNameBase&& fname) noexcept;

	virtual ~HFileNameBase() { }

	HFileNameBase& operator=(const HFileNameBase& fn) noexcept {
		base_class::operator=(fn);
		return *this;
	}

public:	
	virtual filename_type GetType() const noexcept { return filename_type::fn_base; }

	bool Compare (const HFileNameBase& fn) const { 
		return GetName().compare(fn.GetName());
	}

	bool Empty () const noexcept { return GetName().empty(); }



};


class HFileName;
class HDirection : public HFileNameBase {
public:
	using base_class = HFileNameBase;
	using value_type = base_class::value_type;
	using string_type = base_class::string_type;
	using filesize_t = base_class::filesize_t;

public:
	HDirection () noexcept {}

	explicit HDirection(const string_type& s) noexcept : base_class(s) {}

	HDirection (const HDirection& dic) noexcept : base_class(dic) {}

	~HDirection() { }

	HDirection& operator= (const HDirection& dic) noexcept {
		base_class::operator=(dic);
		return *this;
	}

public:
	virtual filename_type GetType() const noexcept { return filename_type::fn_dic; }

	HDirection& SetAsCurrent() noexcept;

	bool IsAbsoluteDic () const noexcept  { return IsAbsolute(); }

	bool IsRelativeDic () const noexcept { return IsRelative(); }

	HDirection& Append (const string_type& strName) noexcept {
		base_class::Append(strName);
		return *this;
	}

	void GoToParent () noexcept;

	HDirection GetParent () const noexcept;

	HRET CreateDic() const noexcept; 

	HFileName GetDicFile(const string_type& strFileName) const noexcept;

	void GetSubFiles (std::vector<string_type>& fs, std::vector<string_type>& ds) const;

};

class HFileName : public HFileNameBase {
public:
	using base_class = HFileNameBase;
	using value_type = base_class::value_type;
	using string_type = base_class::string_type;
	using filesize_t = base_class::filesize_t;

public:
	HFileName () noexcept{}

	HFileName(const string_type& s) noexcept : base_class(s) {}
	
	HFileName (const HFileName& fn) noexcept : base_class(fn) {}
	
	~ HFileName() { }
	
	HFileName& operator= (const HFileName& fn) noexcept {
		base_class::operator=(fn);
		return *this;
	}

public:
	virtual filename_type GetType() const noexcept { return filename_type::fn_file; }

	HFileName& Append (const string_type& new_path) noexcept;

	HDirection GetParentDic () const noexcept;

	string_type GetExtenName () const noexcept;
    
    string_type JustFileName () const noexcept;

	filesize_t GetLength() const noexcept { return base_class::GetFileSize(); }
};


}


#endif //__H_HUICPP_FILENAME_H__

