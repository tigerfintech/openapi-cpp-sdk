#ifndef TIGERAPI_TICK_UTIL_H
#define TIGERAPI_TICK_UTIL_H

#pragma once
#include <string>
#include <map>
#include <optional>

namespace TIGER_API {

    // 返回交易所代码
    std::optional<std::string> get_part_code(const std::string& code);

    // 返回交易所完整名称
    std::optional<std::string> get_part_code_name(const std::string& code);

    // 根据quote_level获取对应的交易条件映射表
    const std::map<std::string, std::string>& get_trade_condition_map(const std::string& quote_level);

    // 根据条件代码和条件映射表获取交易条件描述
    std::optional<std::string> get_trade_condition(
        const std::string& cond, 
        const std::map<std::string, std::string>& cond_map
    );

} // namespace TIGER_API

#endif // TIGERAPI_TICK_UTIL_H