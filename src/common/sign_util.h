//
// Created by sukai on 2023/1/16.
//

#ifndef TIGERAPI_SIGN_UTIL_H
#define TIGERAPI_SIGN_UTIL_H

#include <vector>
#include <cpprest/details/basic_types.h>

using namespace std;
//
// Start of platform-dependent hmac_sha1() block...
//
#if defined(_WIN32) && !defined(__cplusplus_winrt) // Windows desktop

#include <bcrypt.h>
#include <winternl.h>
#include <Windows.h>
#include <openssl/hmac.h>
#include <openssl/pem.h>
#include <openssl/rsa.h>
#include <openssl/bio.h>
#include <openssl/err.h>
#pragma comment(lib, "bcrypt.lib")


RSA * create_rsa(utility::char_t *key, bool is_private) {
    RSA *rsa = nullptr;
    BIO *keybio = BIO_new_mem_buf(key, -1);
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
    unsigned char hash[SHA_DIGEST_LENGTH] = "";

    SHA1((const unsigned char*)context.c_str(), context.size(), hash);
    RSA *rsa = create_rsa((utility::char_t *)key.c_str(), true);
    int ret = RSA_sign(NID_sha1, hash, SHA_DIGEST_LENGTH,
                       encrypted, &encrypted_length, rsa);
    if (ret == 1) {
        utility::string_t s(reinterpret_cast<wchar_t*>(encrypted));
        return s;
    }
    return U("");
}

int sha1_verify(const utility::string_t& context, const utility::string_t& sign, const utility::string_t& key) {

    utility::char_t sigbuf[8196 * 16] = {};
    unsigned int siglen = 0;

    utility::string_t decoded = websocketpp::base64_decode(sign);
    memcpy(sigbuf, decoded.data(), decoded.size());
    siglen = decoded.size();

    unsigned char hash[SHA_DIGEST_LENGTH] = "";

    SHA1((const unsigned char*)context.c_str(), context.size(), hash);

    RSA *rsa = create_rsa((utility::char_t *)key.c_str(), false);

    int ret = RSA_verify(NID_sha1, hash, SHA_DIGEST_LENGTH,
                         reinterpret_cast<unsigned char*>(sigbuf), siglen, rsa);
    return ret;

}

#elif defined(_WIN32) && defined(__cplusplus_winrt) // Windows RT

using namespace Windows::Security::Cryptography;
using namespace Windows::Security::Cryptography::Core;
using namespace Windows::Storage::Streams;

std::vector<unsigned char> hmac_sha1(const utility::string_t& key, const utility::string_t& data)
{
    Platform::String ^ data_str = ref new Platform::String(data.c_str());
    Platform::String ^ key_str = ref new Platform::String(key.c_str());

    MacAlgorithmProvider ^ HMACSha1Provider = MacAlgorithmProvider::OpenAlgorithm(MacAlgorithmNames::HmacSha1);
    IBuffer ^ content_buffer = CryptographicBuffer::ConvertStringToBinary(data_str, BinaryStringEncoding::Utf8);
    IBuffer ^ key_buffer = CryptographicBuffer::ConvertStringToBinary(key_str, BinaryStringEncoding::Utf8);

    auto signature_key = HMACSha1Provider->CreateKey(key_buffer);
    auto signed_buffer = CryptographicEngine::Sign(signature_key, content_buffer);

    Platform::Array<unsigned char, 1> ^ arr;
    CryptographicBuffer::CopyToByteArray(signed_buffer, &arr);
    return std::vector<unsigned char>(arr->Data, arr->Data + arr->Length);
}


RSA * create_rsa(utility::char_t *key, bool is_private) {
    RSA *rsa = nullptr;
    BIO *keybio = BIO_new_mem_buf(key, -1);
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
    unsigned char hash[SHA_DIGEST_LENGTH] = U("");

    SHA1((const unsigned char*)context.c_str(), context.size(), hash);
    RSA *rsa = create_rsa((utility::char_t *)key.c_str(), true);
    int ret = RSA_sign(NID_sha1, hash, SHA_DIGEST_LENGTH,
                       encrypted, &encrypted_length, rsa);
    if (ret == 1) {
        utility::string_t s(reinterpret_cast<char*>(encrypted));
        return s;
    }
    return "";
}

int sha1_verify(const utility::string_t& context, const utility::string_t& sign, const utility::string_t& key) {

    utility::char_t sigbuf[8196 * 16] = {};
    unsigned int siglen = 0;

    utility::string_t decoded = websocketpp::base64_decode(sign);
    memcpy(sigbuf, decoded.data(), decoded.size());
    siglen = decoded.size();

    unsigned char hash[SHA_DIGEST_LENGTH] = U("");

    SHA1((const unsigned char*)context.c_str(), context.size(), hash);

    RSA *rsa = create_rsa((utility::char_t *)key.c_str(), false);

    int ret = RSA_verify(NID_sha1, hash, SHA_DIGEST_LENGTH,
                         reinterpret_cast<unsigned char*>(sigbuf), siglen, rsa);
    return ret;

}
#else // Linux, Mac OS X

#include <openssl/hmac.h>
#include <openssl/pem.h>
#include <openssl/rsa.h>
#include <openssl/bio.h>
#include <openssl/err.h>

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
    RSA *rsa = nullptr;
    BIO *keybio = BIO_new_mem_buf(key, -1);
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
    unsigned char hash[SHA_DIGEST_LENGTH] = U("");

    SHA1((const unsigned char*)context.c_str(), context.size(), hash);
    RSA *rsa = create_rsa((utility::char_t *)key.c_str(), true);
    int ret = RSA_sign(NID_sha1, hash, SHA_DIGEST_LENGTH,
                       encrypted, &encrypted_length, rsa);
    if (ret == 1) {
        utility::string_t s(reinterpret_cast<char*>(encrypted));
        return s;
    }
    return "";
}

int sha1_verify(const utility::string_t& context, const utility::string_t& sign, const utility::string_t& key) {

    utility::char_t sigbuf[8196 * 16] = {};
    unsigned int siglen = 0;

    utility::string_t decoded = websocketpp::base64_decode(sign);
    memcpy(sigbuf, decoded.data(), decoded.size());
    siglen = decoded.size();

    unsigned char hash[SHA_DIGEST_LENGTH] = U("");

    SHA1((const unsigned char*)context.c_str(), context.size(), hash);

    RSA *rsa = create_rsa((utility::char_t *)key.c_str(), false);

    int ret = RSA_verify(NID_sha1, hash, SHA_DIGEST_LENGTH,
                         reinterpret_cast<unsigned char*>(sigbuf), siglen, rsa);
    return ret;

}

#endif
//
// ...End of platform-dependent hmac_sha1() block.
//

#endif //TIGERAPI_SIGN_UTIL_H
