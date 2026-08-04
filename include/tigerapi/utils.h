#ifndef CPPSDK_UTILS_H
#define CPPSDK_UTILS_H

#include <time.h>
#include <string>
#include <map>
#include "zlib.h"
#include "cpprest/json.h"
#include "cpprest/details/basic_types.h"
#include "win32.h"

namespace TIGER_API {
    class OPENAPI_EXPORT Utils {
    public:
        static utility::string_t str8to16(std::string s);

        static std::string str16to8(utility::string_t s);

        static utility::string_t get_timestamp();

        /**
         * Convert a "yyyy-MM-dd" date to an epoch-ms timestamp.
         *
         * The date is interpreted in `timezone` (default US/Eastern, matching the
         * Python/Java SDKs), NOT in the host's local timezone — an option expiry
         * names a market calendar day, so using local time would shift the
         * instant onto the wrong day on any machine outside that timezone and the
         * server would return an empty result.
         *
         * @param timezone tz database name, e.g. U("US/Eastern"), U("Asia/Hong_Kong").
         * @return epoch milliseconds, or 0 if the string cannot be parsed.
         */
        static time_t date_string_to_timestamp(const utility::string_t &date_string,
                                              const utility::string_t &timezone = U("US/Eastern"));

        static utility::string_t get_device_id();

        static void camel_to_snake(web::json::value &obj);

        static utility::string_t json_format(utility::string_t json_str);

        static utility::string_t get_level_str(int level);

        static utility::string_t get_sign(const utility::string_t &key, const utility::string_t &content);

        static bool
        verify_sign(utility::string_t public_key, const utility::string_t &content,
                    const utility::string_t &encoded_signature);

        static utility::string_t
        add_start_end(const utility::string_t &key, const utility::string_t &start_marker,
                      const utility::string_t &end_marker);

        static utility::string_t fill_private_key_marker(const utility::string_t &private_key);

        static utility::string_t fill_public_key_marker(const utility::string_t &public_key);

        static utility::string_t double_to_string(double num, int precision = 1);

        static bool is_directory(const utility::string_t& path);
    };
}
#endif //CPPSDK_UTILS_H
