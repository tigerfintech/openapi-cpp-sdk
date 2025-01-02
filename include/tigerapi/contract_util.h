//
// Created by sukai on 2022/12/27.
//

#ifndef TIGERAPI_CONTRACT_UTIL_H
#define TIGERAPI_CONTRACT_UTIL_H


#include <tuple>
#include "model.h"
#include "win32.h"

namespace TIGER_API {

    class OPENAPI_EXPORT ContractUtil {
    public:
        static Contract stock_contract(const utility::string_t symbol, const utility::string_t currency,
                                               const utility::string_t local_symbol = U(""),
                                               const utility::string_t exchange = U(""), long contract_id = -1);

        static Contract
        option_contract(const utility::string_t symbol, const utility::string_t expiry, const utility::string_t strike,
                        const utility::string_t right,
                        const utility::string_t currency = U("USD"), long multiplier = 100,
                        const utility::string_t local_symbol = U(""),
                        long contract_id = 0);

        static Contract option_contract(const utility::string_t identifier, long multiplier = 100,
                                                const utility::string_t currency = U("USD"));

        static Contract future_contract(const utility::string_t symbol, const utility::string_t currency,
                                                const utility::string_t expiry = U(""),
                                                const utility::string_t exchange = U(""),
                                                const utility::string_t contract_month = U(""), long multiplier = 0,
                                                const utility::string_t local_symbol = U(""));

/**
 * Extract option elements
 * @param identifier Option identifier, e.g. U("AAPL 220414C0017500")， U("JD 211015P00045000")
 * @return Tuple of underlying symbol, expiry, right, strike (underlying_symbol, expiry, right, strike)
 */
        static std::tuple<utility::string_t, utility::string_t, utility::string_t, utility::string_t>
        extract_option_info(const utility::string_t identifier);
    };
}
#endif //TIGERAPI_CONTRACT_UTIL_H
