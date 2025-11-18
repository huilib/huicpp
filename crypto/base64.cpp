

#include <hutil.h>
#include "base64.h"

#include <openssl/bio.h>
#include <openssl/evp.h>
#include <openssl/buffer.h>


namespace HUICPP {

namespace CRYPTO {


HRET Base64::Encode(HCSTRR indata, HSTRR outdata) {

    outdata.clear();
    if(indata.empty()) { HRETURN_OK; }

    HSZ out_ptr = nullptr;
    HSIZE outlen = 0;

    HNOTOK_RETURN(base64_encode(static_cast<HCPTR>(&indata[0]), indata.size(), out_ptr, outlen));

    outdata.assign(static_cast<HSZ>(out_ptr), static_cast<HSZ>(out_ptr) + outlen);
    HFREE(out_ptr);

    HRETURN_OK;

}


HRET Base64::Encode(HCMEMR indata, HSTRR outdata) {

    outdata.clear();
    if(indata.empty()) { HRETURN_OK; }

    HSZ out_ptr = nullptr;
    HSIZE outlen = 0;

    HNOTOK_RETURN(base64_encode(static_cast<HCPTR>(&indata[0]), indata.size(), out_ptr, outlen));

    outdata.assign(static_cast<HSZ>(out_ptr), static_cast<HSZ>(out_ptr) + outlen);
    HFREE(out_ptr);

    HRETURN_OK;

}


HRET Base64::Decode(HCSTRR indata, HMEMR outdata) {

    outdata.clear();
    if(indata.empty()) { HRETURN_OK; }

    HPTR out_data = nullptr;
    HSIZE outlen = 0;
    HNOTOK_RETURN(base64_decode(static_cast<HCPTR>(&indata[0]), indata.size(), out_data, outlen));

    outdata.assign(static_cast<HUSZ>(out_data), static_cast<HUSZ>(out_data) + outlen);
    HFREE(out_data);

    HRETURN_OK;

}


HRET Base64::Decode(HCMEMR indata, HMEMR outdata) {

    outdata.clear();
    if(indata.empty()) { HRETURN_OK; }

    HPTR out_data = nullptr;
    HSIZE outlen = 0;
    HNOTOK_RETURN(base64_decode(static_cast<HCPTR>(&indata[0]), indata.size(), out_data, outlen));

    outdata.assign(static_cast<HUSZ>(out_data), static_cast<HUSZ>(out_data) + outlen);
    HFREE(out_data);

    HRETURN_OK;

}


HRET Base64::base64_encode(HCPTR indata, HSIZE inlen, HSZ & outdata, HSIZER outlen, bool newline) {

    HASSERT_RETURN(indata != nullptr and inlen > 0, INVL_PARA);

    BUF_MEM* bptr = nullptr;

    BIO* b64 = BIO_new(BIO_f_base64());
    if (not newline) {
        BIO_set_flags(b64, BIO_FLAGS_BASE64_NO_NL);
    }

    BIO* bmem = BIO_new(BIO_s_mem());
    b64 = BIO_push(b64, bmem);

    (void)BIO_write(b64, indata, inlen);
    (void)BIO_flush(b64);
    BIO_get_mem_ptr(b64, &bptr);

    outlen = bptr->length;
    outdata = static_cast<HSZ>(malloc(outlen));
    memcpy(outdata, bptr->data, outlen);
    
    BIO_free_all(b64);

    HRETURN_OK;

}


HRET Base64::base64_decode(HCPTR indata, HSIZE inlen, HPTR & outdata, HSIZER outlen, bool newline) {

    HASSERT_RETURN(indata != nullptr and inlen > 0, INVL_PARA);

    outlen = inlen;
    outdata = malloc(outlen);
    ZERO_MEM(outdata, outlen);

    BIO* b64 = BIO_new(BIO_f_base64());
    if (not newline) {
        BIO_set_flags(b64, BIO_FLAGS_BASE64_NO_NL);
    }

    BIO* bmem = BIO_new_mem_buf(indata, inlen);
    bmem = BIO_push(b64, bmem);
    outlen = BIO_read(bmem, outdata, outlen);

    BIO_free_all(bmem);

    HRETURN_OK;

}

}

}
