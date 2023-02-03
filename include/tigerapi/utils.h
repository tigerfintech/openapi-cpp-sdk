

#ifndef CPPSDK_UTILS_H
#define CPPSDK_UTILS_H

#include <time.h>
#include <string>
#include <map>
#include <zlib.h>
#include <cpprest/json.h>
#include <cpprest/details/basic_types.h>
#include "win32.h"

OPENAPI_EXPORT utility::string_t get_timestamp();

OPENAPI_EXPORT time_t date_string_to_timestamp(const utility::string_t &date_string);

OPENAPI_EXPORT utility::string_t get_device_id();

OPENAPI_EXPORT void camel_to_snake(web::json::value &obj);

OPENAPI_EXPORT utility::string_t json_format(utility::string_t json_str);

OPENAPI_EXPORT utility::string_t get_sign(utility::string_t &key, const utility::string_t &content);

OPENAPI_EXPORT bool
verify_sign(utility::string_t public_key, const utility::string_t &content, const utility::string_t &encoded_signature);

OPENAPI_EXPORT utility::string_t
add_start_end(utility::string_t &key, const utility::string_t &start_marker, const utility::string_t end_marker);

OPENAPI_EXPORT utility::string_t fill_private_key_marker(utility::string_t &private_key);

OPENAPI_EXPORT utility::string_t fill_public_key_marker(utility::string_t &public_key);

OPENAPI_EXPORT unsigned int str_hex(unsigned char *str, unsigned char *hex);

OPENAPI_EXPORT void hex_str(unsigned char *inchar, unsigned int len, utility::char_t *outtxt);

#endif //CPPSDK_UTILS_H
