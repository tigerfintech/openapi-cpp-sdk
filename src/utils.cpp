#include "tigerapi/utils.h"
#include <ctime>
#include <iomanip>
#include <random>
#include <algorithm>
#include <regex>
#include "tigerapi/sign_util.h"

using namespace std;
using namespace web;

namespace TIGER_API {
    utility::string_t Utils::str8to16(std::string s) {
#if defined(WIN32) || defined(_WIN32) || defined(__WIN32__) || defined(__NT__)
        return utility::conversions::utf8_to_utf16(s);
#else
        return s;
#endif
    }

    std::string Utils::str16to8(utility::string_t s) {
#if defined(WIN32) || defined(_WIN32) || defined(__WIN32__) || defined(__NT__)
        return utility::conversions::utf16_to_utf8(s);
#else
        return s;
#endif
    }

    utility::string_t Utils::get_timestamp() {
        auto now = std::chrono::system_clock::now();
        auto in_time_t = std::chrono::system_clock::to_time_t(now);
        std::stringstream ss;
        ss << std::put_time(std::localtime(&in_time_t), "%Y-%m-%d %H:%M:%S");
        std::string result = ss.str();
        return str8to16(result);
    }

    time_t Utils::date_string_to_timestamp(const utility::string_t &date_string) {
        struct tm tm;
        utility::istringstream_t ss(date_string);
        ss >> std::get_time(&tm, U("%Y-%m-%d"));
        tm.tm_hour = 0;
        tm.tm_min = 0;
        tm.tm_sec = 0;
        time_t res = mktime(&tm) * 1000;
        return res;
    }

    utility::string_t Utils::get_sign(const utility::string_t &private_key, const utility::string_t &content) {
        utility::string_t filled_private_key = Utils::fill_private_key_marker(private_key);

        utility::string_t encrypted = sha1_sign(content, filled_private_key);
        std::vector<unsigned char> vec(encrypted.begin(), encrypted.end());
        return utility::conversions::to_base64(vec);
    }

    bool Utils::verify_sign(utility::string_t public_key, const utility::string_t &content,
                     const utility::string_t &encoded_signature) {
        //return true;
        utility::string_t filled_public_key = Utils::fill_public_key_marker(public_key);
        int ret = sha1_verify(content, encoded_signature, filled_public_key);

        if (ret != 1) {
            return false;
        }
        return true;
    }

    utility::string_t Utils::get_level_str(int level) {
        utility::string_t level_str;
        for (int levelI = 0; levelI < level; levelI++) {
            level_str.append(U("\t"));
        }
        return level_str;
    }


    utility::string_t Utils::json_format(utility::string_t json_str) {
        int level = 0;
        utility::string_t json_format_str;
        json_format_str.reserve(json_str.length() * 2);
        for (int i = 0; i < json_str.length(); i++) {
            utility::char_t c = json_str.at(i);
            if (level > 0 && !json_format_str.empty() && '\n' == json_format_str.at(json_format_str.length() - 1)) {
                json_format_str += get_level_str(level);
            }
            switch (c) {
                case '{':
                case '[':
                    json_format_str += c;
                    json_format_str += '\n';
                    level++;
                    break;
                case ',':
                    json_format_str += c;
                    json_format_str += '\n';
                    break;
                case '}':
                case ']':
                    json_format_str += '\n';
                    level--;
                    json_format_str += get_level_str(level);
                    json_format_str += c;
                    break;
                default:
                    json_format_str += c;
                    break;
            }
        }

        return json_format_str;

    }

    utility::string_t Utils::get_device_id() {
        std::mt19937 rng;
        rng.seed(std::random_device()());
        std::uniform_int_distribution<> dist(0, 255);

        std::ostringstream mac_addr;
        mac_addr << std::hex << std::setfill('0');
        for (int i = 0; i < 6; i++) {
            int n = dist(rng);
            mac_addr << std::setw(2) << n;
            if (i < 5) {
                mac_addr << ':';
            }
        }
        return Utils::str8to16(mac_addr.str());
    }


    utility::string_t
    Utils::add_start_end(const utility::string_t &key, const utility::string_t &start_marker, const utility::string_t &end_marker) {
        utility::string_t result = key;
        if (result.find(start_marker) == std::string::npos) {
            result = start_marker + result;
        }
        if (result.find(end_marker) == std::string::npos) {
            result = result + end_marker;
        }
        return result;
    }

