#include "tick_util.h"
#include <algorithm>
#include <cctype>

namespace TIGER_API {

    // convert string to lowercase
    std::string to_lower(std::string str) {
        std::transform(str.begin(), str.end(), str.begin(), 
            [](unsigned char c) { return std::tolower(c); });
        return str;
    }


    std::optional<std::string> get_part_code(const std::string& code) {
        auto it = PART_CODE_MAP.find(code);
        if (it != PART_CODE_MAP.end()) {
            return it->second;
        }
        return std::nullopt;
    }

    std::optional<std::string> get_part_code_name(const std::string& code) {
        auto it = PART_CODE_NAME_MAP.find(code);
        if (it != PART_CODE_NAME_MAP.end()) {
            return it->second;
        }
        return std::nullopt;
    }

    const std::map<std::string, std::string>& get_trade_condition_map(
        const std::string& quote_level) 
    {
        if (!quote_level.empty()) {
            std::string lower_level = to_lower(quote_level);
            if (lower_level.compare(0, HK_QUOTE_LEVEL_PREFIX.length(), 
                HK_QUOTE_LEVEL_PREFIX) == 0) {
                return HK_TRADE_COND_MAP;
            }
        }
        return US_TRADE_COND_MAP;
    }

    std::optional<std::string> get_trade_condition(
        const std::string& cond,
        const std::map<std::string, std::string>& cond_map)
    {
        auto it = cond_map.find(cond);
        if (it != cond_map.end()) {
            return it->second;
        }
        return std::nullopt;
    }

} // namespace TIGER_API
