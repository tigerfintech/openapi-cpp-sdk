#include "../include/tigerapi/utils.h"
#include "common/base64.h"
#include <ctime>
#include <iostream>
#include <iomanip>
#include <random>
#include <algorithm>
#include <regex>
#include "common/sign_util.h"

using namespace std;
using namespace web;
using namespace websocketpp;

utility::string_t get_timestamp() {
    time_t t = time(NULL);
    utility::char_t tmp[32];
    strftime((char*)tmp, sizeof(tmp), "%Y-%m-%d %H:%M:%S", localtime(&t));
    return utility::string_t(tmp);
//    struct tm tm;
//    time_t t = time(NULL);
//    gmtime_r(&t, &tm);
//    char timestamp[32];
//    strftime(timestamp, sizeof(timestamp), U("%Y-%m-%d %H:%M:%S"), &tm);
//    return utility::string_t(timestamp);
}

time_t date_string_to_timestamp(const utility::string_t &date_string) {
    struct tm tm;
    utility::istringstream_t ss(date_string);
    ss >> std::get_time(&tm, U("%Y-%m-%d"));
    tm.tm_hour = 0;
    tm.tm_min = 0;
    tm.tm_sec = 0;
    time_t res = mktime(&tm) * 1000;
    return res;
}

utility::string_t get_sign(utility::string_t &private_key, const utility::string_t &content) {
    utility::string_t filled_private_key = fill_private_key_marker(private_key);

    utility::string_t encrypted = sha1_sign(content, filled_private_key);
    return websocketpp::base64_encode(encrypted);
}

bool verify_sign(utility::string_t public_key, const utility::string_t &content,
                 const utility::string_t &encoded_signature) {
    utility::string_t filled_public_key = fill_public_key_marker(public_key);
    int ret = sha1_verify(content, encoded_signature, filled_public_key);

    if (ret != 1) {
                //LOGGER(info) << U("Public Decrypt failed");
        return false;
    }
    return true;
}

utility::string_t get_level_str(int level) {
    utility::string_t level_str;
    for (int levelI = 0; levelI < level; levelI++) {
        level_str.append(U("\t"));
    }
    return level_str;
}


