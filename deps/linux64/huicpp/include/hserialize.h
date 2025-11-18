/**********************************************************
 * Created: Joseph Dec-16-2018
 * Description: 
 * 		Wrapper for data object serialize.
 **********************************************************/

#ifndef __H_HUICPP_SERIALIZE_H__
#define __H_HUICPP_SERIALIZE_H__

#include <limits>
#include <type_traits>
#include "huicpp.h"


namespace HUICPP {

namespace {

template<typename T>
inline T& REF(const T& val) {
    return const_cast<T&>(val);
}

static const HUN MAX_SIZE = 0x02000000;

}    

/////////////////////////////////////////////////////////////////
//
// Templates for serializing to anything that looks like a stream,
// i.e. anything that supports .read(char*, size_t) and .write(char*, size_t)
//

#define IMPLEMENT_SERIALIZE(statements)                                 \
    HSIZE GetSerializeSize() const {                                    \
        tag_SerActionGetSerializeSize ser_action;                       \
        HSIZE nSerSize = 0;                                             \
        tag_streamplaceholder s;                                        \
        {statements}                                                    \
        return nSerSize;                                                \
    }                                                                   \
    template<typename Stream>                                           \
    void Serialize(Stream& s) const {                                   \
        tag_SerActionSerialize ser_action;                              \
        HSIZE nSerSize = 0;                                             \
        {statements}                                                    \
    }                                                                   \
    template<typename Stream>                                           \
    void Unserialize(Stream& s) {                                       \
        tag_SerActionUnserialize ser_action;                            \
        HSIZE nSerSize = 0;                                             \
        {statements}                                                    \
    }

#define READWRITE(obj)      (nSerSize += HUICPP::SerReadWrite(s, (obj), ser_action))

#define WRITEDATA(s, obj)   s.write(reinterpret_cast<HCSZ>(&(obj)), sizeof(obj))
#define READDATA(s, obj)    s.read(reinterpret_cast<HSZ>(&(obj)), sizeof(obj))

inline HSIZE GetSerializeSize(HCH     a)      { return sizeof(a); }
inline HSIZE GetSerializeSize(HUCH    a)      { return sizeof(a); }
inline HSIZE GetSerializeSize(HSN     a)      { return sizeof(a); }
inline HSIZE GetSerializeSize(HUSN    a)      { return sizeof(a); }
inline HSIZE GetSerializeSize(HN      a)      { return sizeof(a); }
inline HSIZE GetSerializeSize(HUN     a)      { return sizeof(a); }
inline HSIZE GetSerializeSize(HLN     a)      { return sizeof(a); }
inline HSIZE GetSerializeSize(HULN    a)      { return sizeof(a); }
inline HSIZE GetSerializeSize(HLL     a)      { return sizeof(a); }
inline HSIZE GetSerializeSize(HULL    a)      { return sizeof(a); }
inline HSIZE GetSerializeSize(HFLD    a)      { return sizeof(a); }
inline HSIZE GetSerializeSize(HDBE    a)      { return sizeof(a); }

template<typename Stream> inline void Serialize(Stream& s, HCH a)       { WRITEDATA(s, a); }
template<typename Stream> inline void Serialize(Stream& s, HUCH a)      { WRITEDATA(s, a); }
template<typename Stream> inline void Serialize(Stream& s, HSN a)       { WRITEDATA(s, a); }
template<typename Stream> inline void Serialize(Stream& s, HUSN a)      { WRITEDATA(s, a); }
template<typename Stream> inline void Serialize(Stream& s, HN a)        { WRITEDATA(s, a); }
template<typename Stream> inline void Serialize(Stream& s, HUN a)       { WRITEDATA(s, a); }
template<typename Stream> inline void Serialize(Stream& s, HLN a)       { WRITEDATA(s, a); }
template<typename Stream> inline void Serialize(Stream& s, HULN a)      { WRITEDATA(s, a); }
template<typename Stream> inline void Serialize(Stream& s, HLL a)       { WRITEDATA(s, a); }
template<typename Stream> inline void Serialize(Stream& s, HULL a)      { WRITEDATA(s, a); }
template<typename Stream> inline void Serialize(Stream& s, HFLD a)      { WRITEDATA(s, a); }
template<typename Stream> inline void Serialize(Stream& s, HDBE a)      { WRITEDATA(s, a); }

template<typename Stream> inline void Unserialize(Stream& s, HCHR a)    { READDATA(s, a); }
template<typename Stream> inline void Unserialize(Stream& s, HUCHR a)   { READDATA(s, a); }
template<typename Stream> inline void Unserialize(Stream& s, HSNR a)    { READDATA(s, a); }
template<typename Stream> inline void Unserialize(Stream& s, HUSNR a)   { READDATA(s, a); }
template<typename Stream> inline void Unserialize(Stream& s, HNR a)     { READDATA(s, a); }
template<typename Stream> inline void Unserialize(Stream& s, HUNR a)    { READDATA(s, a); }
template<typename Stream> inline void Unserialize(Stream& s, HLNR a)    { READDATA(s, a); }
template<typename Stream> inline void Unserialize(Stream& s, HULNR a)   { READDATA(s, a); }
template<typename Stream> inline void Unserialize(Stream& s, HLLR a)    { READDATA(s, a); }
template<typename Stream> inline void Unserialize(Stream& s, HULLR a)   { READDATA(s, a); }
template<typename Stream> inline void Unserialize(Stream& s, HFLDR a)   { READDATA(s, a); }
template<typename Stream> inline void Unserialize(Stream& s, HDBER a)   { READDATA(s, a); }

inline HSIZE GetSerializeSize(bool )  { return sizeof(HCH); }
template<typename Stream> inline void Serialize(Stream& s, bool a)    { HCH f = a; WRITEDATA(s, f); }
template<typename Stream> inline void Unserialize(Stream& s, bool& a) { HCH f; READDATA(s, f); a = f; }


//
// Compact size
//  size <  253        -- 1 byte
//  size <= USHRT_MAX  -- 3 bytes  (253 + 2 bytes)
//  size <= UINT_MAX   -- 5 bytes  (254 + 4 bytes)
//  size >  UINT_MAX   -- 9 bytes  (255 + 8 bytes)
//
inline HSIZE GetSizeOfCompactSize(uint64_t nSize) {
    if (nSize < 253) { 
        return sizeof(HUCH);
    } else if (nSize <= std::numeric_limits<HUSN>::max()) {
        return sizeof(HUCH) + sizeof(HUSN);
    } else if (nSize <= std::numeric_limits<HUN>::max()) {
        return sizeof(HUCH) + sizeof(HUN);
    } else {
         return sizeof(HUCH) + sizeof(uint64_t);
    }
    return 0;
}

template<typename Stream>
void WriteCompactSize(Stream& os, uint64_t nSize) {
    if (nSize < 253)
    {
        HUCH chSize = nSize;
        WRITEDATA(os, chSize);
    }
    else if (nSize <= std::numeric_limits<HUSN>::max())
    {
        HUCH chSize = 253;
        HUSN xSize = nSize;
        WRITEDATA(os, chSize);
        WRITEDATA(os, xSize);
    }
    else if (nSize <= std::numeric_limits<HUN>::max())
    {
        HUCH chSize = 254;
        HUN xSize = nSize;
        WRITEDATA(os, chSize);
        WRITEDATA(os, xSize);
    }
    else
    {
        HUCH chSize = 255;
        uint64_t xSize = nSize;
        WRITEDATA(os, chSize);
        WRITEDATA(os, xSize);
    }
    return;
}

template<typename Stream>
HSIZE ReadCompactSize(Stream& is) {
    HUCH chSize;
    READDATA(is, chSize);
    HSIZE nSizeRet = 0;
    if (chSize < 253)
    {
        nSizeRet = chSize;
    }
    else if (chSize == 253)
    {
        HUSN xSize;
        READDATA(is, xSize);
        nSizeRet = xSize;
    }
    else if (chSize == 254)
    {
        HUN xSize;
        READDATA(is, xSize);
        nSizeRet = xSize;
    }
    else
    {
        uint64_t xSize;
        READDATA(is, xSize);
        nSizeRet = xSize;
    }
    return nSizeRet;
}


// Variable-length integers: bytes are a MSB base-128 encoding of the number.
// The high bit in each byte signifies whether another digit follows. To make
// the encoding is one-to-one, one is subtracted from all but the last digit.
// Thus, the byte sequence a[] with length len, where all but the last byte
// has bit 128 set, encodes the number:
//
//   (a[len-1] & 0x7F) + sum(i=1..len-1, 128^i*((a[len-i-1] & 0x7F)+1))
//
// Properties:
// * Very small (0-127: 1 byte, 128-16511: 2 bytes, 16512-2113663: 3 bytes)
// * Every integer has exactly one encoding
// * Encoding does not depend on size of original integer type
// * No redundancy: every (infinite) byte sequence corresponds to a list
//   of encoded integers.
//
// 0:         [0x00]  256:        [0x81 0x00]
// 1:         [0x01]  16383:      [0xFE 0x7F]
// 127:       [0x7F]  16384:      [0xFF 0x00]
// 128:  [0x80 0x00]  16511: [0x80 0xFF 0x7F]
// 255:  [0x80 0x7F]  65535: [0x82 0xFD 0x7F]
// 2^32:           [0x8E 0xFE 0xFE 0xFF 0x00]

template<typename I>
inline HSIZE GetSizeOfVarInt(I n) {
    HSIZE nRet = 0;
    while(true) {
        nRet++;
        if (n <= 0x7F)
            break;
        n = (n >> 7) - 1;
    }
    return nRet;
}

template<typename Stream, typename I>
void WriteVarInt(Stream& os, I n) {
    HUCH tmp[(sizeof(n)*8+6)/7];
    HN len=0;
    while(true) {
        tmp[len] = (n & 0x7F) | (len ? 0x80 : 0x00);
        if (n <= 0x7F)
            break;
        n = (n >> 7) - 1;
        len++;
    }
    do {
        WRITEDATA(os, tmp[len]);
    } while(len--);
}

template<typename Stream, typename I>
I ReadVarInt(Stream& is) {
    I n = 0;
    while(true) {
        HUCH chData;
        READDATA(is, chData);
        n = (n << 7) | (chData & 0x7F);
        if (chData & 0x80)
            n++;
        else
            return n;
    }
}

#define FLATDATA(obj)  REF(CFlatData((char*)&(obj), (char*)&(obj) + sizeof(obj)))
#define VARINT(obj)    REF(WrapVarInt(REF(obj)))

/** Wrapper for serializing arrays and POD.
 */
class CFlatData
{
protected:
    char* pbegin;
    char* pend;
public:
    CFlatData(void* pbeginIn, void* pendIn) : pbegin((char*)pbeginIn), pend((char*)pendIn) { }
    char* begin() { return pbegin; }
    const char* begin() const { return pbegin; }
    char* end() { return pend; }
    const char* end() const { return pend; }

