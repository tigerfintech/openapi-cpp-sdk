

#ifndef CPPSDK_UTILS_H
#define CPPSDK_UTILS_H

#include <ctime>
#include <string>
#include <map>
#include <zlib.h>
#include <cpprest/json.h>

utility::string_t get_timestamp();

long date_string_to_timestamp(const utility::string_t &date_string);

utility::string_t get_device_id();

void camel_to_snake(web::json::value &obj);

utility::string_t json_format(utility::string_t json_str);

utility::string_t get_sign(utility::string_t &key, const utility::string_t &content);

bool
verify_sign(utility::string_t public_key, const utility::string_t &content, const utility::string_t &encoded_signature);

utility::string_t
add_start_end(utility::string_t &key, const utility::string_t &start_marker, const utility::string_t end_marker);

utility::string_t fill_private_key_marker(utility::string_t &private_key);

utility::string_t fill_public_key_marker(utility::string_t &public_key);

std::tuple<utility::string_t, utility::string_t, utility::string_t, double>
extract_option_info(const utility::string_t &identifier);

unsigned int str_hex(unsigned char *str, unsigned char *hex);

void hex_str(unsigned char *inchar, unsigned int len, utility::char_t *outtxt);

#endif //CPPSDK_UTILS_H
