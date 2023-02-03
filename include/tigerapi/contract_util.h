//
// Created by sukai on 2022/12/27.
//

#ifndef TIGERAPI_CONTRACT_UTIL_H
#define TIGERAPI_CONTRACT_UTIL_H


#include <tuple>
#include "model.h"
#include "win32.h"

OPENAPI_EXPORT Contract stock_contract(const utility::string_t symbol, const utility::string_t currency, const utility::string_t local_symbol = U(""),
                        const utility::string_t exchange = U(""), long contract_id = -1);

OPENAPI_EXPORT Contract option_contract(const utility::string_t symbol, const utility::string_t expiry, double strike, const utility::string_t right,
                         const utility::string_t currency, long multiplier = 100, const utility::string_t local_symbol = U(""),
                         long contract_id = 0);
OPENAPI_EXPORT Contract option_contract(const utility::string_t identifier, long multiplier = 100, const utility::string_t currency = U("USD"));

OPENAPI_EXPORT Contract future_contract(const utility::string_t symbol, const utility::string_t currency, const utility::string_t expiry = U(""),
                         const utility::string_t exchange = U(""), const utility::string_t contract_month = U(""), long multiplier = 0,
                         const utility::string_t local_symbol = U(""));

/**
 * 期权要素信息提取
 * @param identifier 期权标识符，如 U("AAPL 220414C0017500")， U("JD 211015P00045000")
 * @return 正股代码，到期日，行权方向，行权价格 组成的tuple， (underlying_symbol, expiry, right, strike)
 */
OPENAPI_EXPORT std::tuple<utility::string_t, utility::string_t, utility::string_t, double> extract_option_info(const utility::string_t identifier);


#endif //TIGERAPI_CONTRACT_UTIL_H