    HSIZE GetSerializeSize() const
    {
        return pend - pbegin;
    }

    template<typename Stream>
    void Serialize(Stream& s) const
    {
        s.write(pbegin, pend - pbegin);
    }

    template<typename Stream>
    void Unserialize(Stream& s)
    {
        s.read(pbegin, pend - pbegin);
    }
};

template<typename I>
class CVarInt
{
protected:
    I &n;
public:
    CVarInt(I& nIn) : n(nIn) { }

    HSIZE GetSerializeSize() const {
        return GetSizeOfVarInt<I>(n);
    }

    template<typename Stream>
    void Serialize(Stream &s) const {
        WriteVarInt<Stream,I>(s, n);
    }

    template<typename Stream>
    void Unserialize(Stream& s) {
        n = ReadVarInt<Stream,I>(s);
    }
};

template<typename I>
CVarInt<I> WrapVarInt(I& n) { return CVarInt<I>(n); }

//
// Forward declarations
//

// string
template<typename C> HSIZE GetSerializeSize(const std::basic_string<C>& str);
template<typename Stream, typename C> void Serialize(Stream& os, const std::basic_string<C>& str);
template<typename Stream, typename C> void Unserialize(Stream& is, std::basic_string<C>& str);

// vector
template<typename Stream, typename T, typename A> inline void Serialize(Stream& os, const std::vector<T, A>& v);
template<typename Stream, typename T, typename A> inline void Unserialize(Stream& is, std::vector<T, A>& v);


// pair
template<typename K, typename T> HSIZE GetSerializeSize(const std::pair<K, T>& item);
template<typename Stream, typename K, typename T> void Serialize(Stream& os, const std::pair<K, T>& item);
template<typename Stream, typename K, typename T> void Unserialize(Stream& is, std::pair<K, T>& item);


// map
template<typename K, typename T, typename Pred, typename A> HSIZE GetSerializeSize(const std::map<K, T, Pred, A>& m);
template<typename Stream, typename K, typename T, typename Pred, typename A> void Serialize(Stream& os, const std::map<K, T, Pred, A>& m);
template<typename Stream, typename K, typename T, typename Pred, typename A> void Unserialize(Stream& is, std::map<K, T, Pred, A>& m);

// set
template<typename K, typename Pred, typename A> HSIZE GetSerializeSize(const std::set<K, Pred, A>& m);
template<typename Stream, typename K, typename Pred, typename A> void Serialize(Stream& os, const std::set<K, Pred, A>& m);
template<typename Stream, typename K, typename Pred, typename A> void Unserialize(Stream& is, std::set<K, Pred, A>& m);



template<typename T>
inline HSIZE GetSerializeSize(const T& a)
{
    return a.GetSerializeSize();
}

template<typename Stream, typename T>
inline void Serialize(Stream& os, const T& a)
{
    a.Serialize(os);
}

template<typename Stream, typename T>
inline void Unserialize(Stream& is, T& a)
{
    a.Unserialize(is);
}


//
// string
//
template<typename C>
HSIZE GetSerializeSize(const std::basic_string<C>& str)
{
    return GetSizeOfCompactSize(str.size()) + str.size() * sizeof(str[0]);
}

template<typename Stream, typename C>
void Serialize(Stream& os, const std::basic_string<C>& str)
{
    WriteCompactSize(os, str.size());
    if (!str.empty())
        os.write((char*)&str[0], str.size() * sizeof(str[0]));
}

template<typename Stream, typename C>
void Unserialize(Stream& is, std::basic_string<C>& str)
{
    unsigned int nSize = ReadCompactSize(is);
    str.resize(nSize);
    if (nSize != 0)
        is.read((char*)&str[0], nSize * sizeof(str[0]));
}


// vertor
template<typename T>
HSIZE GetSerializeSize_impl(const std::vector<T>& v, const std::true_type&)
{
    return (GetSizeOfCompactSize(v.size()) + v.size() * sizeof(T));
}

template<typename T>
HSIZE GetSerializeSize_impl(const std::vector<T>& v, const std::false_type&)
{
    HSIZE nSize = GetSizeOfCompactSize(v.size());
    for (typename std::vector<T>::const_iterator vi = v.begin(); vi != v.end(); ++vi)
        nSize += GetSerializeSize((*vi));
    return nSize;
}

template<typename T>
inline HSIZE GetSerializeSize(const std::vector<T>& v)
{
    return GetSerializeSize_impl(v, std::is_fundamental<T>());
}


template<typename Stream, typename T>
void Serialize_impl(Stream& os, const std::vector<T>& v, const std::true_type&)
{
    WriteCompactSize(os, v.size());
    if (!v.empty())
        os.write((char*)&v[0], v.size() * sizeof(T));
}

template<typename Stream, typename T>
void Serialize_impl(Stream& os, const std::vector<T>& v, const std::false_type&)
{
    WriteCompactSize(os, v.size());
    for (typename std::vector<T>::const_iterator vi = v.begin(); vi != v.end(); ++vi)
        HUICPP::Serialize(os, (*vi));
}

template<typename Stream, typename T>
inline void Serialize(Stream& os, const std::vector<T>& v)
{
    Serialize_impl(os, v, std::is_fundamental<T>());
}


template<typename Stream, typename T>
void Unserialize_impl(Stream& is, std::vector<T>& v, const std::true_type&)
{
    // Limit size per read so bogus size value won't cause out of memory
    v.clear();
    unsigned int nSize = ReadCompactSize(is);
    unsigned int i = 0;
    while (i < nSize)
    {
        unsigned int blk = std::min(nSize - i, (unsigned int)(1 + 4999999 / sizeof(T)));
        v.resize(i + blk);
        is.read((char*)&v[i], blk * sizeof(T));
        i += blk;
    }
}

template<typename Stream, typename T>
void Unserialize_impl(Stream& is, std::vector<T>& v, const std::false_type&)
{
    v.clear();
    unsigned int nSize = ReadCompactSize(is);
    unsigned int i = 0;
    unsigned int nMid = 0;
    while (nMid < nSize)
    {
        nMid += 5000000 / sizeof(T);
        if (nMid > nSize)
            nMid = nSize;
        v.resize(nMid);
        for (; i < nMid; i++)
            Unserialize(is, v[i]);
    }
}

template<typename Stream, typename T>
inline void Unserialize(Stream& is, std::vector<T>& v)
{
    Unserialize_impl(is, v, std::is_fundamental<T>());
}


//
// pair
//
template<typename K, typename T>
HSIZE GetSerializeSize(const std::pair<K, T>& item)
{
    return GetSerializeSize(item.first) + GetSerializeSize(item.second);
}

template<typename Stream, typename K, typename T>
void Serialize(Stream& os, const std::pair<K, T>& item)
{
    Serialize(os, item.first);
    Serialize(os, item.second);
}

template<typename Stream, typename K, typename T>
void Unserialize(Stream& is, std::pair<K, T>& item)
{
    Unserialize(is, item.first);
    Unserialize(is, item.second);
}




//
// map
//
template<typename K, typename T, typename Pred, typename A>
HSIZE GetSerializeSize(const std::map<K, T, Pred, A>& m)
{
    HSIZE nSize = GetSizeOfCompactSize(m.size());
    for (typename std::map<K, T, Pred, A>::const_iterator mi = m.begin(); mi != m.end(); ++mi)
        nSize += GetSerializeSize((*mi));
    return nSize;
}

template<typename Stream, typename K, typename T, typename Pred, typename A>
void Serialize(Stream& os, const std::map<K, T, Pred, A>& m)
{
    WriteCompactSize(os, m.size());
    for (typename std::map<K, T, Pred, A>::const_iterator mi = m.begin(); mi != m.end(); ++mi)
        Serialize(os, (*mi));
}

template<typename Stream, typename K, typename T, typename Pred, typename A>
void Unserialize(Stream& is, std::map<K, T, Pred, A>& m)
{
    m.clear();
    unsigned int nSize = ReadCompactSize(is);
    typename std::map<K, T, Pred, A>::iterator mi = m.begin();
    for (unsigned int i = 0; i < nSize; i++)
    {
        std::pair<K, T> item;
        Unserialize(is, item);
        mi = m.insert(mi, item);
    }
}



//
// set
//
template<typename K, typename Pred, typename A>
HSIZE GetSerializeSize(const std::set<K, Pred, A>& m)
{
    HSIZE nSize = GetSizeOfCompactSize(m.size());
    for (typename std::set<K, Pred, A>::const_iterator it = m.begin(); it != m.end(); ++it)
        nSize += GetSerializeSize((*it));
    return nSize;
}

template<typename Stream, typename K, typename Pred, typename A>
void Serialize(Stream& os, const std::set<K, Pred, A>& m)
{
    WriteCompactSize(os, m.size());
    for (typename std::set<K, Pred, A>::const_iterator it = m.begin(); it != m.end(); ++it)
        Serialize(os, (*it));
}

template<typename Stream, typename K, typename Pred, typename A>
void Unserialize(Stream& is, std::set<K, Pred, A>& m)
{
    m.clear();
    unsigned int nSize = ReadCompactSize(is);
    typename std::set<K, Pred, A>::iterator it = m.begin();
    for (unsigned int i = 0; i < nSize; i++)
    {
        K key;
        Unserialize(is, key);
        it = m.insert(it, key);
    }
}



//
// Support for IMPLEMENT_SERIALIZE and READWRITE macro
//
struct tag_streamplaceholder { };
struct tag_SerActionGetSerializeSize { };
struct tag_SerActionSerialize { };
struct tag_SerActionUnserialize { };

template<typename Stream, typename T>
inline HSIZE SerReadWrite(Stream& , const T& obj, const tag_SerActionGetSerializeSize&) {
    return HUICPP::GetSerializeSize(obj);
}

template<typename Stream, typename T>
inline HSIZE SerReadWrite(Stream& s, const T& obj, const tag_SerActionSerialize&) {
    HUICPP::Serialize(s, obj);
    return 0;
}

template<typename Stream, typename T>
inline HSIZE SerReadWrite(Stream& s, T& obj, const tag_SerActionUnserialize&) {
    HUICPP::Unserialize(s, obj);
    return 0;
}


class CSizeComputer
{
protected:
    size_t nSize;

public:
    CSizeComputer() : nSize(0) {}

