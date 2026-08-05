#include <gtest/gtest.h>
#include "tigerapi/utils.h"
#include <string>

using namespace TIGER_API;

// ── double_to_string ─────────────────────────────────────────────────────────

TEST(UtilsTest, DoubleToString_Precision1) {
    std::string s = Utils::str16to8(Utils::double_to_string(3.14159, 1));
    EXPECT_EQ(s, "3.1");
}

TEST(UtilsTest, DoubleToString_Precision2) {
    std::string s = Utils::str16to8(Utils::double_to_string(3.14159, 2));
    EXPECT_EQ(s, "3.14");
}

TEST(UtilsTest, DoubleToString_DefaultPrecision) {
    // Default precision=1
    std::string s = Utils::str16to8(Utils::double_to_string(10.5));
    EXPECT_EQ(s, "10.5");
}

TEST(UtilsTest, DoubleToString_WholeNumber) {
    std::string s = Utils::str16to8(Utils::double_to_string(3.0, 1));
    EXPECT_EQ(s, "3.0");
}

// ── str8to16 / str16to8 roundtrip ────────────────────────────────────────────

TEST(UtilsTest, Str8to16Roundtrip_ASCII) {
    std::string original = "hello world";
    utility::string_t wide = Utils::str8to16(original);
    std::string back = Utils::str16to8(wide);
    EXPECT_EQ(back, original);
}

TEST(UtilsTest, Str8to16Roundtrip_Empty) {
    std::string original;
    utility::string_t wide = Utils::str8to16(original);
    std::string back = Utils::str16to8(wide);
    EXPECT_EQ(back, original);
}

TEST(UtilsTest, Str8to16Roundtrip_AlphaNum) {
    std::string original = "AAPL_USD_123";
    std::string back = Utils::str16to8(Utils::str8to16(original));
    EXPECT_EQ(back, original);
}

// ── fill_private_key_marker ──────────────────────────────────────────────────

TEST(UtilsTest, FillPrivateKeyMarker_PreservesRsaHeader) {
    utility::string_t pem =
        U("-----BEGIN RSA PRIVATE KEY-----\nMIIEpAIBAAKCAQEA\n-----END RSA PRIVATE KEY-----");
    utility::string_t result = Utils::fill_private_key_marker(pem);
    // Header already present → returned unchanged
    EXPECT_NE(result.find(U("BEGIN RSA PRIVATE KEY")), utility::string_t::npos);
    EXPECT_NE(result.find(U("END RSA PRIVATE KEY")), utility::string_t::npos);
}

TEST(UtilsTest, FillPrivateKeyMarker_PreservesPkcs8Header) {
    utility::string_t pem =
        U("-----BEGIN PRIVATE KEY-----\nMIIEvQIBADANBg\n-----END PRIVATE KEY-----");
    utility::string_t result = Utils::fill_private_key_marker(pem);
    EXPECT_NE(result.find(U("BEGIN")), utility::string_t::npos);
}

// ── fill_public_key_marker ───────────────────────────────────────────────────

TEST(UtilsTest, FillPublicKeyMarker_AddsPublicKeyHeader) {
    // add_start_end only adds if marker not already present;
    // test with a key that already has the header:
    utility::string_t pem =
        U("-----BEGIN PUBLIC KEY-----\nMIIBIjANBgkqhkiG\n-----END PUBLIC KEY-----");
    utility::string_t result = Utils::fill_public_key_marker(pem);
    EXPECT_NE(result.find(U("BEGIN PUBLIC KEY")), utility::string_t::npos);
    EXPECT_NE(result.find(U("END PUBLIC KEY")), utility::string_t::npos);
}

TEST(UtilsTest, FillPublicKeyMarker_WrapsBarePEM) {
    // Key without header gets wrapped
    utility::string_t bare = U("MIIBIjANBgkqhkiG9w0BAQEFAAOCAQ8AMIIBCgKCAQEA");
    utility::string_t result = Utils::fill_public_key_marker(bare);
    EXPECT_NE(result.find(U("BEGIN PUBLIC KEY")), utility::string_t::npos);
    EXPECT_NE(result.find(U("END PUBLIC KEY")), utility::string_t::npos);
}
