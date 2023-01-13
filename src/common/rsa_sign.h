
#ifndef TIGERAPI_RSA_SIGN_H
#define TIGERAPI_RSA_SIGN_H

#pragma once

#include <openssl/pem.h>
#include <openssl/rsa.h>
#include <openssl/bio.h>
#include <openssl/err.h>
#include <iostream>
#include <stdio.h>

using namespace std;

class Sha1RSASign {
public:
    Sha1RSASign();

    ~Sha1RSASign();

    void print_last_error(const char *msg);

    RSA *create_rsa(utility::char_t *key, bool is_private);

    /*
        shar1 rsa sign from private key context
    */
    int sha1_encrypt(utility::char_t *context, int context_length, utility::char_t *key, utility::char_t *encrypted,
                     unsigned int *encrypted_length);

    int sha1_decrypt(utility::char_t *encrypted, unsigned int encrypted_length, utility::char_t *key,
                     utility::char_t *decrypted, unsigned int decrypted_length);

    /*
        shar1 rsa sign from private key file
    */
    int build_sha1_rsa_sign(char *filename, utility::char_t *src, int srclen, utility::char_t *sign, unsigned int *signlen);

    /*
        encrypt decrypt rsa sign,
    */
    int encrypt(utility::char_t *data, int data_len, utility::char_t *key, utility::char_t *encrypted);

    int decrypt(utility::char_t *context, int contextLength, utility::char_t *key, utility::char_t *decrypted);


};

Sha1RSASign::Sha1RSASign() {
}

Sha1RSASign::~Sha1RSASign() {
}

RSA *Sha1RSASign::create_rsa(utility::char_t *key, bool is_private) {
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
        print_last_error(U("Failed to create key BIO"));
    }

    if (rsa == nullptr) {
        print_last_error(U("Failed to create RSA"));
    }

    return rsa;
}

int Sha1RSASign::sha1_encrypt(utility::char_t *context, int context_length, utility::char_t *key, utility::char_t *encrypted,
                              unsigned int *encrypted_length) {
    RSA *rsa = create_rsa(key, true);
    int ret = -1;
    if (rsa != nullptr) {
        // 用hash作为签名内容
        utility::char_t hash[SHA_DIGEST_LENGTH] = U("");
        SHA1(context, context_length, hash);
        ret = RSA_sign(NID_sha1, hash, SHA_DIGEST_LENGTH, encrypted, encrypted_length, rsa);
        if (1 != ret) {
            cout << U("sha1 rsa encrypt sign error, result is ") << ret << U(", encrypted len is ") << *encrypted_length
                 << endl;

        }
        RSA_free(rsa);
    }
    return ret;
}

int Sha1RSASign::sha1_decrypt(utility::char_t *encrypted, unsigned int encrypted_length, utility::char_t *key,
                              utility::char_t *decrypted, unsigned int decrypted_length) {
    RSA *rsa = create_rsa(key, false);
    int ret = -1;
    if (rsa != nullptr) {
        // 先取hash再验签
        utility::char_t hash[SHA_DIGEST_LENGTH] = U("");
        SHA1(decrypted, decrypted_length, hash);
        ret = RSA_verify(NID_sha1, hash, SHA_DIGEST_LENGTH, encrypted, encrypted_length, rsa);
        if (1 != ret) {
            cout << U("sha1 rsa decrypt verify error, result is ") << ret << endl;
        }
        RSA_free(rsa);
    }
    return ret;
}


void Sha1RSASign::print_last_error(const char *msg) {
    char *err = new char[130];
    ERR_load_crypto_strings();
    ERR_error_string(ERR_get_error(), err);
    // printf_s(U("%s ERROR: %s\n"), msg, err);
    cout << msg << U(" ERROR: ") << err << endl;
    delete[]err;
}

int Sha1RSASign::build_sha1_rsa_sign(char *filename, utility::char_t *src, int srclen, utility::char_t *sign,
                                     unsigned int *signlen) {
    int ret = 0, isrclen = 0, ideslen = 0, i = 0, n = 0, ienclen = 0, rsalen = 0;

    if (filename == NULL || src == NULL || sign == NULL)
        return -1;

    utility::char_t hash[SHA_DIGEST_LENGTH] = U("");
    //unsigned sign_len = sizeof( sign );
    RSA *rsa_pri_key = RSA_new();

    FILE *fp = NULL;
    // int errorCode = fopen(&fp, filename, U("rt"));
    fp = fopen(filename, U("rt"));

    if (fp) {
        RSA_free(rsa_pri_key);
        cout << U("fopen ") << filename << U(" error ") << endl;
        return -2;
    }
    rsa_pri_key = PEM_read_RSAPrivateKey(fp, &rsa_pri_key, NULL, NULL);
    if (rsa_pri_key == NULL) {
        RSA_free(rsa_pri_key);
        fclose(fp);
        printf(U("PEM_read_RSAPrivateKey error\n"));
        return -3;
    }
    fclose(fp);
    SHA1(src, srclen, hash);
    ret = RSA_sign(NID_sha1, hash, SHA_DIGEST_LENGTH, sign, (unsigned int *) signlen, rsa_pri_key);
    if (1 != ret) {
        RSA_free(rsa_pri_key);
        cout << U("RSA_sign err, r is ") << ret << U(", sign_len is ") << *signlen << endl;
        return -4;
    }
    RSA_free(rsa_pri_key);
    return 0;
}

int Sha1RSASign::encrypt(utility::char_t *data, int data_len, utility::char_t *key, utility::char_t *encrypted) {
    RSA *rsa = create_rsa(key, true);
    int result = RSA_private_encrypt(data_len, data, encrypted, rsa, RSA_PKCS1_PADDING);
    return result;
}

int Sha1RSASign::decrypt(utility::char_t *context, int contextLength, utility::char_t *key, utility::char_t *decrypted) {
    RSA *rsa = create_rsa(key, false);
    int result = RSA_public_decrypt(contextLength, context, decrypted, rsa, RSA_PKCS1_PADDING);
    RSA_free(rsa);
    return result;
}

#endif