#include <gtest/gtest.h>
#include "tigerapi/utils.h"
#include <openssl/rsa.h>
#include <openssl/pem.h>
#include <openssl/evp.h>
#include <string>

using namespace TIGER_API;

// Helper: generate a temporary RSA key pair for testing
struct RsaKeyPair {
    std::string private_key_pem;
    std::string public_key_pem;
};

static RsaKeyPair generate_rsa_key_pair() {
    RsaKeyPair result;
    EVP_PKEY* pkey = EVP_PKEY_new();
    EVP_PKEY_CTX* ctx = EVP_PKEY_CTX_new_id(EVP_PKEY_RSA, nullptr);
    EVP_PKEY_keygen_init(ctx);
    EVP_PKEY_CTX_set_rsa_keygen_bits(ctx, 2048);
    EVP_PKEY_keygen(ctx, &pkey);
    EVP_PKEY_CTX_free(ctx);

    // Write private key to PEM string
    BIO* priv_bio = BIO_new(BIO_s_mem());
    PEM_write_bio_PrivateKey(priv_bio, pkey, nullptr, nullptr, 0, nullptr, nullptr);
    char* priv_data;
    long priv_len = BIO_get_mem_data(priv_bio, &priv_data);
    result.private_key_pem.assign(priv_data, priv_len);
    BIO_free(priv_bio);

    // Write public key to PEM string
    BIO* pub_bio = BIO_new(BIO_s_mem());
    PEM_write_bio_PUBKEY(pub_bio, pkey);
    char* pub_data;
    long pub_len = BIO_get_mem_data(pub_bio, &pub_data);
    result.public_key_pem.assign(pub_data, pub_len);
    BIO_free(pub_bio);

    EVP_PKEY_free(pkey);
    return result;
}

// ── get_sign / verify_sign ────────────────────────────────────────────────────

TEST(UtilsSignTest, GetSign_ProducesNonEmptySignature) {
    RsaKeyPair keys = generate_rsa_key_pair();
    utility::string_t content = U("test content for signing");
    utility::string_t signature = Utils::get_sign(
        Utils::str8to16(keys.private_key_pem), content);
    EXPECT_FALSE(signature.empty());
}

TEST(UtilsSignTest, VerifySign_ValidSignature_ReturnsTrue) {
    RsaKeyPair keys = generate_rsa_key_pair();
    utility::string_t content = U("hello world");
    utility::string_t signature = Utils::get_sign(
        Utils::str8to16(keys.private_key_pem), content);
    bool result = Utils::verify_sign(
        Utils::str8to16(keys.public_key_pem), content, signature);
    EXPECT_TRUE(result);
}

TEST(UtilsSignTest, VerifySign_WrongContent_ReturnsFalse) {
    RsaKeyPair keys = generate_rsa_key_pair();
    utility::string_t content = U("hello world");
    utility::string_t signature = Utils::get_sign(
        Utils::str8to16(keys.private_key_pem), content);
    bool result = Utils::verify_sign(
        Utils::str8to16(keys.public_key_pem), U("different content"), signature);
    EXPECT_FALSE(result);
}

TEST(UtilsSignTest, VerifySign_WrongKey_ReturnsFalse) {
    RsaKeyPair keys1 = generate_rsa_key_pair();
    RsaKeyPair keys2 = generate_rsa_key_pair();
    utility::string_t content = U("test content");
    utility::string_t signature = Utils::get_sign(
        Utils::str8to16(keys1.private_key_pem), content);
    // Verify with different key pair
    bool result = Utils::verify_sign(
        Utils::str8to16(keys2.public_key_pem), content, signature);
    EXPECT_FALSE(result);
}

TEST(UtilsSignTest, GetSign_RsaCreationFailed_Throws) {
    // Invalid private key should cause RSA creation to fail
    EXPECT_THROW(Utils::get_sign(U("invalid_key_data"), U("content")),
                 std::runtime_error);
}

// ── fill_private_key_marker (BIO/DER detection) ────────────────────────────────

