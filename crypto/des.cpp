


#include "des.h"
#include <htodoholder.h>

#include <openssl/des.h>
#include <openssl/bio.h>
#include <openssl/evp.h>
#include <openssl/buffer.h>
 

namespace HUICPP {

namespace CRYPTO {


HRET Des3::Encrypt (HCSTRR src, HMEMR dst, HCSTRR passwd) {

    dst.clear();
    HN outlen = static_cast<HN>(src.size() * 2);
    HUSZ outptr = static_cast<HUSZ>(malloc(outlen));
    HSCOPE_EXIT { HFREE(outptr); };

    HNOTOK_RETURN(des3_encrypt(reinterpret_cast<HCUSZ>(src.c_str()), 
       static_cast<HN>(src.size()), 
       outptr, outlen, passwd));

    dst.assign(outptr, outptr + outlen);
    HRETURN_OK;

}


HRET Des3::Encrypt (HMEMR src, HMEMR dst, HCSTRR passwd) {
    dst.clear();
    HN outlen = static_cast<HN>(src.size() * 2);
    HUSZ outptr = static_cast<HUSZ>(malloc(outlen));
    HSCOPE_EXIT { HFREE(outptr); };

    HNOTOK_RETURN(des3_encrypt(&src[0], static_cast<HN>(src.size()), 
       outptr, outlen, passwd));

    dst.assign(outptr, outptr + outlen);
    HRETURN_OK;
}


HRET Des3::Decrypt (HMEMR src, HMEMR dst, HCSTRR passwd) {

    dst.clear();
    HN outlen = static_cast<HN>(src.size() * 2);
    HUSZ outptr = static_cast<HUSZ>(malloc(outlen));
    HSCOPE_EXIT { HFREE(outptr); };

    HNOTOK_RETURN(des3_decrypt(&src[0], static_cast<HN>(src.size()), 
       outptr, outlen, passwd));

    dst.assign(outptr, outptr + outlen);
    HRETURN_OK;

}


HRET Des3::des3_encrypt (HCUSZ in, HN inlen, HUSZ out, HNR outlen, HCSTRR password) {
    
    const EVP_CIPHER * cipher = EVP_des_ede3_ecb( );
    HASSERT_THROW(cipher != nullptr, DEP_ERROR);

    unsigned char key[EVP_MAX_KEY_LENGTH];
    unsigned char iv[EVP_MAX_KEY_LENGTH];

    EVP_BytesToKey(cipher, EVP_md5(), nullptr, reinterpret_cast<HCUSZ>(password.c_str()), password.size(), 
        1, key, iv);

    EVP_CIPHER_CTX* ctx = EVP_CIPHER_CTX_new();
    EVP_CIPHER_CTX_init(ctx);

    HSCOPE_EXIT {
        EVP_CIPHER_CTX_cleanup(ctx);
        EVP_CIPHER_CTX_free(ctx);
    };

    EVP_CIPHER_CTX_set_padding(ctx,0);

    int rv = EVP_EncryptInit_ex(ctx, cipher, nullptr, key, iv);
    HASSERT_RETURN(rv == 1, DEP_ERROR);

    rv = EVP_EncryptUpdate(ctx, out, &outlen, in, inlen);
    HASSERT_RETURN(rv == 1, DEP_ERROR);

    HN templen = 0;
    rv = EVP_EncryptFinal_ex(ctx, out + outlen, &templen);
    HASSERT_RETURN(rv == 1, DEP_ERROR);
    outlen += templen;

    HRETURN_OK;
}


HRET Des3::des3_decrypt (HCUSZ in, HN inlen, HUSZ out, HNR outlen, HCSTRR password) {

    const EVP_CIPHER * cipher = EVP_des_ede3_ecb( );
    HASSERT_THROW(cipher != nullptr, DEP_ERROR);

    unsigned char key[EVP_MAX_KEY_LENGTH];
    unsigned char iv[EVP_MAX_KEY_LENGTH];

    EVP_BytesToKey(cipher, EVP_md5(), nullptr, reinterpret_cast<HCUSZ>(password.c_str()), 
        password.size(), 1, key, iv);

    EVP_CIPHER_CTX* ctx = EVP_CIPHER_CTX_new();
    EVP_CIPHER_CTX_init(ctx);

    HSCOPE_EXIT {
        EVP_CIPHER_CTX_cleanup(ctx);
        EVP_CIPHER_CTX_free(ctx);
    };

    int rv = EVP_DecryptInit_ex(ctx, cipher, nullptr, key, iv);
    HASSERT_RETURN(rv == 1, DEP_ERROR);

    rv = EVP_DecryptUpdate(ctx, out, &outlen, in, inlen);
    HASSERT_RETURN(rv == 1, DEP_ERROR);

    HN templen = 0;
    rv = EVP_DecryptFinal_ex(ctx, out + outlen, &templen);
    HASSERT_RETURN(rv == 1, DEP_ERROR);

    outlen += templen;

    HRETURN_OK;
}




}


}
