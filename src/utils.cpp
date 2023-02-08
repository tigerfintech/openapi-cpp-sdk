#include "../include/tigerapi/utils.h"
#include <ctime>
#include <iomanip>
#include <random>
#include <algorithm>
#include <regex>
#include "../include/tigerapi/sign_util.h"

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

    utility::string_t Utils::get_sign(utility::string_t &private_key, const utility::string_t &content) {
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
        for (int i = 0; i < json_str.length(); i++) {
            char c = json_str.at(i);
            if (level > 0 && '\n' == json_format_str.at(json_format_str.length() - 1)) {
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
    Utils::add_start_end(utility::string_t &key, const utility::string_t &start_marker, const utility::string_t end_marker) {
        if (key.find(start_marker) == std::string::npos) {
            key = start_marker + key;
        }
        if (key.find(end_marker) == std::string::npos) {
            key = key + end_marker;
        }
        return key;
    }

    utility::string_t Utils::fill_private_key_marker(utility::string_t &private_key) {
        return add_start_end(private_key, U("-----BEGIN RSA PRIVATE KEY-----\n"), U("\n-----END RSA PRIVATE KEY-----"));
    }

    utility::string_t Utils::fill_public_key_marker(utility::string_t &public_key) {
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
}