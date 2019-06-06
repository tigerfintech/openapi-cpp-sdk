#pragma once

#include "pch.h"
#include <openssl/pem.h>
#include <openssl/rsa.h>
#include <openssl/bio.h>
#include <openssl/err.h>
#include <iostream>

class Sha1RSASign
{
public:
	Sha1RSASign();
	~Sha1RSASign();

	void printLastError(const char *msg);
	RSA * createRSA(unsigned char * key, bool isPrivate);
	/*
		shar1 rsa sign from private key context
	*/
	int sha1Encrypt(unsigned char * context, int contextLength, unsigned char * key, unsigned char *encrypted, unsigned int* encryptedLength);

	/*
		shar1 rsa sign from private key file
	*/
	int buildSha1RsaSign(char *filename, unsigned char *src, int srclen, unsigned char *sign, unsigned int *signlen);

	/*
		encrypt decryp rsa sign,
	*/
	int encrypt(unsigned char * data, int data_len, unsigned char * key, unsigned char *encrypted);
	int decrypt(unsigned char * context, int contextLength, unsigned char * key, unsigned char *decrypted);


};

Sha1RSASign::Sha1RSASign()
{
}

Sha1RSASign::~Sha1RSASign()
{
}

RSA * Sha1RSASign::createRSA(unsigned char * key, bool isPrivate)
{
	RSA *rsa = nullptr;
	BIO *keybio = BIO_new_mem_buf(key, -1);
	if (keybio != nullptr) {
		if (isPrivate) {
			rsa = PEM_read_bio_RSAPrivateKey(keybio, &rsa, 0, 0);
		}
		else {
			rsa = PEM_read_bio_RSA_PUBKEY(keybio, &rsa, 0, 0);
		}
		BIO_free_all(keybio);
	}
	else {
		printLastError("Failed to create key BIO");
	}

	if (rsa == nullptr) {
		printLastError("Failed to create RSA");
	}

	return rsa;
}

int Sha1RSASign::sha1Encrypt(unsigned char * context, int contextLength, unsigned char * key, unsigned char *encrypted, unsigned int* encryptedLength)
{
	RSA * rsa = createRSA(key, true);
	int ret = -1;
	if (rsa != nullptr)
	{
		unsigned char hash[SHA_DIGEST_LENGTH] = "";
		SHA1(context, contextLength, hash);
		ret = RSA_sign(NID_sha1, hash, SHA_DIGEST_LENGTH, encrypted, encryptedLength, rsa);
		if (1 != ret)
		{
			printf_s("shal rsa encrypt sign error,result is [%d], encrypted length is [%d]\n", ret, *encryptedLength);
		}
		RSA_free(rsa);
	}
	return ret;
}

void Sha1RSASign::printLastError(const char *msg)
{
	char * err = new char[130];
	ERR_load_crypto_strings();
	ERR_error_string(ERR_get_error(), err);
	printf_s("%s ERROR: %s\n", msg, err);
	delete []err;
}

int Sha1RSASign::buildSha1RsaSign(char *filename, unsigned char *src, int srclen, unsigned char *sign, unsigned int *signlen) {
	int ret = 0, isrclen = 0, ideslen = 0, i = 0, n = 0, ienclen = 0, rsalen = 0;

	if (filename == NULL || src == NULL || sign == NULL)
		return -1;

	unsigned char hash[SHA_DIGEST_LENGTH] = "";
	//unsigned sign_len = sizeof( sign );
	RSA *rsa_pri_key = RSA_new();

	FILE *fp = NULL;
	int errorCode = fopen_s(&fp, filename, "rt");
	if (errorCode != 0)
	{
		RSA_free(rsa_pri_key);
		printf("fopen [%s] error:[%d]\n", filename, errorCode);
		return -2;
	}
	rsa_pri_key = PEM_read_RSAPrivateKey(fp, &rsa_pri_key, NULL, NULL);
	if (rsa_pri_key == NULL)
	{
		RSA_free(rsa_pri_key);
		fclose(fp);
		printf("PEM_read_RSAPrivateKey error\n");
		return -3;
	}
	fclose(fp);
	SHA1(src, srclen, hash);
	ret = RSA_sign(NID_sha1, hash, SHA_DIGEST_LENGTH, sign, (unsigned int *)signlen, rsa_pri_key);
	if (1 != ret)
	{
		RSA_free(rsa_pri_key);
		printf("RSA_sign err,r is [%d],sign_len is [%d]\n", ret, *signlen);
		return -4;
	}
	RSA_free(rsa_pri_key);
	return 0;
}

int Sha1RSASign::encrypt(unsigned char * data, int data_len, unsigned char * key, unsigned char *encrypted) {
	RSA * rsa = createRSA(key, true);
	int result = RSA_private_encrypt(data_len, data, encrypted, rsa, RSA_PKCS1_PADDING);
	return result;
}
int Sha1RSASign::decrypt(unsigned char * context, int contextLength, unsigned char * key, unsigned char *decrypted)
{
	RSA * rsa = createRSA(key, false);
	int  result = RSA_public_decrypt(contextLength, context, decrypted, rsa, RSA_PKCS1_PADDING);
	RSA_free(rsa);
	return result;
}