    CSizeComputer& write(const char *, size_t nSize)
    {
        this->nSize += nSize;
        return *this;
    }

    template<typename T>
    CSizeComputer& operator<<(const T& obj)
    {
        HUICPP::Serialize(*this, obj);
        return (*this);
    }

    size_t size() const {
        return nSize;
    }
};

/** Double ended buffer combining vector and stream-like interfaces.
 *
 * >> and << read and write unformatted data using the above serialization templates.
 * Fills with data in linear time; some stringstream implementations take N^2 time.
 */

class CDataStream
{
protected:
    typedef std::vector<char> CSerializeData;
    typedef CSerializeData vector_type;
    vector_type vch;
    unsigned int nReadPos;
    short state;
    short exceptmask;
public:
    typedef vector_type::allocator_type   allocator_type;
    typedef vector_type::size_type        size_type;
    typedef vector_type::difference_type  difference_type;
    typedef vector_type::reference        reference;
    typedef vector_type::const_reference  const_reference;
    typedef vector_type::value_type       value_type;
    typedef vector_type::iterator         iterator;
    typedef vector_type::const_iterator   const_iterator;
    typedef vector_type::reverse_iterator reverse_iterator;

    explicit CDataStream()
    {
        Init();
    }

    CDataStream(const_iterator pbegin, const_iterator pend) : vch(pbegin, pend)
    {
        Init();
    }

#if !defined(_MSC_VER) || _MSC_VER >= 1300
    CDataStream(const char* pbegin, const char* pend) 
    : vch(pbegin, pend)
    {
        Init();
    }
#endif

