//
// Created by sukai on 2022/12/27.
//

#include "../include/tigerapi/contract_util.h"

#include <regex>
#include <string>


Contract stock_contract(const string symbol, const string currency, const string local_symbol, const string exchange,
                        long contract_id) {
    return Contract(U("STK"), symbol, currency,  local_symbol, exchange, contract_id);
}

Contract option_contract(const string symbol, const string expiry, double strike, const string right,
                         const string currency, long multiplier, const string local_symbol, long contract_id) {
    return Contract( U("OPT"), symbol,expiry, strike, right, currency, multiplier, local_symbol, contract_id);
}

Contract option_contract(const string identifier, long multiplier, const string currency) {
    std::string symbol, expiry, right;
    double strike;
    std::tie(symbol, expiry, right, strike) = extract_option_info(identifier);
    if (!expiry.empty() && expiry.find('-') != std::string::npos) {
        expiry.erase(std::remove(expiry.begin(), expiry.end(), '-'), expiry.end());
    }
    return Contract(U("OPT"), symbol, expiry, strike, right, currency, multiplier, U(""), 0);
}

Contract future_contract(const string symbol, const string currency, const string expiry,
                         const string exchange, const string contract_month, long multiplier,
                         const string local_symbol) {
    return Contract(U("FUT"), symbol, expiry, multiplier, contract_month, currency, exchange, local_symbol);
}


std::tuple<std::string, std::string, std::string, double> extract_option_info(const std::string identifier) {
    if (!identifier.empty()) {
        std::smatch tokens;
        std::regex re(R"((\w+)\s*(\d{6})([CP])(\d+))");
        if (std::regex_match(identifier, tokens, re)) {
            std::string underlying_symbol = tokens[1];
            std::string expiry = tokens[2];
            expiry = U("20") + expiry;
            if (expiry.size() == 8) {
                expiry = expiry.substr(0, 4) + U("-") + expiry.substr(4, 2) + U("-") + expiry.substr(6);
            }
            std::string put_call = (tokens[3] == U("C")) ? U("CALL") : U("PUT");
            double strike = std::stod(tokens[4]) / 1000;
            return std::make_tuple(underlying_symbol, expiry, put_call, strike);
        } else {
            return std::make_tuple(U(""), U(""), U(""), 0.0);
        }
    } else {
        return std::make_tuple(U(""), U(""), U(""), 0.0);
    }
}




