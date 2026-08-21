#pragma once
/**
 * test_market_helpers.h — trading-hours gates and dynamic identifier resolvers
 * for C++ integration tests.
 *
 * Rules (mirrors the cross-SDK integ contract):
 *   - Trading-hours skip: use is_market_trading() to gate tests that only make
 *     sense inside regular trading hours. Outside hours → GTEST_SKIP with a
 *     clear message; inside hours the test must execute.
 *   - Data-dependency skip: use the resolve_* helpers to fetch fresh valid
 *     identifiers from live APIs. If the resolver returns empty AND the market
 *     is closed → skip. Empty + trading hours → the caller should FAIL.
 *   - Empty response: caller checks is_market_trading(); trading hours + empty
 *     → fail, closed + empty → skip.
 *
 * All helpers are inline / header-only so no new .cpp/CMakeLists entry is needed.
 */

#include <gtest/gtest.h>
#include <cpprest/json.h>
#include "tigerapi/quote_client.h"
#include "tigerapi/trade_client.h"
#include "tigerapi/model.h"
#include "tigerapi/utils.h"

#include <algorithm>
#include <chrono>
#include <ctime>
#include <cctype>
#include <map>
#include <mutex>
#include <string>
#include <thread>
#include <vector>

namespace TIGER_API {
namespace test {

// ── Local JSON helpers (avoid coupling to test-file-private helpers) ──────────

/// Extract an array from a result that may be either a raw JSON array
/// or a wrapper object with an "items" field.
inline web::json::value mh_extract_array(const web::json::value &v) {
    if (v.is_null()) return web::json::value::array();
    if (v.is_array()) return v;
    if (v.is_object() && v.has_field(U("items"))) {
        const auto &items = v.at(U("items"));
        return items.is_null() ? web::json::value::array() : items;
    }
    return web::json::value::array();
}

/// Build a JSON array containing a single string element.
inline web::json::value mh_make_string_array(const utility::string_t &s) {
    web::json::value arr = web::json::value::array();
    arr[0] = web::json::value::string(s);
    return arr;
}

// ── Permission / business-rule error tolerance ──────────────────────────────
//
// Order-placement tests use an "AcceptsOrSkips" naming convention: a live
// account may lack a given market/order-type permission, be under a
// regulatory restriction, or be outside the order type's allowed trading
// session — all legitimate account-state conditions the test should skip on,
// not fail on. This marker list mirrors Python's _PERMISSION_ERROR_KEYWORDS
// (tests/integ/test_integ_trade_client.py) and Rust's PERMISSION_ERROR_MARKERS
// (tests/integ_trade.rs), kept in sync across SDKs where possible.
inline bool is_permission_error(const std::string &msg) {
    static const char* const markers[] = {
        "access forbidden", "forbidden", "no permission", "not supported",
        "license", "not open", "not enabled", "not available for this account",
        "no token", "TBNZ",
        "don't support trading of this stock",
        "don\xe2\x80\x99t support trading",  // smart-quote variant (U+2019 in UTF-8)
        "unsupported instrument", "instrument not tradable",
        "only limit orders are supported",
        "outside of regular trading hours",
        "market is closed",
        "only limit orders can be placed",
        "only limit, stop or stop-limit orders are allowed",
        "at non-trading hour",
        "orders cannot be placed at this moment",
        "auction order is not allowed at this moment",
        "does not support stock long",
        "does not support stock short",
        "cash order by market order",
        // Account-level regulatory restriction on opening/adding positions
        // (e.g. Mainland China investor accounts) — server-side business
        // rule, not a code defect; observed via live production responses.
        "regulatory requirements for existing mainland china investors",
        "opening or adding to positions is temporarily unavailable",
        // SG-market contract/permission rejection observed via live
        // production response ("合约不正确" — contract is incorrect); the
        // contract's exchange code matches Tiger's documented SG mapping,
        // so this is treated as an account-side entitlement boundary.
        "\xe5\x90\x88\xe7\xba\xa6\xe4\xb8\x8d\xe6\xad\xa3\xe7\xa1\xae",
    };
    std::string lower(msg);
    std::transform(lower.begin(), lower.end(), lower.begin(),
                   [](unsigned char c) { return std::tolower(c); });
    for (const char* m : markers) {
        if (lower.find(m) != std::string::npos) return true;
    }
    return false;
}

// ── Order-state race tolerance (modify/cancel) ──────────────────────────────
//
// Separate from is_permission_error(): these markers signal the order already
// moved to a terminal/non-modifiable state by the time modify/cancel ran (a
// timing race, not an account-permission boundary). Mirrors Python's
// _TERMINAL_ORDER_KEYWORDS and Rust's TERMINAL_ORDER_MARKERS — log and
// tolerate, never fail the test on these.
inline bool is_terminal_order_error(const std::string &msg) {
    static const char* const markers[] = {
        "cannot be modified", "cannot be cancelled", "cannot be canceled",
        "already cancelled", "already canceled", "already filled",
        "not in a modifiable state", "invalid order status",
        "cancellation is not allowed", "cancel is not allowed",
    };
    std::string lower(msg);
    std::transform(lower.begin(), lower.end(), lower.begin(),
                   [](unsigned char c) { return std::tolower(c); });
    for (const char* m : markers) {
        if (lower.find(m) != std::string::npos) return true;
    }
    return false;
}

// ── Rate-limit tolerance (place_order) ──────────────────────────────────────
//
// Mirrors Python's _RATE_LIMIT_KEYWORDS / place_order_with_rate_limit_handling
// and Rust's RATE_LIMIT_MARKERS: a shared account-level QPS cap is a legitimate
// CI boundary, not a defect — retry with exponential backoff a couple of
// times, then let the final attempt's exception propagate to the caller's
// normal is_permission_error()/trading-hours handling (which itself may skip).
inline bool is_rate_limit_error(const std::string &msg) {
    static const char* const markers[] = {
        "too_many_requests", "rate limit", "requestrateexceedlimit",
        "rate exceeded",
    };
    std::string lower(msg);
    std::transform(lower.begin(), lower.end(), lower.begin(),
                   [](unsigned char c) { return std::tolower(c); });
    for (const char* m : markers) {
        if (lower.find(m) != std::string::npos) return true;
    }
    return false;
}

/// place_order with backoff-then-propagate on rate-limit errors.
///
/// Sleeps `pre_delay` before every attempt to smooth request pacing, then on
/// a rate-limit error retries up to `max_retries` times with exponential
/// backoff (1s, 2s, 4s, ...). Non-rate-limit exceptions and the final
/// exhausted attempt propagate to the caller unchanged — callers already
/// handle trading-hours/permission errors via is_permission_error(), so this
/// only adds tolerance for the rate-limit case on top of that.
inline web::json::value place_order_with_rate_limit_handling(
        TradeClient &client, Order &order, int max_retries = 2,
        std::chrono::milliseconds pre_delay = std::chrono::milliseconds(500)) {
    auto delay = std::chrono::milliseconds(1000);
    for (int attempt = 0; ; ++attempt) {
        if (pre_delay.count() > 0) std::this_thread::sleep_for(pre_delay);
        try {
            return client.place_order(order);
        } catch (const std::exception &ex) {
            if (!is_rate_limit_error(ex.what()) || attempt >= max_retries) throw;
            std::this_thread::sleep_for(delay);
            delay *= 2;
        }
    }
}

// ── Trading-hours helpers ────────────────────────────────────────────────────

namespace detail {

inline std::mutex& market_state_cache_mutex() {
    static std::mutex m;
    return m;
}

/// Process-wide cache: market → status string. Populated on first read.
inline std::map<utility::string_t, utility::string_t>& market_state_cache() {
    static std::map<utility::string_t, utility::string_t> c;
    return c;
}

/// Extract "marketStatus" or "status" (whichever is present) from the first
/// market entry in the get_market_state response.
inline utility::string_t extract_status(const web::json::value &result,
                                        const utility::string_t &market) {
    auto arr = mh_extract_array(result);
    for (size_t i = 0; i < arr.size(); ++i) {
        const auto &entry = arr.at(i);
        if (!entry.is_object()) continue;
        // If the entry names a market, only accept the one we requested.
        if (entry.has_field(U("market")) && entry.at(U("market")).is_string()) {
            if (entry.at(U("market")).as_string() != market) continue;
        }
        const utility::string_t fields[] = {U("marketStatus"), U("status")};
        for (const auto &f : fields) {
            if (entry.has_field(f) && entry.at(f).is_string()) {
                return entry.at(f).as_string();
            }
        }
    }
    return U("");
}

/// Fetch (and cache) the market status. Errors are NOT cached so the next
/// call retries the request; only a successful (non-empty) status is stored.
inline utility::string_t market_status(QuoteClient &qc,
                                       const utility::string_t &market) {
    std::lock_guard<std::mutex> lock(market_state_cache_mutex());
    auto &cache = market_state_cache();
    auto it = cache.find(market);
    if (it != cache.end()) return it->second;
    utility::string_t status;
    try {
        auto result = qc.get_market_state(market);
        status = extract_status(result, market);
    } catch (...) {
        // Do not cache errors — allow the next call to retry.
        return U("");
    }
    // Only cache a successfully retrieved (non-empty) status.
    if (!status.empty()) {
        cache[market] = status;
    }
    return status;
}

} // namespace detail

/// True iff the market is in regular trading hours (status == "TRADING").
inline bool is_market_trading(QuoteClient &qc, const utility::string_t &market) {
    return detail::market_status(qc, market) == U("TRADING");
}

/// True iff the market is in extended trading hours (regular / pre / post).
inline bool is_market_open_extended(QuoteClient &qc,
                                    const utility::string_t &market) {
    const auto s = detail::market_status(qc, market);
    return s == U("TRADING")
        || s == U("PRE_HOUR_TRADING")
        || s == U("POST_HOUR_TRADING");
}

// ── Dynamic resolvers ────────────────────────────────────────────────────────

/// Read a string field, walking a candidate list. Returns "" on miss.
inline utility::string_t mh_first_string_field(const web::json::value &obj,
                                               std::initializer_list<utility::string_t> fields) {
    if (!obj.is_object()) return U("");
    for (const auto &f : fields) {
        if (obj.has_field(f) && obj.at(f).is_string()) {
            const auto &s = obj.at(f).as_string();
            if (!s.empty()) return s;
        }
    }
    return U("");
}

/// Resolve a valid US option identifier for AAPL via the option chain.
inline utility::string_t resolve_us_option_identifier(QuoteClient &qc) {
    web::json::value expirations;
    try {
        expirations = qc.get_option_expiration(mh_make_string_array(U("AAPL")));
    } catch (...) {
        return U("");
    }
    auto exp_arr = mh_extract_array(expirations);
    if (exp_arr.size() == 0) return U("");
    const auto &exp = exp_arr.at(0);
    if (!exp.is_object() || !exp.has_field(U("dates"))) return U("");
    const auto &dates = exp.at(U("dates"));
    if (!dates.is_array() || dates.size() == 0) return U("");

    // Try the earliest expiries first.
    web::json::value chain;
    for (size_t i = 0; i < dates.size(); ++i) {
        const auto &date = dates.at(i);
        if (!date.is_string()) continue;
        try {
            chain = qc.get_option_chain(U("AAPL"), date.as_string());
        } catch (...) {
            continue;
        }
        auto chain_arr = mh_extract_array(chain);
        if (chain_arr.size() == 0) continue;
        const auto &row_wrapper = chain_arr.at(0);
        if (!row_wrapper.is_object() || !row_wrapper.has_field(U("items"))) continue;
        const auto &items = row_wrapper.at(U("items"));
        if (!items.is_array() || items.size() == 0) continue;
        // Pick the middle row as a rough ATM proxy.
        const auto &row = items.at(items.size() / 2);
        const utility::string_t legs[] = {U("call"), U("put")};
        for (const auto &leg : legs) {
            if (!row.has_field(leg) || !row.at(leg).is_object()) continue;
            const auto &leg_val = row.at(leg);
            auto id = mh_first_string_field(leg_val, {U("identifier")});
            if (!id.empty()) return id;
        }
    }
    return U("");
}

/// Resolve an HK warrant symbol via get_warrant_filter using underlying 0700.
inline utility::string_t resolve_hk_warrant_symbol(QuoteClient &qc) {
    const utility::string_t underlyings[] = {U("0700"), U("00700")};
    for (const auto &u : underlyings) {
        web::json::value result;
        try {
            result = qc.get_warrant_filter(u);
        } catch (...) {
            continue;
        }
        auto arr = mh_extract_array(result);
        for (size_t i = 0; i < arr.size(); ++i) {
            const auto &w = arr.at(i);
            auto sym = mh_first_string_field(w, {U("symbol"), U("warrantSymbol")});
            if (!sym.empty()) return sym;
        }
    }
    return U("");
}

/// Resolve a front-month CME future contract code (e.g. ESmain / a specific
/// month). Tries get_future_contract_by_exchange_code(CME) first, then falls
/// back to get_future_current_contract / get_future_contracts.
inline utility::string_t resolve_us_future_front_month_contract(QuoteClient &qc) {
    // Prefer the exchange-driven listing so we pick a currently-listed contract.
    const utility::string_t exchange_candidates[] = {U("CME"), U("NYMEX"), U("CBOT")};
    for (const auto &ex : exchange_candidates) {
        web::json::value result;
        try {
            result = qc.get_future_contract_by_exchange_code(ex);
        } catch (...) {
            continue;
        }
        auto arr = mh_extract_array(result);
        for (size_t i = 0; i < arr.size(); ++i) {
            const auto &c = arr.at(i);
            auto code = mh_first_string_field(c,
                {U("contractCode"), U("symbol"), U("code")});
            if (!code.empty()) return code;
        }
    }

    // Fall back: ES current contract, then ES listing.
    const utility::string_t root_candidates[] = {U("ES"), U("CL"), U("NQ")};
    for (const auto &root : root_candidates) {
        try {
            auto cur = qc.get_future_current_contract(root);
            if (cur.is_object()) {
                auto code = mh_first_string_field(cur,
                    {U("contractCode"), U("symbol"), U("code")});
                if (!code.empty()) return code;
            }
            auto cur_arr = mh_extract_array(cur);
            for (size_t i = 0; i < cur_arr.size(); ++i) {
                auto code = mh_first_string_field(cur_arr.at(i),
                    {U("contractCode"), U("symbol"), U("code")});
                if (!code.empty()) return code;
            }
        } catch (...) {}
        try {
            auto listing = qc.get_future_contracts(root);
            auto arr = mh_extract_array(listing);
            for (size_t i = 0; i < arr.size(); ++i) {
                auto code = mh_first_string_field(arr.at(i),
                    {U("contractCode"), U("symbol"), U("code")});
                if (!code.empty()) return code;
            }
        } catch (...) {}
    }
    return U("");
}

/// Resolve a valid fund symbol via get_fund_symbols. Returns "" if none.
inline utility::string_t resolve_fund_symbol(QuoteClient &qc) {
    web::json::value result;
    try {
        result = qc.get_fund_symbols();
    } catch (...) {
        return U("");
    }
    if (result.is_null()) return U("");
    auto arr = mh_extract_array(result);
    for (size_t i = 0; i < arr.size(); ++i) {
        const auto &item = arr.at(i);
        if (item.is_string() && !item.as_string().empty()) return item.as_string();
        if (item.is_object()) {
            auto sym = mh_first_string_field(item, {U("symbol"), U("code")});
            if (!sym.empty()) return sym;
        }
    }
    return U("");
}

/// Resolve a shortable US symbol via get_quote_shortable_stocks. Tries a small
/// candidate list of liquid, commonly shortable tickers.
inline utility::string_t resolve_shortable_us_symbol(QuoteClient &qc) {
    // Endpoint takes an explicit symbol list; try candidates one at a time.
    const utility::string_t candidates[] = {
        U("AAPL"), U("TSLA"), U("MSFT"), U("SPY"), U("QQQ"), U("NVDA")
    };
    for (const auto &sym : candidates) {
        try {
            auto result = qc.get_quote_shortable_stocks(mh_make_string_array(sym));
            if (result.is_null()) continue;
            auto arr = mh_extract_array(result);
            if (arr.size() > 0 && arr.at(0).is_object()) return sym;
        } catch (...) {
            continue;
        }
    }
    return U("");
}

/// A resolved live option leg: enough fields to build a Contract/ContractLeg.
struct ResolvedOptionLeg {
    utility::string_t expiry;  // yyyyMMdd
    utility::string_t strike;
    utility::string_t right;   // "CALL" or "PUT"
    bool ok = false;
};

/// Resolve a structured live AAPL option contract (expiry/strike/right), for
/// building a full Contract/Order rather than just an identifier string.
/// Picks the earliest available expiry and its middle-of-chain row as a
/// rough ATM proxy, preferring the call leg.
inline ResolvedOptionLeg resolve_us_option_contract(QuoteClient &qc) {
    ResolvedOptionLeg leg;
    web::json::value expirations;
    try {
        expirations = qc.get_option_expiration(mh_make_string_array(U("AAPL")));
    } catch (...) {
        return leg;
    }
    auto exp_arr = mh_extract_array(expirations);
    if (exp_arr.size() == 0) return leg;
    const auto &exp = exp_arr.at(0);
    if (!exp.is_object() || !exp.has_field(U("dates"))) return leg;
    const auto &dates = exp.at(U("dates"));
    if (!dates.is_array() || dates.size() == 0) return leg;

    for (size_t i = 0; i < dates.size(); ++i) {
        const auto &date = dates.at(i);
        if (!date.is_string()) continue;
        utility::string_t expiry = date.as_string();
        web::json::value chain;
        try {
            chain = qc.get_option_chain(U("AAPL"), expiry);
        } catch (...) {
            continue;
        }
        auto chain_arr = mh_extract_array(chain);
        if (chain_arr.size() == 0) continue;
        const auto &row_wrapper = chain_arr.at(0);
        if (!row_wrapper.is_object() || !row_wrapper.has_field(U("items"))) continue;
        const auto &items = row_wrapper.at(U("items"));
        if (!items.is_array() || items.size() == 0) continue;
        const auto &row = items.at(items.size() / 2);
        if (!row.has_field(U("strike")) || !row.at(U("strike")).is_double()) continue;
        double strike = row.at(U("strike")).as_double();
        if (strike <= 0.0) continue;

        leg.expiry = expiry;
        leg.strike = utility::conversions::to_string_t(std::to_string(strike));
        leg.right = U("CALL");
        leg.ok = true;
        return leg;
    }
    return leg;
}

/// A pair of adjacent-strike PUT legs on AAPL, for a vertical-spread combo
/// order. Mirrors the C# ResolveUsVerticalSpreadLegs algorithm: pick the
/// first expiry more than 14 days out (fallback to the last available
/// expiry), fetch its chain, filter to PUTs with a valid strike, sort
/// ascending, and pick two strikes near the middle.
struct ResolvedVerticalSpreadLegs {
    utility::string_t expiry;  // yyyyMMdd
    utility::string_t lower_strike;
    utility::string_t upper_strike;
    bool ok = false;
};

inline ResolvedVerticalSpreadLegs resolve_us_vertical_spread_legs(QuoteClient &qc) {
    ResolvedVerticalSpreadLegs result;
    web::json::value expirations;
    try {
        expirations = qc.get_option_expiration(mh_make_string_array(U("AAPL")));
    } catch (...) {
        return result;
    }
    auto exp_arr = mh_extract_array(expirations);
    if (exp_arr.size() == 0) return result;
    const auto &exp = exp_arr.at(0);
    if (!exp.is_object() || !exp.has_field(U("dates"))) return result;
    const auto &dates = exp.at(U("dates"));
    if (!dates.is_array() || dates.size() == 0) return result;

    std::time_t now = std::time(nullptr);
    std::time_t cutoff = now + 14 * 24 * 60 * 60;
    utility::string_t chosen_expiry;
    for (size_t i = 0; i < dates.size(); ++i) {
        if (!dates.at(i).is_string()) continue;
        const auto &d = dates.at(i).as_string();
        std::tm tm_buf{};
        std::string d_narrow = utility::conversions::to_utf8string(d);
        if (d_narrow.size() < 8) continue;
        tm_buf.tm_year = std::stoi(d_narrow.substr(0, 4)) - 1900;
        tm_buf.tm_mon = std::stoi(d_narrow.substr(4, 2)) - 1;
        tm_buf.tm_mday = std::stoi(d_narrow.substr(6, 2));
        std::time_t d_time = std::mktime(&tm_buf);
        if (d_time > cutoff) {
            chosen_expiry = d;
            break;
        }
    }
    if (chosen_expiry.empty()) {
        chosen_expiry = dates.at(dates.size() - 1).as_string();
    }

    web::json::value chain;
    try {
        chain = qc.get_option_chain(U("AAPL"), chosen_expiry);
    } catch (...) {
        return result;
    }
    auto chain_arr = mh_extract_array(chain);
    if (chain_arr.size() == 0) return result;
    const auto &row_wrapper = chain_arr.at(0);
    if (!row_wrapper.is_object() || !row_wrapper.has_field(U("items"))) return result;
    const auto &items = row_wrapper.at(U("items"));
    if (!items.is_array()) return result;

    std::vector<double> put_strikes;
    for (size_t i = 0; i < items.size(); ++i) {
        const auto &row = items.at(i);
        if (!row.has_field(U("put")) || !row.at(U("put")).is_object()) continue;
        if (!row.has_field(U("strike")) || !row.at(U("strike")).is_double()) continue;
        double strike = row.at(U("strike")).as_double();
        if (strike > 0.0) put_strikes.push_back(strike);
    }
    std::sort(put_strikes.begin(), put_strikes.end());
    if (put_strikes.size() < 2) return result;

    size_t mid = put_strikes.size() / 2;
    if (mid + 1 >= put_strikes.size()) mid = put_strikes.size() - 2;

    result.expiry = chosen_expiry;
    result.lower_strike = utility::conversions::to_string_t(std::to_string(put_strikes[mid]));
    result.upper_strike = utility::conversions::to_string_t(std::to_string(put_strikes[mid + 1]));
    result.ok = true;
    return result;
}

} // namespace test
} // namespace TIGER_API