    CDataStream(const std::vector<char>& vchIn) 
    : vch(vchIn.begin(), vchIn.end())
    {
        Init();
    }

    CDataStream(const std::vector<unsigned char>& vchIn) 
    : vch((char*)&vchIn.begin()[0], (char*)&vchIn.end()[0])
    {
        Init();
    }

    void Init()
    {
        nReadPos = 0;
        state = 0;

    }

    CDataStream& operator+=(const CDataStream& b)
    {
        vch.insert(vch.end(), b.begin(), b.end());
        return *this;
    }

    friend CDataStream operator+(const CDataStream& a, const CDataStream& b)
    {
        CDataStream ret = a;
        ret += b;
        return (ret);
    }

    std::string str() const
    {
        return (std::string(begin(), end()));
    }


    //
    // Vector subset
    //
    const_iterator begin() const                     { return vch.begin() + nReadPos; }
    iterator begin()                                 { return vch.begin() + nReadPos; }
    const_iterator end() const                       { return vch.end(); }
    iterator end()                                   { return vch.end(); }
    size_type size() const                           { return vch.size() - nReadPos; }
    bool empty() const                               { return vch.size() == nReadPos; }
    void resize(size_type n, value_type c=0)         { vch.resize(n + nReadPos, c); }
    void reserve(size_type n)                        { vch.reserve(n + nReadPos); }
    const_reference operator[](size_type pos) const  { return vch[pos + nReadPos]; }
    reference operator[](size_type pos)              { return vch[pos + nReadPos]; }
    void clear()                                     { vch.clear(); nReadPos = 0; }
    iterator insert(iterator it, const char& x=char()) { return vch.insert(it, x); }
    void insert(iterator it, size_type n, const char& x) { vch.insert(it, n, x); }

