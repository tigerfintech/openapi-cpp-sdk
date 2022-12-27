//
// Created by sukai on 2022/12/27.
//

#ifndef TIGERAPI_CONTRACT_UTIL_H
#define TIGERAPI_CONTRACT_UTIL_H


#include <tuple>
#include "model.h"

Contract stock_contract(const std::string symbol, const std::string currency, const std::string local_symbol = "",
                        const std::string exchange = "", long contract_id = -1);

Contract option_contract(const string symbol, const string expiry, double strike, const string right,
                         const string currency, long multiplier = 100, const string local_symbol = "",
                         long contract_id = 0);
Contract option_contract(const string identifier, long multiplier = 100, const string currency = "USD");

Contract future_contract(const string symbol, const string currency, const string expiry = "",
                         const string exchange = "", const string contract_month = "", long multiplier = 0,
                         const string local_symbol = "");

/**
 * 期权要素信息提取
 * @param identifier 期权标识符，如 "AAPL 220414C0017500"， "JD 211015P00045000"
 * @return 正股代码，到期日，行权方向，行权价格 组成的tuple， (underlying_symbol, expiry, right, strike)
 */
std::tuple<std::string, std::string, std::string, double> extract_option_info(const std::string identifier);


#endif //TIGERAPI_CONTRACT_UTIL_H
