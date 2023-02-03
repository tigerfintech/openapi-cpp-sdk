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
#if defined(WIN32) || defined(_WIN32) || defined(__WIN32__) || defined(__NT__)
    auto trans_identifier = utility::conversions::utf16_to_utf8(identifier);
#else
    auto trans_identifier = identifier;
#endif
    if (!trans_identifier.empty()) {
        std::regex pattern(R"((\w+(?:\.\w+)?)\s*(\d{6})([CP])(\d+))");
        std::smatch matches;
        if (std::regex_search(trans_identifier, matches, pattern) && matches.size() == 5) {
            std::string underlying_symbol = matches[1].str();
            std::string expiry = "20" + matches[2].str();
            std::string right = matches[3];
            double strike = std::stod(matches[4]) / 1000;
            if (expiry.size() == 8) {
                expiry = expiry.substr(0, 4) + "-" + expiry.substr(4, 2) + "-" + expiry.substr(6);
            }
            right = (right == "C") ? "CALL" : "PUT";
#if defined(WIN32) || defined(_WIN32) || defined(__WIN32__) || defined(__NT__)

            return std::make_tuple(utility::conversions::utf8_to_utf16(underlying_symbol), 
                utility::conversions::utf8_to_utf16(expiry), 
                utility::conversions::utf8_to_utf16(right), 
                strike);
#else
            return std::make_tuple(underlying_symbol, expiry, right, strike);
#endif
        }
    }
    return std::make_tuple(U(""), U(""), U(""), 0.0);
}