utility::string_t json_format(utility::string_t json_str) {
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

int gz_decompress(Byte *zdata, uLong nzdata, Byte *data, uLong *ndata) {
    int err = 0;
    z_stream d_stream = {0}; /* decompression stream */

    static char dummy_head[2] = {
            0x8 + 0x7 * 0x10,
            (((0x8 + 0x7 * 0x10) * 0x100 + 30) / 31 * 31) & 0xFF,
    };

    d_stream.zalloc = NULL;
    d_stream.zfree = NULL;
    d_stream.opaque = NULL;
    d_stream.next_in = zdata;
    d_stream.avail_in = 0;
    d_stream.next_out = data;


    if (inflateInit2(&d_stream, -MAX_WBITS) != Z_OK) {
        return -1;
    }

    // if(inflateInit2(&d_stream, 47) != Z_OK) return -1;

    while (d_stream.total_out < *ndata && d_stream.total_in < nzdata) {
        d_stream.avail_in = d_stream.avail_out = 1; /* force small buffers */
        if ((err = inflate(&d_stream, Z_NO_FLUSH)) == Z_STREAM_END)
            break;

        if (err != Z_OK) {
            if (err == Z_DATA_ERROR) {
                d_stream.next_in = (Bytef *) dummy_head;
                d_stream.avail_in = sizeof(dummy_head);
                if ((err = inflate(&d_stream, Z_NO_FLUSH)) != Z_OK) {
                    return -1;
                }
            } else {
                return -1;
            }
        }
    }

    if (inflateEnd(&d_stream) != Z_OK)
        return -1;
    *ndata = d_stream.total_out;
    return 0;
}


unsigned int str_hex(utility::char_t *str, utility::char_t *hex) {
    utility::char_t ctmp, ctmp1, half;
    unsigned int num = 0;
    do {
        do {
            half = 0;
            ctmp = *str;
            if (!ctmp) break;
            str++;
        } while ((ctmp == 0x20) || (ctmp == 0x2c) || (ctmp == '\t'));
        if (!ctmp) break;
        if (ctmp >= 'a') ctmp = ctmp - 'a' + 10;
        else if (ctmp >= 'A') ctmp = ctmp - 'A' + 10;
        else ctmp = ctmp - '0';
        ctmp = ctmp << 4;
        half = 1;
        ctmp1 = *str;
        if (!ctmp1) break;
        str++;
        if ((ctmp1 == 0x20) || (ctmp1 == 0x2c) || (ctmp1 == '\t')) {
            ctmp = ctmp >> 4;
            ctmp1 = 0;
        } else if (ctmp1 >= 'a') ctmp1 = ctmp1 - 'a' + 10;
        else if (ctmp1 >= 'A') ctmp1 = ctmp1 - 'A' + 10;
        else ctmp1 = ctmp1 - '0';
        ctmp += ctmp1;
        *hex = ctmp;
        hex++;
        num++;
    } while (1);
    if (half) {
        ctmp = ctmp >> 4;
        *hex = ctmp;
        num++;
    }
    return (num);
}

void hex_str(utility::char_t *inchar, unsigned int len, utility::char_t *outtxt) {
    utility::char_t hbit, lbit;
    unsigned int i;
    for (i = 0; i < len; i++) {
        hbit = (*(inchar + i) & 0xf0) >> 4;
        lbit = *(inchar + i) & 0x0f;
        if (hbit > 9) outtxt[2 * i] = 'A' + hbit - 10;
        else outtxt[2 * i] = '0' + hbit;
        if (lbit > 9) outtxt[2 * i + 1] = 'A' + lbit - 10;
        else outtxt[2 * i + 1] = '0' + lbit;
    }
    outtxt[2 * i] = 0;
}

utility::string_t get_device_id() {

#if defined(WIN32) || defined(_WIN32) || defined(__WIN32__) || defined(__NT__)
//    return U("mac-addr-win");
    #include <Windows.h>
    #include <iphlpapi.h>
    #include <iostream>
    #include <sstream>
    #include <iomanip>
    // Get adapter information
    IP_ADAPTER_INFO adapterInfo[16];
    DWORD bufLength = sizeof(adapterInfo);
    DWORD status = GetAdaptersInfo(adapterInfo, &bufLength);
    if (status != ERROR_SUCCESS) {
        std::cerr << "GetAdaptersInfo failed: " << GetLastError() << std::endl;
        return "";
    }

    // Loop through all adapters
    for (IP_ADAPTER_INFO *adapter = adapterInfo; adapter; adapter = adapter->Next) {
        // Get the MAC address
        BYTE macAddress[6];
        memcpy(macAddress, adapter->Address, 6);
        // Convert the MAC address to a string representation
        std::ostringstream macAddressString;
        macAddressString << std::hex << std::setfill('0');
        for (int i = 0; i < 6; i++) {
            macAddressString << std::setw(2) << static_cast<int>(macAddress[i]);
            if (i < 5) {
                macAddressString << ':';
            }
        }
        return macAddressString.str();
    }
    return "";

#else
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<unsigned long long> dis;
    unsigned long long mac = dis(gen);

    utility::stringstream_t ss;
    ss << std::hex << std::setfill('0') << std::setw(12) << mac;
    utility::string_t str = ss.str();
    std::transform(str.begin(), str.end(), str.begin(), ::toupper);

    utility::string_t result = U("");
    for (int i = 0; i < 12; i += 2) {
        result += str.substr(i, 2);
        result += U(":");
    }
    result.pop_back();

    return result;
#endif
}


utility::string_t
add_start_end(utility::string_t &key, const utility::string_t &start_marker, const utility::string_t end_marker) {
    if (key.find(start_marker) == std::string::npos) {
        key = start_marker + key;
    }
    if (key.find(end_marker) == std::string::npos) {
        key = key + end_marker;
    }
    return key;
}

utility::string_t fill_private_key_marker(utility::string_t &private_key) {
    return add_start_end(private_key, U("-----BEGIN RSA PRIVATE KEY-----\n"), U("\n-----END RSA PRIVATE KEY-----"));
}

utility::string_t fill_public_key_marker(utility::string_t &public_key) {
    return add_start_end(public_key, U("-----BEGIN PUBLIC KEY-----\n"), U("\n-----END PUBLIC KEY-----"));
}


void camel_to_snake(web::json::value &obj) {
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


