

#include "hbacktrace.h"
#include <execinfo.h>
#include <htodoholder.h>
#include <sstream>
#include <cxxabi.h>

#ifdef __TEST__
#include <iostream>
#endif

using namespace HUICPP;

namespace {

static void ParseLinuxStyleLine(HCSTRR strLine, HSTRR strRes) {

    strRes.clear();

    if (strLine.length() < 3) {
        return ;
    }

#ifdef __TEST__
    std::cout << ">>> strLine: " << strLine << std::endl;
#endif 
    //
    // Try to parse the linux style backtrace line
    //
    // For example:
    // ./StackTraceTest(_Z16TemplateFunctionIiET_v+0x9) [0x402c87]
    //   |             |                          |   | |        |
    //  line      begin_name        begin_offset--+   | |        |
    //                                    end_offset--+ |        |
    //                                   begin_address--+        +--end_address
    //
    // Skip "./" prefix if exists
    std::size_t line_pos = 0;
    if ((strLine[0] == '.') and (strLine[1] == '/'))
    {
        line_pos = 2;
    }

    size_t begin_name_pos = strLine.find('(', line_pos);
    if (begin_name_pos == HSTR::npos) {
        return ;
    }
    HSTR module_name = strLine.substr(line_pos, begin_name_pos - line_pos);
#ifdef __TEST__    
    std::cout << "begin_name_pos: " << begin_name_pos << ", module_name: " << module_name << std::endl;
#endif 

    size_t end_offset_pos = strLine.find(')', begin_name_pos);
    if (end_offset_pos == HSTR::npos) {
        return ;
    }
    HSTR strFunAndAddr = strLine.substr(begin_name_pos + 1, end_offset_pos - begin_name_pos - 1);
#ifdef __TEST__
    std::cout << "end_offset_pos: " << end_offset_pos << ", strFunAndAddr: " << strFunAndAddr << std::endl;
#endif
    size_t begin_offset_pos = strFunAndAddr.find('+');
    if (begin_offset_pos == HSTR::npos) {
        return ;
    }
    HSTR strFunctionName = strFunAndAddr.substr(0, begin_offset_pos);
#ifdef __TEST__
    std::cout << "begin_offset_pos: " << begin_offset_pos << ", strFunctionName: " << strFunctionName << std::endl;
#endif 
    HSTR strFunctionOffset = strFunAndAddr.substr(begin_offset_pos + 1);
#ifdef __TEST__
    std::cout << "begin_offset_pos: " << begin_offset_pos << ", strFunctionOffset: " << strFunctionOffset << std::endl;
#endif
    size_t begin_address_pos = strLine.find('[', end_offset_pos);
    if (begin_address_pos == HSTR::npos) {
        return ;
    }
    size_t end_address_pos = strLine.find(']', begin_address_pos);
    if (end_address_pos == HSTR::npos) {
        return ;
    }
    HSTR strAddress = strLine.substr(begin_address_pos + 1, end_address_pos - begin_address_pos - 1);
#ifdef __TEST__
    std::cout << "begin_address_pos: " << begin_address_pos << ", end_address_pos: " << end_address_pos << ", strAddress: " << strAddress << std::endl;
#endif 
    int status = 0;
    char* res_full_name = abi::__cxa_demangle(strFunctionName.c_str(), nullptr, nullptr, &status);
    HSTR strFun;
    if (res_full_name != nullptr) {
        strFun.assign(res_full_name);
        HFREE(res_full_name);
    }
#ifdef __TEST__    
    std::cout << "strFun: " << strFun << std::endl;
#endif 
    // Before:
    // StackTraceTest!_Z16TemplateFunctionIiET_v!0x9! [0x402c87]
    //                                                |        |
    //                                 begin_address--+        +--end_address
    // After (! means nullptr):
    // StackTraceTest!_Z16TemplateFunctionIiET_v!0x9! !0x402c87!
    //                                                 |        |
    //                                  begin_address--+        +--end_address

	std::stringstream ss;
    ss << module_name << ":" << strFun << ":" << strFunctionOffset << ":" << strAddress;
    strRes = ss.str();

}

}

namespace HUICPP {

HBackTrace::HBackTrace() 
    : backtrace_baseclass(), m_lines() {

}



HBackTrace::~HBackTrace() {

}


HBackTrace& HBackTrace::SystemCall() {

    HPTR ptr_array[BT_LINES_COUNT];

    int res_line_count = ::backtrace(ptr_array, BT_LINES_COUNT);

    char ** res_data = backtrace_symbols(ptr_array, res_line_count);
    HSCOPE_EXIT { HFREE(res_data); };


    for (int i = 0; i < res_line_count; ++i) {

        HSTR strLine(res_data[i]);

        m_lines.push_back(strLine);

    }

    backtrace_baseclass::SetSystemCallReturn(0);

    return *this;
}


HSTR HBackTrace::ToString() const {

    std::stringstream ss;
    ss << "========StackTrack========" << std::endl;

    for (HVSTR::size_type i = 0; i < m_lines.size(); ++i) {
        HCSTRR strLine = m_lines[i];
        HSTR strStyleLine;
        ParseLinuxStyleLine(strLine, strStyleLine);
        ss << i << "===" << strStyleLine << "===" << std::endl;
    }

    return ss.str();




}


}

