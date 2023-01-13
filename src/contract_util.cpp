//
// Created by sukai on 2022/12/27.
//

#include "../include/tigerapi/contract_util.h"

#include <regex>
#include <string>


Contract stock_contract(const utility::string_t symbol, const utility::string_t currency, const utility::string_t local_symbol, const utility::string_t exchange,
                        long contract_id) {
    return Contract(U("STK"), symbol, currency,  local_symbol, exchange, contract_id);
}

Contract option_contract(const utility::string_t symbol, const utility::string_t expiry, double strike, const utility::string_t right,
                         const utility::string_t currency, long multiplier, const utility::string_t local_symbol, long contract_id) {
    return Contract( U("OPT"), symbol,expiry, strike, right, currency, multiplier, local_symbol, contract_id);
}

Contract option_contract(const utility::string_t identifier, long multiplier, const utility::string_t currency) {
    utility::string_t  symbol, expiry, right;
    double strike;
    std::tie(symbol, expiry, right, strike) = extract_option_info(identifier);
    if (!expiry.empty() && expiry.find('-') != utility::string_t ::npos) {
        expiry.erase(std::remove(expiry.begin(), expiry.end(), '-'), expiry.end());
    }
    return Contract(U("OPT"), symbol, expiry, strike, right, currency, multiplier, U(""), 0);
}

Contract future_contract(const utility::string_t symbol, const utility::string_t currency, const utility::string_t expiry,
                         const utility::string_t exchange, const utility::string_t contract_month, long multiplier,
                         const utility::string_t local_symbol) {
    return Contract(U("FUT"), symbol, expiry, multiplier, contract_month, currency, exchange, local_symbol);
}


std::tuple<utility::string_t , utility::string_t , utility::string_t , double> extract_option_info(const utility::string_t  identifier) {
    if (!identifier.empty()) {
        std::smatch tokens;
        std::regex re(R"((\w+)\s*(\d{6})([CP])(\d+))");
        if (std::regex_match(identifier, tokens, re)) {
            utility::string_t  underlying_symbol = tokens[1];
            utility::string_t  expiry = tokens[2];
            expiry = U("20") + expiry;
            if (expiry.size() == 8) {
                expiry = expiry.substr(0, 4) + U("-") + expiry.substr(4, 2) + U("-") + expiry.substr(6);
            }
            utility::string_t  put_call = (tokens[3] == U("C")) ? U("CALL") : U("PUT");
            double strike = std::stod(tokens[4]) / 1000;
            return std::make_tuple(underlying_symbol, expiry, put_call, strike);
        } else {
            return std::make_tuple(U(""), U(""), U(""), 0.0);
        }
    } else {
        return std::make_tuple(U(""), U(""), U(""), 0.0);
    }
}




