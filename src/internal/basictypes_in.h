/**********************************************************
 * Created: Joseph Mar-26-2021
 * Description: 
 *      Define some basic types.
 * 
 **********************************************************/


#ifndef __HUICPP_BASIC_TYPES_IN_H__
#define __HUICPP_BASIC_TYPES_IN_H__


#include <string>
#include <vector>
#include <map>
#include <set>
#include <atomic>


namespace HUICPP {

    // base type, reference, pointer, const pointer.

    /* char basic type */
    typedef char HCH;
    typedef HCH& HCHR;
    typedef HCH* HSZ;
    typedef const HCH* HCSZ;

    /* unsigned char basic type */
    typedef unsigned char HUCH;
    typedef HUCH& HUCHR;
    typedef HUCH* HUSZ;
    typedef const HUCH* HCUSZ;

    /* short type basic type */
    typedef short HSN;
    typedef HSN& HSNR;
    typedef HSN* HSNP;
    typedef const HSN* HCSNP;

    /* unsigned short basic type */
    typedef unsigned short HUSN;
    typedef HUSN& HUSNR;
    typedef HUSN* HUSNP;
    typedef const HUSN* HCUSNP;

    /* int basic type */
    typedef int HN;
    typedef HN& HNR;
    typedef HN* HNP;
    typedef const HN* HCNP;

    /* unsigned int basic type */
    typedef unsigned int HUN;
    typedef HUN& HUNR;
    typedef HUN* HUNP;
    typedef const HUN* HCUNP;

    /* long basic type */
    typedef long HLN;
    typedef HLN& HLNR;
    typedef HLN* HLNP;
    typedef const HLN* HCLNP;
    
    /* unsigned long basic types */
    typedef unsigned long HULN;
    typedef HULN& HULNR;
    typedef HULN* HULNP;
    typedef const HULN* HCULNP;
    
    /* long long basic types */
    typedef long long HLL;
    typedef HLL& HLLR;
    typedef HLL* HLLP;
    typedef const HLL* HCLLP;

    /* unsigned long long basic types */
    typedef unsigned long long HULL;
    typedef HULL& HULLR;
    typedef HULL* HULLP;
    typedef const HULL* HCULLP;

    /* size basic types */
    typedef size_t HSIZE;
    typedef size_t& HSIZER;
    typedef size_t HSSIZE;
    typedef off_t HOFF;

    /* time and double basic types */
    typedef time_t HTIME;
    typedef float HFLD;
    typedef HFLD& HFLDR;
    typedef double HDBE;
    typedef HDBE& HDBER;

    /* string basic type */
    typedef std::string HSTR;
    typedef const HSTR HCSTR;
    typedef HSTR& HSTRR;
    typedef const HSTR& HCSTRR;

    /* std::map<string, string> */
    typedef std::map<HSTR, HSTR> HMSTR;
    typedef const HMSTR  HCMSTR;
    typedef HMSTR& HMSTRR;
    typedef const HMSTR& HCMSTRR;

    /* std::vector<string> */
    typedef std::vector<HSTR> HVSTR;
    typedef const HVSTR HCVSTR;
    typedef HVSTR& HVSTRR;
    typedef const HVSTR& HCVSTRR;

    /* std::set<string> */
    typedef std::set<HSTR> HSSTR;
    typedef const HSSTR HCSSTR;
    typedef HSSTR& HSSTRR;
    typedef const HSSTR& HCSSTRR;

    /* std::vector<char> */
    typedef std::vector<HCH> HBUF;
    typedef const HBUF HCBUF;
    typedef HBUF& HBUFR;
    typedef const HBUF& HCBUFR;

    /* std::vector<unsigned char> */
    typedef std::vector<HUCH> HMEM;
    typedef HMEM& HMEMR;
    typedef const HMEM HCMEM;
    typedef const HMEM& HCMEMR;

    /* std::pair<string, string> */
    typedef std::pair<HSTR, HSTR> HKV;
    typedef HKV& HKVR;
    typedef const HKV HCKV;
    typedef const HKV& HCKVR;

    /* std::vector<pair<string, string> > */
    typedef std::vector<HKV> HVKV;
    typedef const HVKV HCVKV;
    typedef HVKV& HVKVR;
    typedef const HVKV& HCVKVR;    

    // special types.
    typedef HN FD_T;
    typedef HN RET_T;
    typedef HN SYS_T;             // system type.
    typedef HN SYS_RET_T;         // syscall return type.(int)

    typedef int64_t HN64;
    typedef uint64_t HUN64;

    typedef void* HPTR;
    typedef const void* HCPTR;

    /* char array heap */
    typedef HCH HBUP[1];    

    using counter_t = std::atomic<HN>;
}


#endif //__HUICPP_BASIC_TYPES_IN_H__