    void insert(iterator it, std::vector<char>::const_iterator first, std::vector<char>::const_iterator last)
    {
        if (it == vch.begin() + nReadPos && (unsigned int)(last - first) <= nReadPos)
        {
            // special case for inserting at the front when there's room
            nReadPos -= (last - first);
            memcpy(&vch[nReadPos], &first[0], last - first);
        }
        else
            vch.insert(it, first, last);
    }

#if !defined(_MSC_VER) || _MSC_VER >= 1300
    void insert(iterator it, const char* first, const char* last)
    {
        if (it == vch.begin() + nReadPos && (unsigned int)(last - first) <= nReadPos)
        {
            // special case for inserting at the front when there's room
            nReadPos -= (last - first);
            memcpy(&vch[nReadPos], &first[0], last - first);
        }
        else
            vch.insert(it, first, last);
    }
#endif

    iterator erase(iterator it)
    {
        if (it == vch.begin() + nReadPos)
        {
            // special case for erasing from the front
            if (++nReadPos >= vch.size())
            {
                // whenever we reach the end, we take the opportunity to clear the buffer
                nReadPos = 0;
                return vch.erase(vch.begin(), vch.end());
            }
            return vch.begin() + nReadPos;
        }
        else
            return vch.erase(it);
    }

    iterator erase(iterator first, iterator last)
    {
        if (first == vch.begin() + nReadPos)
        {
            // special case for erasing from the front
            if (last == vch.end())
            {
                nReadPos = 0;
                return vch.erase(vch.begin(), vch.end());
            }
            else
            {
                nReadPos = (last - vch.begin());
                return last;
            }
        }
        else
            return vch.erase(first, last);
    }

