

#include "../include/tigerapi/utils.h"
#include "base64.h"
#include <ctime>
#include <iostream>
#include <random>
#include <algorithm>
#include <regex>
#include "common/rsa_sign.h"

using namespace std;
using namespace web;
using namespace websocketpp;

std::string get_timestamp() {
    struct tm tm;
    time_t t = time(NULL);
    gmtime_r(&t, &tm);
    char timestamp[32];
    strftime(timestamp, sizeof(timestamp), U("%Y-%m-%d %H:%M:%S"), &tm);
    return std::string(timestamp);
}

time_t date_string_to_timestamp(const std::string &date_string) {
    struct tm tm;
    std::istringstream ss(date_string);
    ss >> std::get_time(&tm, U("%Y-%m-%d"));
    tm.tm_hour = 0;
    tm.tm_min = 0;
    tm.tm_sec = 0;
    time_t res = mktime(&tm) * 1000;
    return res;
}

std::string get_sign(unsigned char * private_key, unsigned char * content) {

    unsigned char encrypted[8196 * 16] = {};
    Sha1RSASign sha1RSASign;
    unsigned int encrypted_length = 0;
    int encrypted_ret = sha1RSASign.sha1_encrypt(content, strlen((char *) content),
                                                 private_key, encrypted,
                                                 &encrypted_length);

    if (encrypted_ret != 1) {
        sha1RSASign.print_last_error(U("Private Encrypt failed"));
        exit(0);
    }
    std::string encoded = websocketpp::base64_encode(encrypted, encrypted_length);
    return encoded;
}

std::string get_sign(std::string private_key, std::string content) {

    unsigned char plain_text[8196 * 16];
    std::copy(content.begin(), content.end(), plain_text);
    return get_sign((unsigned char *) fill_private_key_marker(private_key).c_str(), (unsigned char *) content.c_str());
}

bool verify_sign(std::string public_key, std::string content, std::string encoded_signature) {
    std::string filled_public_key = fill_public_key_marker(public_key).c_str();
    unsigned char encrypted[8196 * 16] = {};
    unsigned int encrypted_length = 0;

    std::string decoded = websocketpp::base64_decode(encoded_signature);
    memcpy(encrypted, decoded.data(), decoded.size());
    encrypted_length = decoded.size();

    unsigned char decrypted[4098] = {};
    unsigned int decrypted_length = content.size();
    memcpy(decrypted, content.data(), content.size());

    Sha1RSASign sha1RSASign;
    int decrypted_ret = sha1RSASign.sha1_decrypt(encrypted, encrypted_length,
                                                 (unsigned char *) filled_public_key.c_str(), decrypted, decrypted_length);
    if (decrypted_ret != 1) {
        sha1RSASign.print_last_error(U("Public Decrypt failed"));
        return false;
    }
    return true;
}

string get_level_str(int level) {
    string level_str;
    for (int levelI = 0; levelI < level; levelI++) {
        level_str.append(U("\t"));
    }
    return level_str;
}


string json_format(string json_str) {
    int level = 0;
    string json_format_str;
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


unsigned int str_hex(unsigned char *str, unsigned char *hex) {
    unsigned char ctmp, ctmp1, half;
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

void hex_str(unsigned char *inchar, unsigned int len, unsigned char *outtxt) {
    unsigned char hbit, lbit;
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

std::string get_device_id() {
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<unsigned long long> dis;
    unsigned long long mac = dis(gen);

    std::stringstream ss;
    ss << std::hex << std::setfill('0') << std::setw(12) << mac;
    std::string str = ss.str();
    std::transform(str.begin(), str.end(), str.begin(), ::toupper);

    std::string result = U("");
    for (int i = 0; i < 12; i += 2) {
        result += str.substr(i, 2);
        result += U(":");
    }
    result.pop_back();

    return result;
}




std::string add_start_end(std::string& key, std::string start_marker, std::string end_marker) {
    if (key.find(start_marker) == std::string::npos) {
        key = start_marker + key;
    }
    if (key.find(end_marker) == std::string::npos) {
        key = key + end_marker;
    }
    return key;
}

std::string fill_private_key_marker(std::string& private_key) {
    return add_start_end(private_key, U("-----BEGIN RSA PRIVATE KEY-----\n"), U("\n-----END RSA PRIVATE KEY-----"));
}

std::string fill_public_key_marker(std::string& public_key) {
    return add_start_end(public_key, U("-----BEGIN PUBLIC KEY-----\n"), U("\n-----END PUBLIC KEY-----"));
}


void camel_to_snake(web::json::value& obj) {
    if (obj.is_object()) {
        // Iterate through all the keys in the object
        for (const auto& kv : obj.as_object()) {
            // Convert the key to snake case
            std::string key = kv.first;
//            if (key.empty()) {
//                continue;
//            }
            std::string snake_key;
            bool is_first_char = true;
            for (const auto& c : key) {
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
        for (auto& value : obj.as_array()) {
            camel_to_snake(value);
        }
    }
}




std::tuple<std::string, std::string, std::string, double> extract_option_info(const std::string &identifier) {
    if (!identifier.empty()) {
        std::regex pattern(R"((\w+)\s*(\d{6})([CP])(\d+))");
        std::smatch matches;

        if (std::regex_search(identifier, matches, pattern) && matches.size() == 5) {
            std::string underlying_symbol = matches[1];
            std::string expiry = U("20") + matches[2].str();
            std::string right = matches[3];
            double strike = std::stod(matches[4]) / 1000;
            if (expiry.size() == 8) {
                expiry = expiry.substr(0, 4) + U("-") + expiry.substr(4, 2) + U("-") + expiry.substr(6);
            }
            right = (right == U("C")) ? U("CALL") : U("PUT");
            return std::make_tuple(underlying_symbol, expiry, right, strike);
        }
    }
    return std::make_tuple(U(""), U(""), U(""), 0.0);
}


