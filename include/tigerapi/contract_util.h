//
// Created by sukai on 2022/12/27.
//

#ifndef TIGERAPI_CONTRACT_UTIL_H
#define TIGERAPI_CONTRACT_UTIL_H


#include <tuple>
#include "model.h"

Contract stock_contract(const std::string symbol, const std::string currency, const std::string local_symbol = U(""),
                        const std::string exchange = U(""), long contract_id = -1);

Contract option_contract(const string symbol, const string expiry, double strike, const string right,
                         const string currency, long multiplier = 100, const string local_symbol = U(""),
                         long contract_id = 0);
Contract option_contract(const string identifier, long multiplier = 100, const string currency = U("USD"));

Contract future_contract(const string symbol, const string currency, const string expiry = U(""),
                         const string exchange = U(""), const string contract_month = U(""), long multiplier = 0,
                         const string local_symbol = U(""));

/**
 * 期权要素信息提取
 * @param identifier 期权标识符，如 U("AAPL 220414C0017500")， U("JD 211015P00045000")
 * @return 正股代码，到期日，行权方向，行权价格 组成的tuple， (underlying_symbol, expiry, right, strike)
 */
std::tuple<std::string, std::string, std::string, double> extract_option_info(const std::string identifier);


#endif //TIGERAPI_CONTRACT_UTIL_H
