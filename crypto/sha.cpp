

#include "sha.h"
#include <openssl/sha.h>
#include "hex.h"
#include <fstream>

namespace HUICPP {

namespace CRYPTO {

void Sha::Sha256(HCSTRR strInData, HMEMR outdata) {

    sha256_buf_t outbuf;
    sha256(strInData, outbuf);
    outdata.clear();
    outdata.assign(outbuf, outbuf + SHA256_BUF_LEN);

}


void Sha::Sha256(HCMEMR indata, HMEMR outdata) {

    sha256_buf_t outbuf;
    sha256(indata, outbuf);
    outdata.clear();
    outdata.assign(outbuf, outbuf + SHA256_BUF_LEN);

}


HSTR Sha::Sha256(HCSTRR instr) {

    HMEM outmem;
    Sha256(instr, outmem);
    return Hex::HexMem(outmem);

}


HSTR Sha::Sha256(HCMEMR instr) {

    HMEM outmem;
    Sha256(instr, outmem);
    return Hex::HexMem(outmem);

}


HSTR Sha::Sha256File(HCSTRR strFileName) {

    std::ifstream ifs(strFileName, std::ios_base::in | std::ios_base::binary);
    if (ifs.fail()) { return ""; }
    HBLOCK_BUF buf = {0};
    SHA256_CTX ctx;
    SHA256_Init(&ctx);
    while(ifs.good()) {
        memset(buf, 0, HBLOCK_LEN);
        ifs.read(buf, HBLOCK_LEN);
        SHA256_Update(&ctx, buf, ifs.gcount());
    }   
    sha256_buf_t outbuf;
    SHA256_Final(outbuf, &ctx);
    return Hex::HexStr(outbuf, SHA256_BUF_LEN);

}


void Sha::sha256(HCSTRR indata, sha256_buf_t outbuf) {

    sha256(reinterpret_cast<HCUSZ>(&indata[0]), indata.size(), outbuf);

}


void Sha::sha256(HCMEMR indata, sha256_buf_t outbuf) {

    sha256(&indata[0], indata.size(), outbuf);

}


void Sha::sha256(HCUSZ indata, HSIZE len, sha256_buf_t outbuf) {

    ::SHA256(indata, len, outbuf);

}


}

}