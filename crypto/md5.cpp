



#include "md5.h"
#include <openssl/md5.h>
#include "hex.h"

#include <fstream>

namespace HUICPP {


namespace CRYPTO {

void Md5::MD5(HCSTRR indata, HMEMR outdata) {

    md5_buf_t outbuf;
    md5(indata, outbuf);
    outdata.clear();
    outdata.assign(outbuf, outbuf + MD5_BUF_LEN);

}


void Md5::MD5(HCMEMR indata, HMEMR outdata) {

    md5_buf_t outbuf;
    md5(indata, outbuf);
    outdata.clear();
    outdata.assign(outbuf, outbuf + MD5_BUF_LEN);

}


HSTR Md5::MD5(HCSTRR instr) {

    HMEM outmem;
    MD5(instr, outmem);
    return Hex::HexMem(outmem);

}


HSTR Md5::MD5(HCMEMR instr) {

    HMEM outmem;
    MD5(instr, outmem);
    return Hex::HexMem(outmem);

}


HSTR Md5::Md5File(HCSTRR strFileName) {

    std::ifstream ifs(strFileName, std::ios_base::in | std::ios_base::binary);
    if (ifs.fail()) { return ""; }
    HBLOCK_BUF buf = {0};
    MD5_CTX ctx;
    MD5_Init(&ctx);
    while(ifs.good()) {
        memset(buf, 0, HBLOCK_LEN);
        ifs.read(buf, HBLOCK_LEN);
        MD5_Update(&ctx, buf, ifs.gcount());
    }   
    md5_buf_t outbuf;
    MD5_Final(outbuf, &ctx);
    return Hex::HexStr(outbuf, MD5_BUF_LEN);
    
}


void Md5::md5(HCSTRR indata, md5_buf_t outbuf) {

    md5(reinterpret_cast<HCUSZ>(&indata[0]), indata.size(), outbuf);

}


void Md5::md5(HCMEMR indata, md5_buf_t outbuf) {

    md5(&indata[0], indata.size(), outbuf);

}


void Md5::md5(HCUSZ indata, HSIZE len, md5_buf_t outbuf) {

    (void)::MD5(indata, len, outbuf);

}


}


}
