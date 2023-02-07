//
// Created by sukai on 2023/1/16.
//
#ifndef TIGERAPI_SIGN_UTIL_H
#define TIGERAPI_SIGN_UTIL_H

#include <vector>
#include <openssl/hmac.h>
#include <openssl/pem.h>
#include <openssl/rsa.h>
#include <openssl/bio.h>
#include <openssl/err.h>
#include <cpprest/details/basic_types.h>

using namespace std;
#pragma warning (disable: 4996)

std::vector<unsigned char> hmac_sha1(const utility::string_t& key, const utility::string_t& data)
{
    unsigned char digest[EVP_MAX_MD_SIZE];
    unsigned int digest_len = 0;

    HMAC(EVP_sha1(),
         key.c_str(),
         static_cast<int>(key.length()),
         (const unsigned char*)data.c_str(),
         data.length(),
         digest,
         &digest_len);

    return std::vector<unsigned char>(digest, digest + digest_len);
}


RSA * create_rsa(utility::char_t *key, bool is_private) {
    RSA* rsa = nullptr;
#if defined(WIN32) || defined(_WIN32) || defined(__WIN32__) || defined(__NT__)
    auto str1 = utility::conversions::utf16_to_utf8(key);
    BIO* keybio = BIO_new_mem_buf(str1.c_str(), -1);
#else
    auto str1 = key;
    BIO* keybio = BIO_new_mem_buf(str1, -1);
#endif
    if (keybio != nullptr) {
        if (is_private) {
            rsa = PEM_read_bio_RSAPrivateKey(keybio, nullptr, 0, 0);
        } else {
            rsa = PEM_read_bio_RSA_PUBKEY(keybio, nullptr, 0, 0);
        }
        BIO_free_all(keybio);
    } else {
        ucout << U("Failed to create key BIO");
    }

    if (rsa == nullptr) {
        ucout << U("Failed to create RSA");
    }

    return rsa;
}

utility::string_t sha1_sign(const utility::string_t& context, const utility::string_t& key) {
    unsigned char encrypted[8196 * 16] = {};
    unsigned int encrypted_length;
    unsigned char hash[SHA_DIGEST_LENGTH] = { 0 };
    
#if defined(WIN32) || defined(_WIN32) || defined(__WIN32__) || defined(__NT__)
    auto str_transfer_content = utility::conversions::utf16_to_utf8(context);
#else
    auto str_transfer_content = context;
#endif
    int context_size = str_transfer_content.size();
    SHA1((const unsigned char*)str_transfer_content.c_str(), context_size, hash);
    RSA *rsa = create_rsa((utility::char_t *)key.c_str(), true);
    int ret = RSA_sign(NID_sha1, hash, SHA_DIGEST_LENGTH,
                       encrypted, &encrypted_length, rsa);
    if (ret == 1) {
        utility::string_t s(encrypted, encrypted + encrypted_length);
        return s;
    }
    return U("");
}

int sha1_verify(const utility::string_t& context, const utility::string_t& sign, const utility::string_t& key) {

    utility::char_t sigbuf[8196 * 16] = {};
    unsigned int siglen = 0;

    utility::string_t decoded = TIGER_API::base64_decode(sign);
    memcpy(sigbuf, decoded.data(), decoded.size());
    siglen = decoded.size();

    unsigned char hash[SHA_DIGEST_LENGTH] = { 0 };

    SHA1((const unsigned char*)context.c_str(), context.size(), hash);

    RSA *rsa = create_rsa((utility::char_t *)key.c_str(), false);

    int ret = RSA_verify(NID_sha1, hash, SHA_DIGEST_LENGTH,
                         reinterpret_cast<unsigned char*>(sigbuf), siglen, rsa);
    return ret;

}

#endif //TIGERAPI_SIGN_UTIL_H