    inline void Compact()
    {
        vch.erase(vch.begin(), vch.begin() + nReadPos);
        nReadPos = 0;
    }

    bool Rewind(size_type n)
    {
        // Rewind by n characters if the buffer hasn't been compacted yet
        if (n > nReadPos)
            return false;
        nReadPos -= n;
        return true;
    }


    //
    // Stream subset
    //
    void setstate(short bits, const char* )
    {
        state |= bits;

    }

    bool eof() const             { return size() == 0; }

    bool good() const            { return !eof() && (state == 0); }
    void clear(short n)          { state = n; }  // name conflict with vector clear()
    short exceptions()           { return exceptmask; }
    short exceptions(short mask) { short prev = exceptmask; exceptmask = mask; setstate(0, "CDataStream"); return prev; }
    CDataStream* rdbuf()         { return this; }
    int in_avail()               { return size(); }

    CDataStream& read(char* pch, size_t nSize)
    {
        // Read from the beginning of the buffer
        unsigned int nReadPosNext = nReadPos + nSize;
        if (nReadPosNext >= vch.size())
        {
            if (nReadPosNext > vch.size())
            {

                memset(pch, 0, nSize);
                nSize = vch.size() - nReadPos;
            }
            memcpy(pch, &vch[nReadPos], nSize);
            nReadPos = 0;
            vch.clear();
            return (*this);
        }
        memcpy(pch, &vch[nReadPos], nSize);
        nReadPos = nReadPosNext;
        return (*this);
    }