TEST(UtilsSignTest, FillPrivateKeyMarker_BareBase64_WrapsAsRsa) {
    // A bare base64 string (not PEM) should be wrapped as RSA PRIVATE KEY
    utility::string_t bare = U("dGVzdA==0000000000000000000000000000000000000000000000000");
    utility::string_t result = Utils::fill_private_key_marker(bare);
    // Should have BEGIN RSA PRIVATE KEY or BEGIN PRIVATE KEY header
    // (string intentionally kept whole — the marker is a PEM format label, not a real key)
    bool has_rsa = result.find(U("BEGIN RSA PRIVATE KEY")) != utility::string_t::npos;
    bool has_pkcs8 = result.find(U("BEGIN PRIVATE KEY")) != utility::string_t::npos;
    EXPECT_TRUE(has_rsa || has_pkcs8);
}

TEST(UtilsSignTest, FillPrivateKeyMarker_EmptyString_WrapsAsRsa) {
    utility::string_t result = Utils::fill_private_key_marker(U(""));
    EXPECT_NE(result.find(U("BEGIN")), utility::string_t::npos);
    EXPECT_NE(result.find(U("END")), utility::string_t::npos);
}

TEST(UtilsSignTest, FillPrivateKeyMarker_WithRealPrivateKey_PreservesHeader) {
    RsaKeyPair keys = generate_rsa_key_pair();
    utility::string_t pem = Utils::str8to16(keys.private_key_pem);
    utility::string_t result = Utils::fill_private_key_marker(pem);
    // Should have BEGIN PRIVATE KEY (PKCS#8 format from OpenSSL)
    // (string kept whole — PEM format label, not a real key)
    EXPECT_NE(result.find(U("BEGIN PRIVATE KEY")), utility::string_t::npos);
    EXPECT_NE(result.find(U("END PRIVATE KEY")), utility::string_t::npos);
}

// ── fill_public_key_marker ─────────────────────────────────────────────────────

TEST(UtilsSignTest, FillPublicKeyMarker_WithRealPublicKey_PreservesHeader) {
    RsaKeyPair keys = generate_rsa_key_pair();
    utility::string_t pem = Utils::str8to16(keys.public_key_pem);
    utility::string_t result = Utils::fill_public_key_marker(pem);
    EXPECT_NE(result.find(U("BEGIN PUBLIC KEY")), utility::string_t::npos);
    EXPECT_NE(result.find(U("END PUBLIC KEY")), utility::string_t::npos);
}

TEST(UtilsSignTest, FillPublicKeyMarker_BareKey_WrapsKey) {
    utility::string_t bare = U("MIIBIjANBgkqhkiG9w0BAQEFAAOCAQ8AMIIBCgKCAQEA");
    utility::string_t result = Utils::fill_public_key_marker(bare);
    EXPECT_NE(result.find(U("BEGIN PUBLIC KEY")), utility::string_t::npos);
    EXPECT_NE(result.find(U("END PUBLIC KEY")), utility::string_t::npos);
}

// ── Sign and verify roundtrip ──────────────────────────────────────────────────

TEST(UtilsSignTest, SignVerifyRoundtrip_LongContent) {
    RsaKeyPair keys = generate_rsa_key_pair();
    utility::string_t content = U("{\"tiger_id\":\"12345\",\"method\":\"positions\",\"timestamp\":\"2024-01-15 10:30:00\",\"biz_content\":\"{\\\"account\\\":\\\"DU123\\\"}\"}");
    utility::string_t signature = Utils::get_sign(
        Utils::str8to16(keys.private_key_pem), content);
    EXPECT_FALSE(signature.empty());
    bool verified = Utils::verify_sign(
        Utils::str8to16(keys.public_key_pem), content, signature);
    EXPECT_TRUE(verified);
}

TEST(UtilsSignTest, SignVerifyRoundtrip_EmptyContent) {
    RsaKeyPair keys = generate_rsa_key_pair();
    utility::string_t content = U("");
    utility::string_t signature = Utils::get_sign(
        Utils::str8to16(keys.private_key_pem), content);
    EXPECT_FALSE(signature.empty());
    bool verified = Utils::verify_sign(
        Utils::str8to16(keys.public_key_pem), content, signature);
    EXPECT_TRUE(verified);
}
