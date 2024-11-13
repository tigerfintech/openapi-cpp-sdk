#ifndef TIGERAPI_TICK_UTIL_H
#define TIGERAPI_TICK_UTIL_H

#pragma once
#include <string>
#include <map>
#include <optional>

namespace TIGER_API {

    // Returns exchange code
    std::string get_part_code(const std::string& code);

    // Returns full exchange name
    std::string get_part_code_name(const std::string& code);

    // Get trade condition mapping table based on quote level
    const std::map<std::string, std::string>& get_trade_condition_map(const std::string& quote_level);

    // Get trade condition description based on condition code and condition mapping table
    std::string get_trade_condition(
        const std::string& cond, 
        const std::map<std::string, std::string>& cond_map
    );

} // namespace TIGER_API

#endif // TIGERAPI_TICK_UTIL_H