    CDataStream& ignore(int nSize)
    {
        unsigned int nReadPosNext = nReadPos + nSize;
        if (nReadPosNext >= vch.size())
        {
            if (nReadPosNext > vch.size())

            nReadPos = 0;
            vch.clear();
            return (*this);
        }
        nReadPos = nReadPosNext;
        return (*this);
    }

    CDataStream& write(const char* pch, size_t nSize)
    {
        // Write to the end of the buffer
        vch.insert(vch.end(), pch, pch + nSize);
        return (*this);
    }

    template<typename Stream>
    void Serialize(Stream& s) const
    {
        // Special case: stream << stream concatenates like stream += stream
        if (!vch.empty())
            s.write((char*)&vch[0], vch.size() * sizeof(vch[0]));
    }

    template<typename T>
    unsigned int GetSerializeSize(const T& obj)
    {
        // Tells the size of the object if serialized to this stream
        return HUICPP::GetSerializeSize(obj);
    }

    template<typename T>
    CDataStream& operator<<(const T& obj)
    {
        // Serialize to this stream
        HUICPP::Serialize(*this, obj);
        return (*this);
    }

    template<typename T>
    CDataStream& operator>>(T& obj)
    {
        // Unserialize from this stream
        HUICPP::Unserialize(*this, obj);
        return (*this);
    }

