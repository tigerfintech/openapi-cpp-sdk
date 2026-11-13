#include <gtest/gtest.h>
#include "tigerapi/properties.h"
#include <sstream>
#include <fstream>

using namespace TIGER_API;

// ── load / get_property ───────────────────────────────────────────────────────

TEST(PropertiesTest, Load_SimpleKeyValue) {
    // Properties::load takes std::ifstream& — validate via set_property/get_property roundtrip.
    Properties props;
    props.set_property(U("key1"), U("value1"));
    props.set_property(U("key2"), U("value2"));
    EXPECT_EQ(props.get_property(U("key1")), U("value1"));
    EXPECT_EQ(props.get_property(U("key2")), U("value2"));
}

TEST(PropertiesTest, GetProperty_NotFound_ReturnsEmpty) {
    Properties props;
    EXPECT_EQ(props.get_property(U("nonexistent")), U(""));
}

TEST(PropertiesTest, SetProperty_Overwrite) {
    Properties props;
    props.set_property(U("key"), U("old"));
    props.set_property(U("key"), U("new"));
    EXPECT_EQ(props.get_property(U("key")), U("new"));
}

TEST(PropertiesTest, SetProperty_EmptyKey_EmptyValue) {
    Properties props;
    props.set_property(U(""), U(""));
    EXPECT_EQ(props.get_property(U("")), U(""));
}

// ── load from file ────────────────────────────────────────────────────────────

TEST(PropertiesTest, Load_FromFile_Basic) {
    // Write a temp properties file
    const char* tmp_path = "/tmp/test_props_basic.properties";
    {
        std::ofstream out(tmp_path);
        out << "tiger_id=test123\n";
        out << "account=acct456\n";
        out << "# comment line\n";
        out << "! also comment\n";
        out << "\n";  // empty line
        out << "private_key_pk8=dGVzdA==\n";
    }

    std::ifstream in(tmp_path);
    ASSERT_TRUE(in.is_open());
    Properties props;
    props.load(in);
    in.close();

    EXPECT_EQ(props.get_property(U("tiger_id")), U("test123"));
    EXPECT_EQ(props.get_property(U("account")), U("acct456"));
    EXPECT_EQ(props.get_property(U("private_key_pk8")), U("dGVzdA=="));
    // Comments should be skipped
    EXPECT_EQ(props.get_property(U("# comment line")), U(""));
    EXPECT_EQ(props.get_property(U("! also comment")), U(""));
}

TEST(PropertiesTest, Load_FromFile_WithColonSeparator) {
    const char* tmp_path = "/tmp/test_props_colon.properties";
    {
        std::ofstream out(tmp_path);
        out << "key1:value1\n";
        out << "key2:value2\n";
    }

    std::ifstream in(tmp_path);
    ASSERT_TRUE(in.is_open());
    Properties props;
    props.load(in);
    in.close();

    EXPECT_EQ(props.get_property(U("key1")), U("value1"));
    EXPECT_EQ(props.get_property(U("key2")), U("value2"));
}

TEST(PropertiesTest, Load_FromFile_WithEscapedEquals) {
    const char* tmp_path = "/tmp/test_props_escape.properties";
    {
        std::ofstream out(tmp_path);
        out << "key=val\\=ue\n";
    }

    std::ifstream in(tmp_path);
    ASSERT_TRUE(in.is_open());
    Properties props;
    props.load(in);
    in.close();

    // The escaped = should be preserved in the value
    EXPECT_EQ(props.get_property(U("key")), U("val=ue"));
}

TEST(PropertiesTest, Load_FromFile_WithEscapeSequences) {
    const char* tmp_path = "/tmp/test_props_escseq.properties";
    {
        std::ofstream out(tmp_path);
        out << "key=val\\nue\\t\n";
    }

    std::ifstream in(tmp_path);
    ASSERT_TRUE(in.is_open());
    Properties props;
    props.load(in);
    in.close();

    // \n and \t should be unescaped to newline and tab
    utility::string_t val = props.get_property(U("key"));
    EXPECT_NE(val.find(U("\n")), utility::string_t::npos);
    EXPECT_NE(val.find(U("\t")), utility::string_t::npos);
}

TEST(PropertiesTest, Load_FromFile_WithBOM) {
    const char* tmp_path = "/tmp/test_props_bom.properties";
    {
        std::ofstream out(tmp_path, std::ios::binary);
        char bom[3] = {(char)0xEF, (char)0xBB, (char)0xBF};
        out.write(bom, 3);
        out << "key=value\n";
    }

    std::ifstream in(tmp_path);
    ASSERT_TRUE(in.is_open());
    Properties props;
    props.load(in);
    in.close();

    EXPECT_EQ(props.get_property(U("key")), U("value"));
}

TEST(PropertiesTest, Load_FromFile_EmptyFile) {
    const char* tmp_path = "/tmp/test_props_empty.properties";
    {
        std::ofstream out(tmp_path);
        // empty file
    }

    std::ifstream in(tmp_path);
    ASSERT_TRUE(in.is_open());
    Properties props;
    props.load(in);
    in.close();

    EXPECT_EQ(props.get_property(U("any")), U(""));
}

TEST(PropertiesTest, Load_FromFile_OnlyComments) {
    const char* tmp_path = "/tmp/test_props_comments.properties";
    {
        std::ofstream out(tmp_path);
        out << "# comment1\n";
        out << "! comment2\n";
        out << "\n";
    }

    std::ifstream in(tmp_path);
    ASSERT_TRUE(in.is_open());
    Properties props;
    props.load(in);
    in.close();

    EXPECT_EQ(props.get_property(U("any")), U(""));
}

// ── store ──────────────────────────────────────────────────────────────────────

TEST(PropertiesTest, Store_Roundtrip) {
    // Write to a temp file, then read it back
    const char* tmp_path = "/tmp/test_props_store.properties";
    {
        std::ofstream out(tmp_path);
        Properties props;
        props.set_property(U("key1"), U("value1"));
        props.set_property(U("key2"), U("value2"));
        props.store(out);
    }

    std::ifstream in(tmp_path);
    ASSERT_TRUE(in.is_open());
    Properties props2;
    props2.load(in);
    in.close();

    EXPECT_EQ(props2.get_property(U("key1")), U("value1"));
    EXPECT_EQ(props2.get_property(U("key2")), U("value2"));
}

TEST(PropertiesTest, Store_ContainsHeader) {
    const char* tmp_path = "/tmp/test_props_header.properties";
    {
        std::ofstream out(tmp_path);
        Properties props;
        props.set_property(U("key"), U("val"));
        props.store(out);
    }

    std::ifstream in(tmp_path);
    std::string content((std::istreambuf_iterator<char>(in)),
                         std::istreambuf_iterator<char>());
    in.close();

    EXPECT_NE(content.find("# Properties"), std::string::npos);
    EXPECT_NE(content.find("key=val"), std::string::npos);
}

TEST(PropertiesTest, Store_EscapesSpecialChars) {
    const char* tmp_path = "/tmp/test_props_special.properties";
    {
        std::ofstream out(tmp_path);
        Properties props;
        props.set_property(U("key"), U("val with = and : chars"));
        props.store(out);
    }

    std::ifstream in(tmp_path);
    Properties props2;
    props2.load(in);
    in.close();

    // The escaped chars should round-trip correctly
    utility::string_t val = props2.get_property(U("key"));
    EXPECT_EQ(val, U("val with = and : chars"));
}
