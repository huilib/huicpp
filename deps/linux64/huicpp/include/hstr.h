/*
 * Created: Joseph Dec-23-2016
 * Description: 
  		Wrapper for huicpp string.
*/

#ifndef _H_HUICPP_STR_H_
#define _H_HUICPP_STR_H_

#include "huicpp.h"

namespace HUICPP{

class HStr : public HSTR{
public:
	using base = HSTR;
	using value_type = base::value_type;
	using size_type = base::size_type;
	using allocator_type = base::allocator_type;

public:
	HStr() {}

    HStr(const HStr & str) : base(str) {}

	HStr(const base& str) : base(str) {}

	HStr(const value_type * sz) : base(sz) {}

	HStr(const value_type * c, size_type n, const allocator_type & al = allocator_type())
	    : base(c, n, al) {}

	HStr(const base & b, size_type pos, size_type count = base::npos )
	    : base(b, pos, count) {}

	HStr(size_type c, value_type v)
	    :base(c, v) {}
	
public:
	HStr& operator= (const value_type * vt) {
	    assign(vt);
	    return *this;
	}

	HStr& operator= (const HStr& str) {
	    assign(str);
	    return *this;
	}

public:
	HRET Split(HCSTRR key, HVSTRR res, HSIZE beg = 0) const;

	HStr& Ltrim(HCSZ csz = nullptr);
	
	HStr& Rtrim (HCSZ csz = nullptr);
	
	HStr& Trim(HCSZ csz = nullptr);

	HStr& Upper();

	HStr& Lower();

	bool IsAllDigit() const noexcept {
	    return IsAllDigit(*this);
	}

	bool IsAllDigitAndDos() const noexcept {
		return IsAllDigitAndDos(*this);
	}

	bool IsAllDigitAndChar() const noexcept {
		return IsAllDigitAndChar(*this);
	}

	HStr& RemoveRight(HSIZE len);

	HStr& RemoveLeft(HSIZE len);

	HStr& Replace(HCSTRR srckey, HCSTRR dstKey);

	HStr& Replaces(HCSTRR srcKey, HCSTRR dstKey);

	bool IsIn(HCSTRR str) const noexcept {
		return IsIn(*this, str);
	}

	bool IsBeginWith (HCSTRR key) const noexcept {
		return IsBeginWith(*this, key);
	}

	bool IsEndWith (HCSTRR key) const noexcept {
		return IsEndWith(*this, key);
	}

	HSIZE Counter (HCSTRR key) const noexcept {
		return Counter(*this, key);
	}
	
	HStr& Remove(HCSTRR srcKey);

	HStr& Removes(HCSTRR srcKey);

	HN ToN () const {
	    return std::stoi(*this);
	}

    HLN ToL () const {
		return std::stol(*this);
	}

	HDBE ToD() const {
		return std::stod(*this);
	}

	HLL ToLL() const {
		return std::stoll(*this);
	}


	HStr& AppendFormat(const value_type* fmt, ... );

	HStr& ToHexString(HCPTR data, HSIZE len);
	
public:	
	static RET_T Lower(HSTRR str);

	static bool IsAllDigit(HCSTRR str) noexcept;

	static bool IsAllDigitAndDos (HCSTRR str) noexcept;

    static bool IsAllDigitAndChar (HCSTRR str) noexcept;

	static void RemoveRight(HSTRR str, HSIZE len);

	static void RemoveLeft(HSTRR str, HSIZE len);

	static RET_T Replace (HSTRR src, HCSTRR srcKey, HCSTRR dstKey);

	static RET_T Replaces (HSTRR src, HCSTRR srcKey, HCSTRR dstkey);

	static bool IsIn(HCSTRR src, HCSTRR key) noexcept;

	static bool IsBeginWith (HCSTRR src, HCSTRR key) noexcept;

	static bool IsEndWith (HCSTRR src, HCSTRR key) noexcept;

	static HSIZE Counter (HCSTRR src, HCSTRR key) noexcept;

	static RET_T Remove(HSTRR src, HCSTRR strKey);

	static RET_T Removes(HSTRR src, HCSTRR strKey);

	static HStr V2Str (HCVSTRR strs, HCSTR l = "|");

	static HStr Format(const value_type* fmt, ...);

	static HStr vFormat(const value_type* fmt, va_list ap);

	static HStr FromN(HN n){
		return std::to_string(n);
	}

	static HStr FromL(HLN l){
		return std::to_string(l);
	}

	static HStr FromD(HDBE d){
		return std::to_string(d);
	}

    static HStr FromLL (HLL ll) {
		return std::to_string(ll);
	}

private:
	static HStr s_tr_str;

};

}

#endif