    utility::string_t Utils::fill_private_key_marker(const utility::string_t &private_key) {
        // If key already has PEM header, return as-is
        if (private_key.find(U("-----BEGIN")) != std::string::npos) {
            return private_key;
        }
        // Detect PKCS#8 DER base64: PKCS#8 PrivateKeyInfo starts with 30 82 ... 02 01 00 30
        // The 4th byte (index 3) after base64 decode is 0x02 for PKCS#1, 0x30 for PKCS#8.
        // More reliably: try to detect by checking for RSA OID bytes in the DER header.
        // PKCS#8 DER base64 strings start with "MIICdw" or "MIIEv" pattern (contains AlgorithmIdentifier).
        // PKCS#1 DER base64 strings start with "MIICXg", "MIICWw", "MIIEo" etc.
        // Simplest reliable check: decode first 16 bytes and look for PKCS#8 sequence marker (0x30 at byte 4).
        bool is_pkcs8 = false;
#if defined(WIN32) || defined(_WIN32) || defined(__WIN32__) || defined(__NT__)
        std::string key_utf8 = utility::conversions::utf16_to_utf8(private_key);
#else
        std::string key_utf8 = private_key;
#endif
        // Decode first few bytes to determine format
        BIO* b64bio = BIO_new(BIO_f_base64());
        BIO* membio = BIO_new_mem_buf(key_utf8.c_str(), -1);
        if (b64bio == nullptr || membio == nullptr) {
            if (b64bio != nullptr) BIO_free(b64bio);
            if (membio != nullptr) BIO_free(membio);
            return add_start_end(private_key, U("-----BEGIN RSA PRIVATE KEY-----\n"), U("\n-----END RSA PRIVATE KEY-----"));
        }
        BIO_set_flags(b64bio, BIO_FLAGS_BASE64_NO_NL);
        BIO* chainbio = BIO_push(b64bio, membio);
        if (chainbio == nullptr) {
            BIO_free_all(b64bio);
            return add_start_end(private_key, U("-----BEGIN RSA PRIVATE KEY-----\n"), U("\n-----END RSA PRIVATE KEY-----"));
        }
        unsigned char header[8] = {0};
        int read_len = BIO_read(chainbio, header, 8);
        BIO_free_all(chainbio);
        if (read_len <= 0) {
            return add_start_end(private_key, U("-----BEGIN RSA PRIVATE KEY-----\n"), U("\n-----END RSA PRIVATE KEY-----"));
        }
        // PKCS#8 PrivateKeyInfo: SEQUENCE { INTEGER 0, SEQUENCE { OID ... } ... }
        // byte[0]=0x30 (SEQUENCE), byte[4]=0x02 (INTEGER version=0), byte[6] or nearby=0x30 (SEQUENCE AlgId)
        // PKCS#1 RSAPrivateKey: SEQUENCE { INTEGER 0, INTEGER n, ... }
        // byte[0]=0x30 (SEQUENCE), byte[4]=0x02, byte[6]=first INTEGER of modulus
        // Distinguish: in PKCS#8, after the outer SEQUENCE header and version INTEGER,
        // there is an AlgorithmIdentifier SEQUENCE (0x30). In PKCS#1, there are multiple INTEGERs.
        // Simple heuristic: PKCS#8 DER base64 decoded byte[4] is typically 0x02 (version INTEGER tag)
        // and byte[7] is 0x30 (AlgorithmIdentifier SEQUENCE tag).
        // For a 2048-bit key: PKCS#8 header = 30 82 xx xx 02 01 00 30 ...
        //                     PKCS#1 header = 30 82 xx xx 02 03 00 ... (version INTEGER then modulus)
        // Reliable: check byte[6] after outer sequence: PKCS#8 has 0x30 there, PKCS#1 has 0x02
        if (read_len >= 8 && header[0] == 0x30) {
            // byte[4] is the tag of the first element inside the outer SEQUENCE
            // For PKCS#8: version INTEGER (0x02), then length 0x01, value 0x00, then AlgId SEQUENCE (0x30)
            // So byte[4]=0x02, byte[5]=0x01, byte[6]=0x00, byte[7]=0x30
            if (read_len >= 8 && header[4] == 0x02 && header[5] == 0x01 && header[6] == 0x00 && header[7] == 0x30) {
                is_pkcs8 = true;
            }
        }
        if (is_pkcs8) {
            return add_start_end(private_key, U("-----BEGIN PRIVATE KEY-----\n"), U("\n-----END PRIVATE KEY-----"));
        } else {
            return add_start_end(private_key, U("-----BEGIN RSA PRIVATE KEY-----\n"), U("\n-----END RSA PRIVATE KEY-----"));
        }
    }

    utility::string_t Utils::fill_public_key_marker(const utility::string_t &public_key) {
        return add_start_end(public_key, U("-----BEGIN PUBLIC KEY-----\n"), U("\n-----END PUBLIC KEY-----"));
    }


    void Utils::camel_to_snake(web::json::value &obj) {
        if (obj.is_object()) {
            // Iterate through all the keys in the object
            for (const auto &kv: obj.as_object()) {
                // Convert the key to snake case
                utility::string_t key = kv.first;
//            if (key.empty()) {
//                continue;
//            }
                utility::string_t snake_key;
                bool is_first_char = true;
                for (const auto &c: key) {
                    if (isupper(c)) {
                        if (!is_first_char) {
                            snake_key += '_';
                        }
                        snake_key += tolower(c);
                    } else {
                        snake_key += c;
                    }
                    is_first_char = false;
                }
                // Recurse into the value and convert it as well
                auto value = kv.second;
                camel_to_snake(value);
                // Replace the key with the converted key
                obj[snake_key] = value;
                obj.erase(key);
            }
        } else if (obj.is_array()) {
            // Iterate through all the elements in the array and convert them
            for (auto &value: obj.as_array()) {
                camel_to_snake(value);
            }
        }
    }


    utility::string_t Utils::double_to_string(double num, int precision) {
        std::ostringstream stream;
        stream << std::fixed << std::setprecision(precision) << num;
        std::string result = stream.str();
        return Utils::str8to16(result);
    }

	bool Utils::is_directory(const utility::string_t& path) {
		if (path.empty()) {
			return false;
		}

#ifdef _WIN32
		return path.back() == U('\\') || path.back() == U('/');
#else
		return path.back() == U('/');
#endif
	}
}