    void GetAndClear(CSerializeData &data) {
        data.insert(data.end(), begin(), end());
        clear();
    }
};


/** RAII wrapper for FILE*.
 *
 * Will automatically close the file when it goes out of scope if not null.
 * If you're returning the file pointer, return file.release().
 * If you need to close the file early, use file.fclose() instead of fclose(file).
 */
class CAutoFile
{
protected:
    FILE* file;
    short state;
    short exceptmask;
public:
    CAutoFile(FILE* filenew)
    {
        file = filenew;
        state = 0;

    }

    ~CAutoFile()
    {
        fclose();
    }

    void fclose()
    {
        if (file != NULL && file != stdin && file != stdout && file != stderr)
            ::fclose(file);
        file = NULL;
    }

    FILE* release()             { FILE* ret = file; file = NULL; return ret; }
    operator FILE*()            { return file; }
    FILE* operator->()          { return file; }
    FILE& operator*()           { return *file; }
    FILE** operator&()          { return &file; }
    FILE* operator=(FILE* pnew) { return file = pnew; }
    bool operator!()            { return (file == NULL); }


    //
    // Stream subset
    //
    void setstate(short bits, const char* )
    {
        state |= bits;
    }


    bool good() const            { return state == 0; }
    void clear(short n = 0)      { state = n; }
    short exceptions()           { return exceptmask; }
    short exceptions(short mask) { short prev = exceptmask; exceptmask = mask; setstate(0, "CAutoFile"); return prev; }

    CAutoFile& read(char* pch, size_t nSize)
    {

	auto cb = fread(pch, 1, nSize, file);
	(void)cb;
        return (*this);
    }

    CAutoFile& write(const char* pch, size_t nSize)
    {

        auto cb = fwrite(pch, 1, nSize, file);
	(void)cb;

        return (*this);
    }

    template<typename T>
    unsigned int GetSerializeSize(const T& obj)
    {
        // Tells the size of the object if serialized to this stream
        return HUICPP::GetSerializeSize(obj);
    }

    template<typename T>
    CAutoFile& operator<<(const T& obj)
    {
        // Serialize to this stream

        HUICPP::Serialize(*this, obj);
        return (*this);
    }

    template<typename T>
    CAutoFile& operator>>(T& obj)
    {

        HUICPP::Unserialize(*this, obj);
        return (*this);
    }
};

}

#endif // __H_HUICPP_SERIALIZE_H__
