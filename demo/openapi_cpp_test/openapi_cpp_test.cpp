// openapi_cpp_test.cpp
#include <functional>
#include "tigerapi/push_client.h"
#include "tigerapi/tiger_client.h"
#include "tigerapi/quote_client.h"
#include "tigerapi/trade_client.h"
#include "tigerapi/contract_util.h"
#include "tigerapi/model.h"
#include "tigerapi/order_util.h"
#include "tigerapi/utils.h"
#include "tigerapi/properties.h"
#include "cpprest/details/basic_types.h"
#include "tigerapi/price_util.h"
#include <chrono>
#include <thread>
#include <fstream>
#include <cstdio>
#include <cstdlib>
#include <sstream>
#include "tigerapi/logger.h"

using namespace std;
using namespace web;
using namespace web::json;
using namespace TIGER_API;

namespace {
    std::string get_temp_directory() {
        const char* variables[] = {"TMPDIR", "TEMP", "TMP"};
        for (const char* variable : variables) {
            const char* value = std::getenv(variable);
            if (value && *value) {
                return value;
            }
        }
        return ".";
    }
}

/**
 * Test Trade Client
 */
class TestTradeClient {
public:
    static void test_get_prime_asset(const std::shared_ptr<TradeClient>& trade_client) {
        value res = trade_client->get_prime_asset();
        ucout << U("asset: ") << res << endl;
    }

    static void test_get_prime_portfolio(const std::shared_ptr<TradeClient>& trade_client) {
        PortfolioAccount res = trade_client->get_prime_portfolio();
        ucout << U("portfolio: ") << res.to_string() << endl;
    }

    static void test_get_asset(const std::shared_ptr<TradeClient>& trade_client) {
        value res = trade_client->get_asset();
        ucout << U("asset: ") << res << endl;
    }

    static void test_get_position(const std::shared_ptr<TradeClient>& trade_client) {
        value res = trade_client->get_positions();
        ucout << U("position: ") << res << endl;
    }

    static void test_get_position_list(const std::shared_ptr<TradeClient>& trade_client) {
        vector<Position> res = trade_client->get_position_list();
        ucout << U("position size: ") << res.size() << U(" , first item: ") << res[0].to_string() << endl;
    }

    static void test_get_orders(const std::shared_ptr<TradeClient>& trade_client) {
        value res = trade_client->get_orders();
        ucout << U("orders: ") << res << endl;
    }

    static void test_get_active_orders(const std::shared_ptr<TradeClient>& trade_client) {
        value res = trade_client->get_active_orders();
        ucout << U("active orders: ") << res << endl;
    }

    static void test_get_transactions(const std::shared_ptr<TradeClient>& trade_client) {
        value res = trade_client->get_transactions(trade_client->client_config.account, U("AAPL"));
        ucout << U("transactions: ") << res << endl;
    }

    static void test_get_contract(const std::shared_ptr<TradeClient>& trade_client) {
        value res = trade_client->get_contract(U("AAPL"));
        ucout << U("contract: ") << res << endl;
    }

    static void test_get_contracts(const std::shared_ptr<TradeClient>& trade_client) {
        value symbols = value::array();
        symbols[0] = value::string(U("AAPL"));
        symbols[1] = value::string(U("JD"));
        value res = trade_client->get_contracts(symbols, U("STK"));
        ucout << U("contracts: ") << res << endl;
    }

    static void test_get_quote_contract(const std::shared_ptr<TradeClient>& trade_client) {
        value res = trade_client->get_quote_contract(U("00700"), U("IOPT"), U("20230728"));
        ucout << U("quote contract: ") << res << endl;
    }

    static void test_get_estimate_tradable_quantity(const std::shared_ptr<TradeClient>& trade_client) {
        Contract contract = ContractUtil::stock_contract(U("AAPL"), U("USD"));
        Order order = OrderUtil::limit_order(contract, U("BUY"), 1, 100.0);
        value res = trade_client->get_estimate_tradable_quantity(order);
        ucout << U("estimate tradable quantity: ") << res << endl;
    }

    static void test_place_order(const std::shared_ptr<TradeClient>& trade_client) {
        Contract contract = ContractUtil::stock_contract(U("AAPL"), U("USD"));
        Order order = OrderUtil::limit_order(contract, U("BUY"), 1, 100.0);
        value res = trade_client->place_order(order);
        unsigned long long id = res[U("id")].as_number().to_uint64();
        ucout << U("return id: ") << id << endl;
        ucout << U("order id: ") << order.id << endl;
        ucout << U("place order result: ") << res << endl;
    }

    static void test_place_future_order(const std::shared_ptr<TradeClient>& trade_client) {
        Contract contract = ContractUtil::future_contract(U("NG2308"), U("USD"));
        Order order = OrderUtil::limit_order(contract, U("BUY"), 1, 1.5);
        value res = trade_client->place_order(order);
        unsigned long long id = res[U("id")].as_number().to_uint64();
        ucout << U("return id: ") << id << endl;
        ucout << U("order id: ") << order.id << endl;
        ucout << U("place order result: ") << res << endl;
    }

    static void test_place_option_order(const std::shared_ptr<TradeClient>& trade_client) {
        Contract contract = ContractUtil::option_contract(U("TQQQ"), U("20230915"), U("43.0"), U("CALL"), U("USD"));
        //Contract contract = ContractUtil::option_contract(U("AAPL"), U("20230721"), U("185.0"), U("PUT"), U("USD"));
        //Contract contract = ContractUtil::option_contract(U("AAPL 230721C00185000"));
        //Contract contract = ContractUtil::option_contract(U("AAPL 230721P00185000"));
        Order order = OrderUtil::limit_order(contract, U("BUY"), 3, U("0.18"));
        //order.adjust_limit = 0.01;
        value res = trade_client->place_order(order);                          
        //unsigned long long id = res[U("id")].as_number().to_uint64();
        ucout << U("order id: ") << order.id << endl;
        ucout << U("place order result: ") << res << endl;
    }
    
    static void test_get_order(const std::shared_ptr<TradeClient>& trade_client) {
        //Contract contract = stock_contract(U("AAPL"), U("USD"));
        //Order order = OrderUtil::limit_order(contract, U("BUY"), 1, 100.0);
        //trade_client->place_order(order);
        Order my_order = trade_client->get_order(31318009878020096);
        ucout << U("order id ") << my_order.id << endl;
        ucout << U("order : ") << my_order.to_string() << endl;
    }

    static void test_cancel_order(const std::shared_ptr<TradeClient>& trade_client) {
        value res = trade_client->cancel_order(31319396151853056);
        ucout << U("cancel order : ") << res << endl;
    }

    static void test_modify_order(const std::shared_ptr<TradeClient>& trade_client) {
        Contract contract = ContractUtil::stock_contract(U("AAPL"), U("USD"));
        Order order = OrderUtil::limit_order(contract, U("BUY"), 1, 100.0);
        long id = (long)trade_client->place_order(order)[U("id")].as_number().to_uint64();
        value res = trade_client->modify_order(order, 105);
        ucout << U("modify order res: ") << res << endl;
        Order mod_order = trade_client->get_order(id);
        ucout << U("modified order: ") << mod_order.to_string() << endl;
    }

    static void test_get_analytics_asset(const std::shared_ptr<TradeClient>& trade_client) {
        value res = trade_client->get_analytics_asset(trade_client->client_config.account, U("2023-11-01"),
                                                      U("2023-12-31"));
        ucout << U("analytics asset: ") << res << endl;
    }

    static void test_get_segment_fund_history(const std::shared_ptr<TradeClient>& trade_client) {
        value res = trade_client->get_segment_fund_history();
        ucout << U("segment fund history: ") << res << endl;
    }

    static void test_get_segment_fund_available(const std::shared_ptr<TradeClient>& trade_client) {
        value res = trade_client->get_segment_fund_available(U("SEC"));
        ucout << U("segment fund available: ") << res << endl;
    }

    static void test_transfer_segment_fund(const std::shared_ptr<TradeClient>& trade_client) {
        value res = trade_client->transfer_segment_fund(U("SEC"), U("FUT"), 10.0);
        ucout << U("transfer segment fund: ") << res << endl;
    }

    static void test_place_forex_order(const std::shared_ptr<TradeClient>& trade_client) {
        value res = trade_client->place_forex_order(U("SEC"), U("USD"), U("HKD"), 1.0);
        ucout << U("place forex order: ") << res << endl;
    }

    static void test_price_util(const std::shared_ptr<TradeClient>& trade_client) {
        value contract = trade_client->get_contract(U("AAPL"));
        value tick_sizes = contract.at( U("tickSizes") );
        double res = PriceUtil::fix_price_by_tick_size(1.111, tick_sizes);
        ucout << U("fix price by tick size: ") << res << endl;
    }

    // --- New Trade interfaces ---
    static void test_preview_order(const std::shared_ptr<TradeClient>& trade_client) {
        Contract contract = ContractUtil::stock_contract(U("AAPL"), U("USD"));
        Order order = OrderUtil::limit_order(contract, U("BUY"), 1, 100.0);
        value res = trade_client->preview_order(order);
        ucout << U("preview order: ") << res << endl;
    }

    static void test_get_aggregate_assets(const std::shared_ptr<TradeClient>& trade_client) {
        value res = trade_client->get_aggregate_assets();
        ucout << U("aggregate assets: ") << res << endl;
    }

    static void test_get_funding_history(const std::shared_ptr<TradeClient>& trade_client) {
        value res = trade_client->get_funding_history(U("SEC"));
        ucout << U("funding history: ") << res << endl;
    }

    static void test_get_fund_details(const std::shared_ptr<TradeClient>& trade_client) {
        value seg_types = value::array();
        seg_types[0] = value::string(U("SEC"));
        value res = trade_client->get_fund_details(seg_types);
        ucout << U("fund details: ") << res << endl;
    }

    static void test_get_position_transfer_records(const std::shared_ptr<TradeClient>& trade_client) {
        value res = trade_client->get_position_transfer_records(U("2025-01-01"), U("2025-01-31"));
        ucout << U("position transfer records: ") << res << endl;
    }

    static void test_get_accounts(const std::shared_ptr<TradeClient>& trade_client) {
        value res = trade_client->get_accounts();
        ucout << U("accounts: ") << res << endl;
    }

    static void test_get_filled_orders(const std::shared_ptr<TradeClient>& trade_client) {
        // start_date is required: use 90 days ago
        auto now = std::chrono::system_clock::now();
        auto now_ms = std::chrono::duration_cast<std::chrono::milliseconds>(now.time_since_epoch()).count();
        auto ninety_days_ago_ms = now_ms - 90LL * 24 * 3600 * 1000;
        value res = trade_client->get_filled_orders(U(""), U(""), U("ALL"), U(""), ninety_days_ago_ms, now_ms);
        ucout << U("filled orders: ") << res << endl;
    }

    static void test_get_inactive_orders(const std::shared_ptr<TradeClient>& trade_client) {
        value res = trade_client->get_inactive_orders();
        ucout << U("inactive orders: ") << res << endl;
    }

    static void test_trade(const std::shared_ptr<TradeClient>& trade_client) {
        TestTradeClient::test_place_order(trade_client);
    }

    static void test_option_exercise(const std::shared_ptr<TradeClient>& trade_client) {
        // Query positions available for exercise
        value positions = trade_client->get_option_exercise_positions(U("Exercise"));
        ucout << U("option exercise positions: ") << positions << endl;

        // Query exercise records
        value records = trade_client->get_option_exercise_records(U("Exercise"), U(""), U(""), U(""), 1, 20);
        ucout << U("option exercise records: ") << records << endl;

        // Preview exercise effect (replace contract_id with a real one from positions above)
        // value check = trade_client->check_option_exercise(1684414425LL, U("Exercise"), 1.0, U("2025-06-20"), 0);
        // ucout << U("option exercise check: ") << check << endl;
    }

};

/**
 * Test Quote Client
 */
class TestQuoteClient {
public:
    static void test_grab_quote_permission(std::shared_ptr<QuoteClient> quote_client) {
        value perms = quote_client->grab_quote_permission();
        ucout << U("Quote perms: ") << perms << endl;
    }

    static void test_get_symbols(std::shared_ptr<QuoteClient> quote_client) {
        value result = quote_client->get_symbols();
        ucout << U("result: ") << result << endl;
    }

    static void test_get_symbols_names(const std::shared_ptr<QuoteClient> quote_client) {
        //        value result = quote_client->get_all_symbol_names(U("HK"));
        value result = quote_client->get_all_symbol_names(Market::HK, false);
        ucout << U("result: ") << result << endl;
    }

    static void test_get_market_state(const std::shared_ptr<QuoteClient> quote_client) {
        value result = quote_client->get_market_state(U("HK"));
        ucout << U("result: ") << result << endl;
    }


    static void test_get_trading_calendar(const std::shared_ptr<QuoteClient> quote_client) {
        value result = quote_client->get_trading_calendar(U("US"), U("2023-01-01"), U("2023-03-01"));
        ucout << U("result: ") << result << endl;
    }

    static void test_get_timeline(const std::shared_ptr<QuoteClient> quote_client) {
        value symbols = value::array();
        symbols[0] = value::string(U("AAPL"));
        symbols[1] = value::string(U("JD"));
        value result = quote_client->get_timeline(symbols, false, 1675167178931);
        ucout << U("result: ") << result << endl;
    }

    static void test_get_history_timeline(const std::shared_ptr<QuoteClient> quote_client) {
        value symbols = value::array();
        symbols[0] = value::string(U("AAPL"));
        symbols[1] = value::string(U("JD"));
        value result = quote_client->get_history_timeline(symbols, U("2023-01-10"));
        ucout << U("result: ") << result << endl;
    }
    
    static void test_get_quote_real_time(const std::shared_ptr<QuoteClient> quote_client) {
        value symbols = value::array();
        symbols[0] = value::string(U("AAPL"));
        symbols[1] = value::string(U("JD"));
        auto result = quote_client->get_quote_real_time_value(symbols);
        vector<RealtimeQuote> result1 = quote_client->get_quote_real_time(symbols);
        ucout << U("result: ") << result.at(0).serialize() << endl;
    }

    static void test_get_quote_delay(const std::shared_ptr<QuoteClient> quote_client) {
        value symbols = value::array();
        symbols[0] = value::string(U("AAPL"));
        symbols[1] = value::string(U("JD"));
        value result = quote_client->get_quote_delay(symbols);
        ucout << U("result: ") << result << endl;
    }

    static void test_get_quote_depth(const std::shared_ptr<QuoteClient> quote_client) {
        value symbols = value::array();
        symbols[0] = value::string(U("AAPL"));
        symbols[1] = value::string(U("JD"));
        value result = quote_client->get_quote_depth(symbols);
        ucout << U("result: ") << result << endl;
    }

    static void test_get_shortable_stocks(const std::shared_ptr<QuoteClient> quote_client) {
        value symbols = value::array();
        symbols[0] = value::string(U("AAPL"));
        symbols[1] = value::string(U("JD"));
        value result = quote_client->get_quote_shortable_stocks(symbols);
        ucout << U("result: ") << result << endl;
    }

    static void test_get_kline(std::shared_ptr<QuoteClient> quote_client) {
        value symbols = value::array();
        symbols[0] = value::string(U("AAPL"));
        symbols[1] = value::string(U("JD"));
        //ucout << U("symbols ") << symbols << endl;
        //value result = quote_client->get_kline(symbols, U("day"), -1, -1, U("br"), 5);
        vector<Kline> result = quote_client->get_kline(symbols, U("day"), -1, -1, 5);
        ucout << result.at(0).to_string() << endl;
    }

    static void test_get_quote_stock_trade(std::shared_ptr<QuoteClient> quote_client) {
        value symbols = value::array();
        symbols[0] = value::string(U("AAPL"));
        symbols[1] = value::string(U("JD"));
        value result = quote_client->get_quote_stock_trade(symbols);
        ucout << U("Result: ") << result << endl;
    }

    static void test_get_trade_tick(std::shared_ptr<QuoteClient> quote_client) {
        value symbols = value::array();
        symbols[0] = value::string(U("AAPL"));
        symbols[1] = value::string(U("JD"));
        value result = quote_client->get_trade_tick(symbols, 0, 100000000);
        ucout << U("Result: ") << result << endl;
    }

    static void test_get_stock_broker(std::shared_ptr<QuoteClient> quote_client) {
        value result = quote_client->get_stock_broker(U("00700"), 40, U("zh_CN"), U("STK"));
        ucout << U("Result: ") << result << endl;
    }

    static void test_get_capital_flow(std::shared_ptr<QuoteClient> quote_client) {
        value result = quote_client->get_capital_flow(U("AAPL"), U("US"), U("intraday"));
        ucout << U("Result: ") << result << endl;
    }

    static void test_get_capital_flow_with_time(std::shared_ptr<QuoteClient> quote_client) {
        value result = quote_client->get_capital_flow(U("AAPL"), U("US"), U("day"), 1704067200000, -1, 10);
        ucout << U("Result: ") << result << endl;
    }

    static void test_get_capital_distribution(std::shared_ptr<QuoteClient> quote_client) {
        value result = quote_client->get_capital_distribution(U("AAPL"));
        ucout << U("Result: ") << result << endl;
    }

    static void test_get_capital_distribution_with_lang(std::shared_ptr<QuoteClient> quote_client) {
        value result = quote_client->get_capital_distribution(U("AAPL"), Market::US, U("zh_CN"));
        ucout << U("Result: ") << result << endl;
    }

    static void test_get_future_exchange(std::shared_ptr<QuoteClient> quote_client) {
        value result = quote_client->get_future_exchange();
        ucout << U("Result: ") << result << endl;
    }

    static void test_get_future_contracts(std::shared_ptr<QuoteClient> quote_client) {
        value result = quote_client->get_future_contracts(U("CL"));
        ucout << U("Result: ") << result << endl;
    }

    static void test_get_future_current_contract(std::shared_ptr<QuoteClient> quote_client) {
        value result = quote_client->get_future_current_contract(U("CL"));
        ucout << U("Result: ") << result << endl;
    }

    static void test_get_future_continuous_contracts(std::shared_ptr<QuoteClient> quote_client) {
        value result = quote_client->get_future_continuous_contracts(U("CL"));
        ucout << U("Result: ") << result << endl;
    }

    static void test_get_future_contract_by_contract_code(std::shared_ptr<QuoteClient> quote_client) {
        value result = quote_client->get_future_contract_by_contract_code(U("CL2303"));
        ucout << U("Result: ") << result << endl;
    }

    static void test_get_future_trading_date(std::shared_ptr<QuoteClient> quote_client) {
        value result = quote_client->get_future_trading_date(U("CL2303"), U("2023-01-12"));
        ucout << U("Result: ") << result << endl;
    }

    static void test_get_future_kline(std::shared_ptr<QuoteClient> quote_client) {
        value symbols = value::array();
        symbols[0] = value::string(U("CL2303"));
        ucout << U("symbols ") << symbols << endl;
//        value result = quote_client->get_future_kline(symbols);
        vector<Kline> result = quote_client->get_future_kline(symbols, BarPeriod::DAY);
        ucout << U("result: ") << result.at(0).to_string() << endl;
    }

    static void test_get_future_tick(std::shared_ptr<QuoteClient> quote_client) {
        value result = quote_client->get_future_tick(U("CL2312"));
        ucout << U("result: ") << result << endl;
    }

    static void test_get_future_real_time_quote(std::shared_ptr<QuoteClient> quote_client) {
        // Dynamically get current main contract code
        value current = quote_client->get_future_current_contract(U("CL"));
        if (!current.is_object() || !current.has_field(U("contractCode")) ||
            !current[U("contractCode")].is_string()) {
            throw std::runtime_error("Missing contractCode in future current contract response");
        }
        utility::string_t contract_code = current[U("contractCode")].as_string();
        ucout << U("Using current CL contract: ") << contract_code << endl;
        value symbols = value::array();
        symbols[0] = value::string(contract_code);
        auto result = quote_client->get_future_real_time_quote(symbols);
        if (!result.empty()) {
            ucout << U("result: ") << result.at(0).to_string() << endl;
        } else {
            ucout << U("result: empty (market may be closed)") << endl;
        }
    }

    static void test_get_option_expiration(std::shared_ptr<QuoteClient> quote_client) {
        value symbols = value::array();
        symbols[0] = value::string(U("TSLA"));
        ucout << U("symbols ") << symbols << endl;
        value result = quote_client->get_option_expiration(symbols);
        ucout << U("result: ") << result << endl;
    }

    static void test_get_option_chain(std::shared_ptr<QuoteClient> quote_client) {
        value result = quote_client->get_option_chain(U("TSLA"), U("2023-01-20"));
        ucout << U("result: ") << result << endl;
    }


    static void test_get_option_brief(std::shared_ptr<QuoteClient> quote_client) {
        value result = quote_client->get_option_brief(U("AAPL 230224C000150000"));
        ucout << U("result: ") << result << endl;
    }

    static void test_get_option_kline(std::shared_ptr<QuoteClient> quote_client) {
        // First try to get a valid identifier from option_brief for a known option
        // Use get_option_expiration to find future dates, then construct identifier
        value exp_symbols = value::array();
        exp_symbols[0] = value::string(U("AAPL"));
        value expirations = quote_client->get_option_expiration(exp_symbols);
        if (!expirations.is_array() || expirations.size() == 0 ||
            !expirations[0].is_object() || !expirations[0].has_field(U("dates")) ||
            !expirations[0][U("dates")].is_array()) {
            throw std::runtime_error("No valid option expiration dates returned");
        }
        value dates = expirations[0][U("dates")];
        utility::string_t identifier;

        // Try get_option_chain with multiple expiry dates (skip first 5 near-term)
        int tried = 0;
        for (size_t i = 5; i < dates.size() && identifier.empty(); i++) {
            utility::string_t expiry_date = dates[i].as_string();
            ucout << U("Trying option expiry: ") << expiry_date << endl;
            value chain = quote_client->get_option_chain(U("AAPL"), expiry_date);
            if (chain.is_array() && chain.size() > 0) {
                value items = chain[0][U("items")];
                if (items.is_array() && items.size() > 0) {
                    // items[i] has shape {"call": {..., "identifier": "..."}, "put": {...}}
                    value first = items[0];
                    if (first.has_field(U("call")) && first[U("call")].has_field(U("identifier"))) {
                        identifier = first[U("call")][U("identifier")].as_string();
                    } else if (first.has_field(U("put")) && first[U("put")].has_field(U("identifier"))) {
                        identifier = first[U("put")][U("identifier")].as_string();
                    }
                }
            }
            tried++;
            if (tried >= 5) break;
        }

        // Fallback: use get_option_brief to find any valid identifier
        if (identifier.empty()) {
            ucout << U("Chain returned no items, trying option_brief fallback...") << endl;
            // Get a future expiry date (pick the one ~30 days out)
            utility::string_t fallback_expiry;
            if (dates.size() > 8) {
                fallback_expiry = dates[8].as_string();
            } else if (dates.size() > 0) {
                fallback_expiry = dates[dates.size() - 1].as_string();
            }
            // Construct an approximate identifier: AAPL with strike ~250 (near current price)
            // Format: "AAPL  YYMMDDCSSTTTTTTT" - use option_brief with known format
            // Instead, just use a recent real-time quote to get current price, then find nearest strike
            value quote_symbols = value::array();
            quote_symbols[0] = value::string(U("AAPL"));
            auto quotes = quote_client->get_quote_real_time(quote_symbols);
            double current_price = 250.0; // default
            if (!quotes.empty()) {
                current_price = quotes[0].latest_price;
            }
            // Round to nearest 5
            int strike = ((int)(current_price / 5.0 + 0.5)) * 5;
            // Build OCC identifier: "AAPL  YYMMDDC00STRIKE00"
            // expiry format: YYYY-MM-DD -> YYMMDD
            if (!fallback_expiry.empty() && fallback_expiry.size() >= 10) {
                std::string yy = std::string(fallback_expiry.begin() + 2, fallback_expiry.begin() + 4);
                std::string mm = std::string(fallback_expiry.begin() + 5, fallback_expiry.begin() + 7);
                std::string dd = std::string(fallback_expiry.begin() + 8, fallback_expiry.begin() + 10);
                // Strike in format: 00000000 (8 digits, price * 1000)
                char strike_str[9];
                snprintf(strike_str, sizeof(strike_str), "%08d", strike * 1000);
                identifier = U("AAPL  ") + utility::string_t(yy.begin(), yy.end())
                    + utility::string_t(mm.begin(), mm.end())
                    + utility::string_t(dd.begin(), dd.end())
                    + U("C") + utility::string_t(strike_str, strike_str + 8);
                ucout << U("Constructed identifier: ") << identifier << endl;
                // Verify with option_brief
                value brief = quote_client->get_option_brief(identifier);
                if (brief.is_array() && brief.size() > 0 && brief[0].has_field(U("identifier"))) {
                    identifier = brief[0][U("identifier")].as_string();
                    ucout << U("Verified identifier: ") << identifier << endl;
                } else {
                    identifier.clear();
                }
            }
        }

        if (identifier.empty()) {
            throw std::runtime_error("Could not find any valid option identifier");
        }
        ucout << U("Using option identifier: ") << identifier << endl;
        value identifiers = value::array();
        identifiers[0] = value::string(identifier);
        // Use recent 30 days time range
        auto now = std::chrono::system_clock::now();
        auto now_ms = std::chrono::duration_cast<std::chrono::milliseconds>(now.time_since_epoch()).count();
        auto thirty_days_ago_ms = now_ms - 30LL * 24 * 3600 * 1000;
        vector<Kline> result = quote_client->get_option_kline(identifiers, thirty_days_ago_ms, now_ms);
        if (!result.empty()) {
            ucout << U("result: ") << result.at(0).to_string() << endl;
        } else {
            ucout << U("result: empty (no kline data in range)") << endl;
        }
    }

    static void test_get_option_trade_tick(std::shared_ptr<QuoteClient> quote_client) {
        value identifiers = value::array();
        identifiers[0] = value::string(U("AAPL 230224C000150000"));
        value result = quote_client->get_option_trade_tick(identifiers);
        ucout << U("result: ") << result << endl;
    }

    static void test_get_warrant_real_time_quote(std::shared_ptr<QuoteClient> quote_client) {
        value result = quote_client->get_warrant_real_time_quote(U("15792"));
        ucout << U("result: ") << result << endl;
    }

    static void test_get_warrant_filter(std::shared_ptr<QuoteClient> quote_client) {
        value result = quote_client->get_warrant_filter(U("00700"));
        ucout << U("result: ") << result << endl;
    }

    static void test_get_kline_quota(std::shared_ptr<QuoteClient> quote_client) {
        value result = quote_client->get_kline_quota();
        ucout << U("result: ") << result << endl;
    }

    static void test_get_addon_entitlements(std::shared_ptr<QuoteClient> quote_client) {
        value result = quote_client->get_addon_entitlements();
        ucout << U("result: ") << result << endl;
    }

    // --- New Option interfaces ---
    static void test_get_option_symbols(std::shared_ptr<QuoteClient> quote_client) {
        value result = quote_client->get_option_symbols(U("HK"));
        ucout << U("result: ") << result << endl;
    }

    static void test_get_option_depth(std::shared_ptr<QuoteClient> quote_client) {
        value symbols = value::array();
        value item = value::object(true);
        item[U("symbol")] = value::string(U("AAPL"));
        item[U("expiry")] = value::number(static_cast<int64_t>(1773932400000));
        item[U("right")] = value::string(U("CALL"));
        item[U("strike")] = value::string(U("200.0"));
        symbols[0] = item;
        value result = quote_client->get_option_depth(symbols, U("US"));
        ucout << U("result: ") << result << endl;
    }

    static void test_get_option_timeline(std::shared_ptr<QuoteClient> quote_client) {
        value symbols = value::array();
        value item = value::object(true);
        item[U("symbol")] = value::string(U("AAPL"));
        item[U("expiry")] = value::number(static_cast<int64_t>(1773932400000));
        item[U("right")] = value::string(U("CALL"));
        item[U("strike")] = value::string(U("200.0"));
        symbols[0] = item;
        value result = quote_client->get_option_timeline(symbols, U("US"));
        ucout << U("result: ") << result << endl;
    }

    static void test_get_option_analysis(std::shared_ptr<QuoteClient> quote_client) {
        value symbols = value::array();
        value item = value::object(true);
        item[U("symbol")] = value::string(U("AAPL"));
        item[U("period")] = value::string(U("52week"));
        symbols[0] = item;
        value result = quote_client->get_option_analysis(symbols, U("US"));
        ucout << U("result: ") << result << endl;
    }

    // --- New Future interfaces ---
    static void test_get_future_depth(std::shared_ptr<QuoteClient> quote_client) {
        value contract_codes = value::array();
        contract_codes[0] = value::string(U("CL2412"));
        value result = quote_client->get_future_depth(contract_codes);
        ucout << U("result: ") << result << endl;
    }

    static void test_get_future_history_main_contract(std::shared_ptr<QuoteClient> quote_client) {
        // This API expects type codes (like "CL"), not specific contract codes
        value type_codes = value::array();
        type_codes[0] = value::string(U("CL"));
        // Must provide time range
        auto now = std::chrono::system_clock::now();
        auto now_ms = std::chrono::duration_cast<std::chrono::milliseconds>(now.time_since_epoch()).count();
        auto one_year_ago_ms = now_ms - 365LL * 24 * 3600 * 1000;
        value result = quote_client->get_future_history_main_contract(type_codes, one_year_ago_ms, now_ms);
        ucout << U("result: ") << result << endl;
    }

    // --- New Fund interfaces ---
    static void test_get_fund_symbols(std::shared_ptr<QuoteClient> quote_client) {
        value result = quote_client->get_fund_symbols();
        ucout << U("result: ") << result << endl;
    }

    static void test_get_fund_contracts(std::shared_ptr<QuoteClient> quote_client) {
        value symbols = value::array();
        symbols[0] = value::string(U("IE00BYX5NX33.USD"));
        value result = quote_client->get_fund_contracts(symbols);
        ucout << U("result: ") << result << endl;
    }

    static void test_get_fund_quote(std::shared_ptr<QuoteClient> quote_client) {
        value symbols = value::array();
        symbols[0] = value::string(U("IE00BYX5NX33.USD"));
        value result = quote_client->get_fund_quote(symbols);
        ucout << U("result: ") << result << endl;
    }

    static void test_get_fund_history_quote(std::shared_ptr<QuoteClient> quote_client) {
        value symbols = value::array();
        symbols[0] = value::string(U("IE00BYX5NX33.USD"));
        value result = quote_client->get_fund_history_quote(symbols, 1704067200000, 1706745600000, 10);
        ucout << U("result: ") << result << endl;
    }

    // --- New Financial interfaces ---
    static void test_get_financial_currency(std::shared_ptr<QuoteClient> quote_client) {
        value symbols = value::array();
        symbols[0] = value::string(U("AAPL"));
        value result = quote_client->get_financial_currency(symbols, U("US"));
        ucout << U("result: ") << result << endl;
    }

    static void test_get_financial_exchange_rate(std::shared_ptr<QuoteClient> quote_client) {
        value currency_list = value::array();
        currency_list[0] = value::string(U("USD/CNH"));
        value result = quote_client->get_financial_exchange_rate(currency_list, U("2024-01-01"), U("2024-01-31"));
        ucout << U("result: ") << result << endl;
    }

    // --- Corporate Action: new types (SYMBOL_CHANGE / DELISTING / IPO) ---
    static void test_get_corporate_symbol_change(std::shared_ptr<QuoteClient> quote_client) {
        value symbols = value::array();
        symbols[0] = value::string(U("META"));
        value result = quote_client->get_corporate_symbol_change(symbols, U("US"), U("2022-01-01"), U("2023-01-01"));
        ucout << U("corporate_symbol_change: ") << result << endl;
    }
    static void test_get_corporate_delisting(std::shared_ptr<QuoteClient> quote_client) {
        value symbols = value::array();
        symbols[0] = value::string(U("TWTR"));
        value result = quote_client->get_corporate_delisting(symbols, U("US"), U("2022-01-01"), U("2023-01-01"));
        ucout << U("corporate_delisting: ") << result << endl;
    }
    static void test_get_corporate_ipo(std::shared_ptr<QuoteClient> quote_client) {
        value symbols = value::array();
        symbols[0] = value::string(U("RIVN"));
        value result = quote_client->get_corporate_ipo(symbols, U("US"), U("2021-01-01"), U("2022-01-01"));
        ucout << U("corporate_ipo: ") << result << endl;
    }

    // --- Other new quote interfaces ---
    static void test_get_stock_fundamental(std::shared_ptr<QuoteClient> quote_client) {
        value symbols = value::array();
        symbols[0] = value::string(U("AAPL"));
        symbols[1] = value::string(U("TSLA"));
        value result = quote_client->get_stock_fundamental(symbols, U("US"));
        ucout << U("result: ") << result << endl;
    }

    static void test_get_trade_rank(std::shared_ptr<QuoteClient> quote_client) {
        value result = quote_client->get_trade_rank(U("US"));
        ucout << U("result: ") << result << endl;
    }

    static void test_get_quote_overnight(std::shared_ptr<QuoteClient> quote_client) {
        value symbols = value::array();
        symbols[0] = value::string(U("AAPL"));
        value result = quote_client->get_quote_overnight(symbols);
        ucout << U("result: ") << result << endl;
    }

    static void test_get_broker_hold(std::shared_ptr<QuoteClient> quote_client) {
        value result = quote_client->get_broker_hold(U("HK"), U(""), U(""), 10);
        ucout << U("result: ") << result << endl;
    }

    static void test_get_market_scanner_tags(std::shared_ptr<QuoteClient> quote_client) {
        value multi_tags_fields = value::array();
        multi_tags_fields[0] = value::string(U("MultiTagField_Industry"));
        value result = quote_client->get_market_scanner_tags(U("US"), multi_tags_fields);
        ucout << U("result: ") << result << endl;
    }

    static void test_get_brief(std::shared_ptr<QuoteClient> quote_client) {
        value symbols = value::array();
        symbols[0] = value::string(U("AAPL"));
        symbols[1] = value::string(U("JD"));
        value result = quote_client->get_brief(symbols);
        ucout << U("result: ") << result << endl;
    }

    static void test_get_stock_detail(std::shared_ptr<QuoteClient> quote_client) {
        value symbols = value::array();
        symbols[0] = value::string(U("AAPL"));
        value result = quote_client->get_stock_detail(symbols);
        ucout << U("result: ") << result << endl;
    }

    static void test_get_industry_list(std::shared_ptr<QuoteClient> quote_client) {
        value result = quote_client->get_industry_list();
        ucout << U("result: ") << result << endl;
    }

    static void test_get_industry_stocks(std::shared_ptr<QuoteClient> quote_client) {
        value result = quote_client->get_industry_stocks(U("4520"), U("US"));
        ucout << U("result: ") << result << endl;
    }

    static void test_get_stock_industry(std::shared_ptr<QuoteClient> quote_client) {
        value result = quote_client->get_stock_industry(U("AAPL"), U("US"));
        ucout << U("result: ") << result << endl;
    }

    static void test_quote(const std::shared_ptr<QuoteClient> quote_client) {
        TestQuoteClient::test_get_kline(quote_client);
    }
};


/**
 * Directly use TigerApi
 */
class TestTigerApi {
public:
    static void test_grab_quote_permission(std::shared_ptr<TigerClient> tigerapi) {
        value obj = value::object(true);
        value perms = tigerapi->post(GRAB_QUOTE_PERMISSION, obj);
        ucout << U("quote perms: ") << perms << endl;
    }

    static void test_get_market_status(std::shared_ptr<TigerClient>  tigerapi) {
        value obj = value::object(true);
        obj[U("market")] = value::string(U("US"));
        tigerapi->post(MARKET_STATE, obj);
    }

    static void test_get_positions(std::shared_ptr<TigerClient>  tigerapi) {
        value obj = value::object(true);
        obj[U("market")] = value::string(U("US"));
        obj[P_ACCOUNT] = value::string(U("1111"));
        tigerapi->post(POSITIONS, obj);
    }
};

std::atomic<bool> keep_running(true);
void signal_handler(int signal)
{
    if (signal == SIGINT || signal == SIGTERM)
    {
        keep_running = false;
    }
}

class TestPushClient {
private:
    std::shared_ptr<IPushClient> push_client;
    std::vector<std::string> symbols;

public:
    TestPushClient(std::shared_ptr<IPushClient> client) : push_client(client) {
        std::vector<std::string> hk_option_symbols = {"TCH.HK 20241230 410.00 CALL"};
        std::vector<std::string> future_symbols = {"CL2412"};
        std::vector<std::string> stock_symbols = {"AAPL", "TSLA"};
        symbols = stock_symbols;
    }

    void connected_callback() {
        ucout << "Connected to push server" << std::endl;
        push_client->subscribe_position(utility::conversions::to_utf8string(push_client->get_client_config().account));
        push_client->subscribe_order(utility::conversions::to_utf8string(push_client->get_client_config().account));
        unsigned int asset_sub_id = push_client->subscribe_asset(utility::conversions::to_utf8string(push_client->get_client_config().account));
        ucout << "Subscribe asset result: " << asset_sub_id << std::endl;
        // push_client->query_subscribed_symbols();
        unsigned int res = push_client->subscribe_quote(symbols);
        ucout << "Subscribe quote result: " << res << std::endl;
        // push_client->subscribe_kline(symbols);
        // push_client->subscribe_quote_depth(symbols);
        unsigned int res1 = push_client->subscribe_tick(symbols);
        ucout << "Subscribe tick result: " << res1 << std::endl;
    }

    void error_callback(const tigeropen::push::pb::Response& data) {
        ucout << "Error callback: " << std::endl;
        ucout << "- code: " << data.code() << std::endl;
        ucout << "- msg: " << utility::conversions::to_string_t(data.msg()) << std::endl;
    }

    void kickout_callback(const tigeropen::push::pb::Response& data) {
        ucout << "Kickout callback: " << std::endl;
        ucout << "- code: " << data.code() << std::endl;
        ucout << "- msg: " << utility::conversions::to_string_t(data.msg()) << std::endl;

    }

    void position_changed_callback(const tigeropen::push::pb::PositionData& data) {
        ucout << "Position changed:" << std::endl;
        ucout << "- symbol: " << utility::conversions::to_string_t(data.symbol()) << std::endl;
        ucout << "- positionqty: " << data.positionqty() << std::endl;
    }

    void order_changed_callback(const tigeropen::push::pb::OrderStatusData& data) {
        // example: {"dataType":"OrderStatus","orderStatusData":{"id":"37129891133115200","account":"123123123","symbol":"PDD","identifier":"PDD","multiplier":1,
        // "action":"BUY","market":"US","currency":"USD","segType":"S","secType":"STK","orderType":"LMT","isLong":true,"totalQuantity":"1","limitPrice":50,
        // "status":"PendingSubmit","replaceStatus":"NONE","cancelStatus":"NONE","outsideRth":true,"canModify":true,"canCancel":true,"name":"PDD Holdings","source":"openapi","openTime":"1732177851000","timestamp":"1732177851874"}}
        ucout << "Order changed:" << std::endl;
        ucout << "- id: " << data.id() << std::endl;
        ucout << "- status: " << utility::conversions::to_string_t(data.status()) << std::endl;
        ucout << "- avgfillprice: " << data.avgfillprice() << std::endl;
    }

    void asset_changed_callback(const tigeropen::push::pb::AssetData& data) {
        // example: {"dataType":"Asset","assetData":{"account":"111111111111","segType":"S","availableFunds":797832.55572773679,
        // "excessLiquidity":826227.79308567208,"netLiquidation":944515.55984458746,"equityWithLoan":944494.85984458751,"buyingPower":3191330.2229109472,
        // "cashBalance":656046.6059349241,"grossPositionValue":288448.25390966347,"initMarginReq":146662.30411685078,"maintMarginReq":118287.76675891539,"timestamp":"1732177851891"}}
        ucout << "Asset changed:" << std::endl;
        ucout << "- cashbalance: " << data.cashbalance() << std::endl;
        ucout << "- netliquidation: " << data.netliquidation() << std::endl;
    }

    void tick_changed_callback(const TradeTick& data) {
        ucout << "TradeTick changed: " << std::endl;
        ucout << "- data: " << utility::conversions::to_string_t(data.to_string()) << std::endl;
    }

    void full_tick_changed_callback(const tigeropen::push::pb::TickData& data) {
        ucout << "Full TickData changed: " << std::endl;
        ucout << "- symbol: " << utility::conversions::to_string_t(data.symbol()) << std::endl;
        ucout << "- tick size: " << data.ticks_size() << std::endl;
    }

    void query_subscribed_symbols_changed_callback(const tigeropen::push::pb::Response& data) {
        ucout << "QuerySubscribedSymbols changed: " << std::endl;
        ucout << "- data: " << utility::conversions::to_string_t(data.msg()) << std::endl;
    }

    void quote_changed_callback(const tigeropen::push::pb::QuoteBasicData& data) {
        ucout << "BasicQuote changed: " << std::endl;
        ucout << "- symbol: " << utility::conversions::to_string_t(data.symbol()) << std::endl;
        ucout << "- latestPrice: " << data.latestprice() << std::endl;
        ucout << "- volume: " << data.volume() << std::endl;
    }

    void quote_bbo_changed_callback(const tigeropen::push::pb::QuoteBBOData& data) {
        ucout << "BBOQuote changed: " << std::endl;
        ucout << "- symbol: " << utility::conversions::to_string_t(data.symbol()) << std::endl;
        ucout << "- bidPrice: " << data.bidprice() << std::endl;
        ucout << "- askPrice: " << data.askprice() << std::endl;
    }

    void quote_depth_changed_callback(const tigeropen::push::pb::QuoteDepthData& data) {
        ucout << "QuoteDepth changed: " << std::endl;
        ucout << "- symbol: " << utility::conversions::to_string_t(data.symbol()) << std::endl;
        ucout << "- ask price size: " << data.ask().price_size() << std::endl;
        ucout << "- bid price size: " << data.bid().price_size() << std::endl;
    }

    void kline_changed_callback(const tigeropen::push::pb::KlineData& data) {
        ucout << "Kline changed: " << std::endl;
        ucout << "- symbol: " << utility::conversions::to_string_t(data.symbol()) << std::endl;
        ucout << "- open: " << data.open() << std::endl;
        ucout << "- high: " << data.high() << std::endl;
        ucout << "- low: " << data.low() << std::endl;
        ucout << "- close: " << data.close() << std::endl;
    }

    void start_test(ClientConfig config) {
        push_client->set_connected_callback(std::bind(&TestPushClient::connected_callback, this));
        push_client->set_error_callback(std::bind(&TestPushClient::error_callback, this, std::placeholders::_1));
        push_client->set_kickout_callback(std::bind(&TestPushClient::kickout_callback, this, std::placeholders::_1));
        push_client->set_position_changed_callback(std::bind(&TestPushClient::position_changed_callback, this, std::placeholders::_1));
        push_client->set_order_changed_callback(std::bind(&TestPushClient::order_changed_callback, this, std::placeholders::_1));
        push_client->set_asset_changed_callback(std::bind(&TestPushClient::asset_changed_callback, this, std::placeholders::_1));
        push_client->set_tick_changed_callback(std::bind(&TestPushClient::tick_changed_callback, this, std::placeholders::_1));
        push_client->set_full_tick_changed_callback(std::bind(&TestPushClient::full_tick_changed_callback, this, std::placeholders::_1));
        push_client->set_query_subscribed_symbols_changed_callback(std::bind(&TestPushClient::query_subscribed_symbols_changed_callback, this, std::placeholders::_1));
        push_client->set_quote_changed_callback(std::bind(&TestPushClient::quote_changed_callback, this, std::placeholders::_1));
        push_client->set_quote_bbo_changed_callback(std::bind(&TestPushClient::quote_bbo_changed_callback, this, std::placeholders::_1));
        push_client->set_quote_depth_changed_callback(std::bind(&TestPushClient::quote_depth_changed_callback, this, std::placeholders::_1));
        push_client->set_kline_changed_callback(std::bind(&TestPushClient::kline_changed_callback, this, std::placeholders::_1));


        push_client->connect();

        std::signal(SIGINT, signal_handler);  //Ctrl+C
        std::signal(SIGTERM, signal_handler); //kill
        while (keep_running)
        {
            std::this_thread::sleep_for(std::chrono::seconds(1));
        }

        push_client->unsubscribe_quote(symbols);
        push_client->unsubscribe_kline(symbols);
        push_client->unsubscribe_quote_depth(symbols);
        push_client->unsubscribe_tick(symbols);
        push_client->unsubscribe_asset(utility::conversions::to_utf8string(config.account));
        push_client->unsubscribe_position(utility::conversions::to_utf8string(config.account));
        push_client->unsubscribe_order(utility::conversions::to_utf8string(config.account));
        push_client->disconnect();
    }

    void start_test_with_timeout(ClientConfig config, int timeout_seconds = 10) {
        push_client->set_connected_callback(std::bind(&TestPushClient::connected_callback, this));
        push_client->set_error_callback(std::bind(&TestPushClient::error_callback, this, std::placeholders::_1));
        push_client->set_kickout_callback(std::bind(&TestPushClient::kickout_callback, this, std::placeholders::_1));
        push_client->set_position_changed_callback(std::bind(&TestPushClient::position_changed_callback, this, std::placeholders::_1));
        push_client->set_order_changed_callback(std::bind(&TestPushClient::order_changed_callback, this, std::placeholders::_1));
        push_client->set_asset_changed_callback(std::bind(&TestPushClient::asset_changed_callback, this, std::placeholders::_1));
        push_client->set_tick_changed_callback(std::bind(&TestPushClient::tick_changed_callback, this, std::placeholders::_1));
        push_client->set_full_tick_changed_callback(std::bind(&TestPushClient::full_tick_changed_callback, this, std::placeholders::_1));
        push_client->set_query_subscribed_symbols_changed_callback(std::bind(&TestPushClient::query_subscribed_symbols_changed_callback, this, std::placeholders::_1));
        push_client->set_quote_changed_callback(std::bind(&TestPushClient::quote_changed_callback, this, std::placeholders::_1));
        push_client->set_quote_bbo_changed_callback(std::bind(&TestPushClient::quote_bbo_changed_callback, this, std::placeholders::_1));
        push_client->set_quote_depth_changed_callback(std::bind(&TestPushClient::quote_depth_changed_callback, this, std::placeholders::_1));
        push_client->set_kline_changed_callback(std::bind(&TestPushClient::kline_changed_callback, this, std::placeholders::_1));

        ucout << "PushClient: connecting..." << std::endl;
        push_client->connect();

        ucout << "PushClient: waiting " << timeout_seconds << " seconds for data..." << std::endl;
        for (int i = 0; i < timeout_seconds; i++) {
            std::this_thread::sleep_for(std::chrono::seconds(1));
        }

        ucout << "PushClient: unsubscribing and disconnecting..." << std::endl;
        push_client->unsubscribe_quote(symbols);
        push_client->unsubscribe_tick(symbols);
        push_client->unsubscribe_asset(utility::conversions::to_utf8string(config.account));
        push_client->unsubscribe_position(utility::conversions::to_utf8string(config.account));
        push_client->unsubscribe_order(utility::conversions::to_utf8string(config.account));
        push_client->disconnect();
        ucout << "PushClient: disconnected." << std::endl;
    }

    static void test_push_client(std::shared_ptr<IPushClient> push_client, ClientConfig config) {
        TestPushClient test(push_client);
        test.start_test(config);
    }

    static void test_push_client_timed(std::shared_ptr<IPushClient> push_client, ClientConfig config, int timeout_seconds = 10) {
        TestPushClient test(push_client);
        test.start_test_with_timeout(config, timeout_seconds);
    }
};

// Test result tracking
struct TestResult {
    utility::string_t name;
    bool passed;
    bool skipped;
    utility::string_t error_msg;
};

template<typename Func>
TestResult run_test(const utility::string_t& name, Func func) {
    TestResult result;
    result.name = name;
    result.skipped = false;
    try {
        func();
        result.passed = true;
    } catch (const std::exception& e) {
        result.passed = false;
        result.error_msg = utility::conversions::to_string_t(e.what());
    } catch (...) {
        result.passed = false;
        result.error_msg = U("Unknown exception");
    }
    utility::string_t status = result.passed ? U("[PASS]") : U("[FAIL]");
    ucout << status << U(" ") << name;
    if (!result.passed) {
        ucout << U(" - ") << result.error_msg;
    }
    ucout << endl;
    return result;
}

TestResult skip_test(const utility::string_t& name, const utility::string_t& reason) {
    TestResult result;
    result.name = name;
    result.passed = false;
    result.skipped = true;
    result.error_msg = reason;
    ucout << U("[SKIP] ") << name << U(" - ") << reason << endl;
    return result;
}

void print_report(const std::vector<TestResult>& results, const std::string& report_file = "") {
    int total = results.size();
    int passed = 0, failed = 0, skipped = 0;
    for (const auto& r : results) {
        if (r.skipped) skipped++;
        else if (r.passed) passed++;
        else failed++;
    }

    // Build report string
    std::ostringstream oss;
    oss << std::endl;
    oss << "========================================" << std::endl;
    oss << "     OpenAPI C++ SDK Test Report         " << std::endl;
    oss << "     SDK Version: openapi-cpp-sdk-1.2.0  " << std::endl;
    oss << "========================================" << std::endl;
    oss << "Total: " << total << "  Passed: " << passed << "  Failed: " << failed << "  Skipped: " << skipped << std::endl;
    oss << "Pass Rate: " << (total > 0 ? (passed * 100 / total) : 0) << "% (" << passed << "/" << total << ")" << std::endl;
    oss << "----------------------------------------" << std::endl;

    if (failed > 0) {
        oss << "Failed tests:" << std::endl;
        for (const auto& r : results) {
            if (!r.passed && !r.skipped) {
                oss << "  [FAIL] " << utility::conversions::to_utf8string(r.name) << " - " << utility::conversions::to_utf8string(r.error_msg) << std::endl;
            }
        }
        oss << "----------------------------------------" << std::endl;
    }

    if (skipped > 0) {
        oss << "Skipped tests:" << std::endl;
        for (const auto& r : results) {
            if (r.skipped) {
                oss << "  [SKIP] " << utility::conversions::to_utf8string(r.name) << " - " << utility::conversions::to_utf8string(r.error_msg) << std::endl;
            }
        }
        oss << "----------------------------------------" << std::endl;
    }

    oss << std::endl;
    oss << "--- Quote: Basic/Stock APIs ---" << std::endl;
    oss << "--- Quote: Stock Extended APIs ---" << std::endl;
    oss << "--- Quote: Option APIs ---" << std::endl;
    oss << "--- Quote: Future APIs ---" << std::endl;
    oss << "--- Quote: Fund APIs ---" << std::endl;
    oss << "--- Quote: Financial APIs ---" << std::endl;
    oss << "--- Quote: Industry APIs ---" << std::endl;
    oss << "--- Quote: Other APIs ---" << std::endl;
    oss << "--- Trade: Account Query APIs ---" << std::endl;
    oss << "--- Trade: Order Query APIs ---" << std::endl;
    oss << "--- Trade: Contract Query APIs ---" << std::endl;
    oss << "--- Trade: Fund Management APIs ---" << std::endl;
    oss << "--- Trade: Order Preview API ---" << std::endl;
    oss << "--- Push: WebSocket Streaming ---" << std::endl;
    oss << std::endl;

    oss << "All tests:" << std::endl;
    int idx = 1;
    for (const auto& r : results) {
        std::string status;
        if (r.skipped) status = "[SKIP]";
        else if (r.passed) status = "[PASS]";
        else status = "[FAIL]";
        oss << "  " << idx++ << ". " << status << " " << utility::conversions::to_utf8string(r.name);
        if (!r.passed && !r.error_msg.empty()) {
            oss << " - " << utility::conversions::to_utf8string(r.error_msg);
        }
        oss << std::endl;
    }
    oss << "========================================" << std::endl;

    // Print to console
    std::string report = oss.str();
    std::cout << report;

    // Save to file if specified
    if (!report_file.empty()) {
        std::ofstream file(report_file);
        if (file.is_open()) {
            file << report;
            file.close();
            std::cout << "Report saved to: " << report_file << std::endl;
        } else {
            std::cerr << "Failed to save report to: " << report_file << std::endl;
        }
    }
}

// ===================================================================
// Token auto-refresh unit tests (no network required)
// ===================================================================
class TestTokenRefresh {
public:
    // Verify that start_token_refresh drives the fetch_fn at least once within
    // the specified interval and that the token is updated thread-safely.
    static void test_callback_is_called() {
        ClientConfig cfg(false);
        cfg.token = U("old_token");
        std::atomic<int> call_count{0};

        bool started = cfg.start_token_refresh(1, [&]() -> utility::string_t {
            call_count++;
            return U("new_token");
        });
        if (!started) {
            throw std::runtime_error("start_token_refresh returned false unexpectedly");
        }

        // Wait up to 3s for at least one refresh call.
        for (int i = 0; i < 30 && call_count == 0; ++i) {
            std::this_thread::sleep_for(std::chrono::milliseconds(100));
        }
        cfg.stop_token_refresh();

        if (call_count == 0) {
            throw std::runtime_error("fetch_fn was never called within timeout");
        }
        utility::string_t tok = cfg.get_token();
        if (tok != U("new_token")) {
            throw std::runtime_error("token was not updated after refresh");
        }
        ucout << U("  call_count=") << call_count.load() << U(", token=") << tok << endl;
    }

    // Verify that a second start_token_refresh call is rejected while one is running.
    static void test_double_start_rejected() {
        ClientConfig cfg(false);
        std::atomic<bool> keep_running{true};

        bool first = cfg.start_token_refresh(3600, [&]() -> utility::string_t {
            while (keep_running) {
                std::this_thread::sleep_for(std::chrono::milliseconds(50));
            }
            return U("token");
        });
        bool second = cfg.start_token_refresh(3600, [&]() -> utility::string_t {
            return U("should_not_run");
        });
        keep_running = false;
        cfg.stop_token_refresh();

        if (!first) throw std::runtime_error("first start should succeed");
        if (second) throw std::runtime_error("second start should be rejected");
    }

    // Verify that stop_token_refresh is safe to call multiple times.
    static void test_stop_idempotent() {
        ClientConfig cfg(false);
        cfg.start_token_refresh(3600, [&]() { return U("tok"); });
        cfg.stop_token_refresh();
        cfg.stop_token_refresh(); // must not hang or throw
    }

    // Verify that get_token() returns the current token under lock.
    static void test_get_token_thread_safe() {
        ClientConfig cfg(false);
        cfg.token = U("valueA");
        std::atomic<int> errors{0};
        std::atomic<bool> stop_writer{false};

        // Writer thread alternates between two non-empty values.
        std::thread writer([&cfg, &stop_writer]() {
            int i = 0;
            while (!stop_writer.load()) {
                cfg.set_token(i % 2 == 0 ? U("valueA") : U("valueB"));
                ++i;
                std::this_thread::sleep_for(std::chrono::microseconds(50));
            }
        });

        std::vector<std::thread> readers;
        for (int i = 0; i < 4; ++i) {
            readers.emplace_back([&cfg, &errors]() {
                for (int j = 0; j < 100; ++j) {
                    utility::string_t t = cfg.get_token();
                    if (t.empty()) {
                        errors.fetch_add(1, std::memory_order_relaxed);
                    }
                    std::this_thread::sleep_for(std::chrono::microseconds(100));
                }
            });
        }
        for (auto& t : readers) t.join();
        stop_writer.store(true);
        writer.join();

        if (errors.load() > 0) {
            throw std::runtime_error("get_token returned empty unexpectedly " + std::to_string(errors.load()) + " times");
        }
    }

    // is_directory: real dir → true, file path → false, missing path → false.
    static void test_is_directory() {
        // Use a temp dir we know exists.
        std::string tmp = get_temp_directory();
        if (!Utils::is_directory(Utils::str8to16(tmp))) {
            throw std::runtime_error("expected temp dir to be a directory: " + tmp);
        }
        // A path ending in a filename (even nonexistent) is not a directory.
        std::string non_dir = tmp + "/nonexistent_file.txt";
        if (Utils::is_directory(Utils::str8to16(non_dir))) {
            throw std::runtime_error("expected non-dir path to return false: " + non_dir);
        }
        ucout << U("  is_directory: temp=true, file=false") << endl;
    }

    // load_token reads a token file written in the same format save_token uses.
    static void test_save_and_load_token() {
        std::string dir = get_temp_directory();
        if (dir.back() != '/' && dir.back() != '\\') dir += '/';

        utility::string_t udir = Utils::str8to16(dir);
        utility::string_t token_val = U("test_token_abc123");

        // Write the token file in the same format save_token uses.
        std::string token_file = dir + "tiger_openapi_token.properties";
        {
            Properties props;
            props.set_property(U("token"), token_val);
            std::ofstream f(token_file);
            props.store(f);
        }

        // Load into a fresh ClientConfig pointed at same dir; token should be read.
        ClientConfig cfg(false, udir);
        utility::string_t loaded = cfg.get_token();
        if (loaded != token_val) {
            throw std::runtime_error(
                "loaded token mismatch: expected=" +
                utility::conversions::to_utf8string(token_val) +
                " got=" + utility::conversions::to_utf8string(loaded));
        }

        std::remove(token_file.c_str());
        ucout << U("  token file written and reloaded correctly") << endl;
    }

    // get_props_path: file-style props_path derives token file from same directory.
    static void test_props_path_as_file() {
        std::string dir = get_temp_directory();
        if (dir.back() != '/' && dir.back() != '\\') dir += '/';

        // Point props_path directly at the config file (file-style, not dir-style).
        std::string cfg_file = dir + "tiger_openapi_config.properties";
        {
            std::ofstream f(cfg_file);
            f << "tiger_id=dummy\n";
        }

        utility::string_t ucfg = Utils::str8to16(cfg_file);

        // When props_path is a .properties file, token file should be in the same dir.
        utility::string_t token_val = U("file_style_token_xyz");
        std::string token_file = dir + "tiger_openapi_token.properties";

        // Write token file directly (same format as save_token).
        {
            Properties props;
            props.set_property(U("token"), token_val);
            std::ofstream f(token_file);
            props.store(f);
        }

        ClientConfig cfg2(false, ucfg);
        if (cfg2.get_token() != token_val) {
            throw std::runtime_error("token mismatch for file-style props_path");
        }

        std::remove(cfg_file.c_str());
        std::remove(token_file.c_str());
        ucout << U("  file-style props_path: token loaded correctly from same dir") << endl;
    }
};

int main(int argc, char* argv[]) {
    LoggerConfig::set_log_level(el::Level::Debug);
    // Load config: TIGER_CONFIG_PATH env > argv[1] > ~/.tigeropen/
    utility::string_t config_path;
    const char* config_path_env = std::getenv("TIGER_CONFIG_PATH");
    if (config_path_env && *config_path_env) {
        config_path = Utils::str8to16(std::string(config_path_env));
    } else if (argc >= 2) {
        config_path = Utils::str8to16(std::string(argv[1]));
    } else {
        const char* home_env = std::getenv("HOME");
        const char* userprofile_env = std::getenv("USERPROFILE");
        if (home_env && *home_env) {
            config_path = Utils::str8to16(std::string(home_env)) + U("/.tigeropen/");
        } else if (userprofile_env && *userprofile_env) {
            config_path = Utils::str8to16(std::string(userprofile_env)) + U("/.tigeropen/");
        } else {
            config_path = U("demo/openapi_cpp_test/");
        }
    }
    ClientConfig config(false, config_path);

    // Allow overriding server URL via env var (useful for QA environments).
    const char* server_url_env = std::getenv("TIGER_SERVER_URL");
    if (server_url_env && *server_url_env) {
        config.set_server_url(Utils::str8to16(std::string(server_url_env)));
    }

    // Allow overriding server public key via env var (needed for QA/sandbox servers).
    const char* server_pub_key_env = std::getenv("TIGER_SERVER_PUB_KEY");
    if (server_pub_key_env && *server_pub_key_env) {
        config.set_server_public_key(Utils::str8to16(std::string(server_pub_key_env)));
    }

    config.use_full_tick = true;

    std::vector<TestResult> results;

    // ===================================================================
    // ===== TOKEN AUTO-REFRESH UNIT TESTS (no network) =====
    // ===================================================================
    ucout << U("\n===== Token Auto-Refresh Unit Tests =====\n") << endl;

    results.push_back(run_test(U("token_refresh::callback_is_called"), []() {
        TestTokenRefresh::test_callback_is_called();
    }));

    results.push_back(run_test(U("token_refresh::double_start_rejected"), []() {
        TestTokenRefresh::test_double_start_rejected();
    }));

    results.push_back(run_test(U("token_refresh::stop_idempotent"), []() {
        TestTokenRefresh::test_stop_idempotent();
    }));

    results.push_back(run_test(U("token_refresh::get_token_thread_safe"), []() {
        TestTokenRefresh::test_get_token_thread_safe();
    }));

    results.push_back(run_test(U("token_refresh::is_directory"), []() {
        TestTokenRefresh::test_is_directory();
    }));

    results.push_back(run_test(U("token_refresh::save_and_load_token"), []() {
        TestTokenRefresh::test_save_and_load_token();
    }));

    results.push_back(run_test(U("token_refresh::props_path_as_file"), []() {
        TestTokenRefresh::test_props_path_as_file();
    }));

    // ===================================================================
    // ===== TOKEN REFRESH REAL-NETWORK TESTS (require TIGER_CONFIG_PATH) =====
    // ===================================================================
    const char* cfg_path_env = std::getenv("TIGER_CONFIG_PATH");
    if (cfg_path_env && *cfg_path_env) {
        ucout << U("\n===== Token Refresh Network Tests =====\n") << endl;

        // Test 1: refresh_token() fetches and persists a new token.
        results.push_back(run_test(U("token_refresh::refresh_token (real)"), [&]() {
            TigerClient base_client(config);
            utility::string_t old_tok = config.get_token();
            utility::string_t new_tok = base_client.refresh_token();
            if (new_tok.empty()) {
                throw std::runtime_error("refresh_token() returned empty");
            }
            ucout << U("  new token length=") << new_tok.size() << endl;
            // Verify persisted: reload config from same path and check token matches.
            ClientConfig cfg2(false, Utils::str8to16(std::string(cfg_path_env)));
            if (cfg2.get_token() != new_tok) {
                throw std::runtime_error("persisted token mismatch after refresh_token()");
            }
            ucout << U("  token persisted and reloaded correctly") << endl;
        }));

        // Test 2: start_token_refresh runs one cycle and updates the token.
        results.push_back(run_test(U("token_refresh::auto_refresh_one_cycle (real)"), [&]() {
            TigerClient base_client(config);
            utility::string_t tok_before = base_client.client_config.get_token();

            bool started = base_client.start_token_refresh(2);
            if (!started) {
                throw std::runtime_error("start_token_refresh() returned false");
            }
            // Wait for at least one refresh cycle (2s interval + buffer).
            std::this_thread::sleep_for(std::chrono::seconds(4));
            base_client.stop_token_refresh();

            utility::string_t tok_after = base_client.client_config.get_token();
            if (tok_after.empty()) {
                throw std::runtime_error("token is empty after auto-refresh cycle");
            }
            ucout << U("  token after auto-refresh length=") << tok_after.size() << endl;
            if (tok_after == tok_before) {
                ucout << U("  (token unchanged — server may have issued same value)") << endl;
            } else {
                ucout << U("  token updated by auto-refresh") << endl;
            }
        }));
    }

    // If TOKEN_ONLY env is set, skip the rest of the tests.
    if (std::getenv("TOKEN_ONLY")) {
        ucout << U("\n===== TOKEN_ONLY mode: skipping remaining tests =====\n") << endl;
        int pass = 0, fail = 0, skip = 0;
        for (const auto& r : results) {
            if (r.skipped) ++skip;
            else if (r.passed) ++pass;
            else ++fail;
        }
        ucout << U("\nSummary: ") << pass << U(" passed, ") << fail << U(" failed, ") << skip << U(" skipped") << endl;
        return fail > 0 ? 1 : 0;
    }

    // ===================================================================
    // ===== REAL CONFIG TESTS (require tiger_openapi_config.properties) =====
    // ===================================================================
    ucout << U("\n===== Initializing QuoteClient =====\n") << endl;
    std::shared_ptr<QuoteClient> quote_client;
    try {
        quote_client = std::make_shared<QuoteClient>(config);
    } catch (const std::exception& e) {
        TestResult init_result;
        init_result.name = U("QuoteClient::init (grab_quote_permission)");
        init_result.skipped = false;
        init_result.passed = false;
        init_result.error_msg = utility::conversions::to_string_t(e.what());
        ucout << U("[FAIL] ") << init_result.name << U(" - ") << init_result.error_msg << endl;
        results.push_back(init_result);
    }

    if (!quote_client) {
        // Same ClientConfig (server_url/server_public_key) is used for QuoteClient,
        // TradeClient and PushClient below, so if signature verification or auth
        // failed here, every subsequent real-network test would fail identically.
        // Stop here instead of dereferencing a null client.
        ucout << U("\n===== Aborting remaining real-config tests: QuoteClient failed to initialize =====\n") << endl;
        print_report(results, "test_report.txt");
        return 1;
    }

    // --- 1. Basic/Stock Quote APIs ---
    ucout << U("\n--- Quote: Basic/Stock APIs ---\n") << endl;

    results.push_back(run_test(U("grab_quote_permission"), [&]() {
        TestQuoteClient::test_grab_quote_permission(quote_client);
    }));

    results.push_back(run_test(U("get_symbols"), [&]() {
        TestQuoteClient::test_get_symbols(quote_client);
    }));

    results.push_back(run_test(U("get_all_symbol_names"), [&]() {
        TestQuoteClient::test_get_symbols_names(quote_client);
    }));

    results.push_back(run_test(U("get_market_state"), [&]() {
        TestQuoteClient::test_get_market_state(quote_client);
    }));

    results.push_back(run_test(U("get_trading_calendar"), [&]() {
        TestQuoteClient::test_get_trading_calendar(quote_client);
    }));

    results.push_back(run_test(U("get_brief"), [&]() {
        TestQuoteClient::test_get_brief(quote_client);
    }));

    results.push_back(run_test(U("get_stock_detail"), [&]() {
        TestQuoteClient::test_get_stock_detail(quote_client);
    }));

    results.push_back(run_test(U("get_timeline"), [&]() {
        TestQuoteClient::test_get_timeline(quote_client);
    }));

    results.push_back(run_test(U("get_history_timeline"), [&]() {
        TestQuoteClient::test_get_history_timeline(quote_client);
    }));

    results.push_back(run_test(U("get_quote_real_time"), [&]() {
        TestQuoteClient::test_get_quote_real_time(quote_client);
    }));

    results.push_back(run_test(U("get_quote_delay"), [&]() {
        TestQuoteClient::test_get_quote_delay(quote_client);
    }));

    results.push_back(run_test(U("get_quote_depth"), [&]() {
        TestQuoteClient::test_get_quote_depth(quote_client);
    }));

    results.push_back(skip_test(U("get_quote_shortable_stocks"), U("Server does not support this method for current account")));

    results.push_back(run_test(U("get_kline"), [&]() {
        TestQuoteClient::test_get_kline(quote_client);
    }));

    results.push_back(run_test(U("get_quote_stock_trade"), [&]() {
        TestQuoteClient::test_get_quote_stock_trade(quote_client);
    }));

    results.push_back(run_test(U("get_trade_tick"), [&]() {
        TestQuoteClient::test_get_trade_tick(quote_client);
    }));

    results.push_back(run_test(U("get_kline_quota"), [&]() {
        TestQuoteClient::test_get_kline_quota(quote_client);
    }));

    // --- 2. Stock Extended APIs ---
    ucout << U("\n--- Quote: Stock Extended APIs ---\n") << endl;

    results.push_back(run_test(U("get_stock_broker"), [&]() {
        TestQuoteClient::test_get_stock_broker(quote_client);
    }));

    results.push_back(run_test(U("get_capital_flow"), [&]() {
        TestQuoteClient::test_get_capital_flow(quote_client);
    }));

    results.push_back(run_test(U("get_capital_flow (with time)"), [&]() {
        TestQuoteClient::test_get_capital_flow_with_time(quote_client);
    }));

    results.push_back(run_test(U("get_capital_distribution"), [&]() {
        TestQuoteClient::test_get_capital_distribution(quote_client);
    }));

    results.push_back(run_test(U("get_capital_distribution (with lang)"), [&]() {
        TestQuoteClient::test_get_capital_distribution_with_lang(quote_client);
    }));

    // --- 3. Option Quote APIs ---
    ucout << U("\n--- Quote: Option APIs ---\n") << endl;

    results.push_back(run_test(U("get_option_expiration"), [&]() {
        TestQuoteClient::test_get_option_expiration(quote_client);
    }));

    results.push_back(run_test(U("get_option_chain"), [&]() {
        TestQuoteClient::test_get_option_chain(quote_client);
    }));

    results.push_back(run_test(U("get_option_brief"), [&]() {
        TestQuoteClient::test_get_option_brief(quote_client);
    }));

    results.push_back(run_test(U("get_option_kline"), [&]() {
        TestQuoteClient::test_get_option_kline(quote_client);
    }));

    results.push_back(run_test(U("get_option_trade_tick"), [&]() {
        TestQuoteClient::test_get_option_trade_tick(quote_client);
    }));

    results.push_back(run_test(U("get_option_symbols"), [&]() {
        TestQuoteClient::test_get_option_symbols(quote_client);
    }));

    results.push_back(run_test(U("get_option_depth"), [&]() {
        TestQuoteClient::test_get_option_depth(quote_client);
    }));

    results.push_back(run_test(U("get_option_timeline"), [&]() {
        TestQuoteClient::test_get_option_timeline(quote_client);
    }));

    results.push_back(run_test(U("get_option_analysis"), [&]() {
        TestQuoteClient::test_get_option_analysis(quote_client);
    }));

    results.push_back(run_test(U("get_warrant_real_time_quote"), [&]() {
        TestQuoteClient::test_get_warrant_real_time_quote(quote_client);
    }));

    results.push_back(run_test(U("get_warrant_filter"), [&]() {
        TestQuoteClient::test_get_warrant_filter(quote_client);
    }));

    // --- 4. Future Quote APIs ---
    ucout << U("\n--- Quote: Future APIs ---\n") << endl;

    results.push_back(run_test(U("get_future_exchange"), [&]() {
        TestQuoteClient::test_get_future_exchange(quote_client);
    }));

    results.push_back(run_test(U("get_future_contracts"), [&]() {
        TestQuoteClient::test_get_future_contracts(quote_client);
    }));

    results.push_back(run_test(U("get_future_current_contract"), [&]() {
        TestQuoteClient::test_get_future_current_contract(quote_client);
    }));

    results.push_back(run_test(U("get_future_continuous_contracts"), [&]() {
        TestQuoteClient::test_get_future_continuous_contracts(quote_client);
    }));

    results.push_back(run_test(U("get_future_contract_by_contract_code"), [&]() {
        TestQuoteClient::test_get_future_contract_by_contract_code(quote_client);
    }));

    results.push_back(run_test(U("get_future_trading_date"), [&]() {
        TestQuoteClient::test_get_future_trading_date(quote_client);
    }));

    results.push_back(run_test(U("get_future_kline"), [&]() {
        TestQuoteClient::test_get_future_kline(quote_client);
    }));

    results.push_back(run_test(U("get_future_tick"), [&]() {
        TestQuoteClient::test_get_future_tick(quote_client);
    }));

    results.push_back(run_test(U("get_future_real_time_quote"), [&]() {
        TestQuoteClient::test_get_future_real_time_quote(quote_client);
    }));

    results.push_back(run_test(U("get_future_depth"), [&]() {
        TestQuoteClient::test_get_future_depth(quote_client);
    }));

    results.push_back(skip_test(U("get_future_history_main_contract"), U("API not supported by server (not in standard API docs)")));

    // --- 5. Fund Quote APIs ---
    ucout << U("\n--- Quote: Fund APIs ---\n") << endl;

    results.push_back(run_test(U("get_fund_symbols"), [&]() {
        TestQuoteClient::test_get_fund_symbols(quote_client);
    }));

    results.push_back(run_test(U("get_fund_contracts"), [&]() {
        TestQuoteClient::test_get_fund_contracts(quote_client);
    }));

    results.push_back(run_test(U("get_fund_quote"), [&]() {
        TestQuoteClient::test_get_fund_quote(quote_client);
    }));

    results.push_back(run_test(U("get_fund_history_quote"), [&]() {
        TestQuoteClient::test_get_fund_history_quote(quote_client);
    }));

    // --- 6. Financial APIs ---
    ucout << U("\n--- Quote: Financial APIs ---\n") << endl;

    results.push_back(run_test(U("get_financial_currency"), [&]() {
        TestQuoteClient::test_get_financial_currency(quote_client);
    }));

    results.push_back(run_test(U("get_financial_exchange_rate"), [&]() {
        TestQuoteClient::test_get_financial_exchange_rate(quote_client);
    }));

    // --- 7. Industry APIs ---
    ucout << U("\n--- Quote: Industry APIs ---\n") << endl;

    results.push_back(run_test(U("get_industry_list"), [&]() {
        TestQuoteClient::test_get_industry_list(quote_client);
    }));

    results.push_back(run_test(U("get_industry_stocks"), [&]() {
        TestQuoteClient::test_get_industry_stocks(quote_client);
    }));

    results.push_back(run_test(U("get_stock_industry"), [&]() {
        TestQuoteClient::test_get_stock_industry(quote_client);
    }));

    // --- 8. Other Quote APIs ---
    ucout << U("\n--- Quote: Other APIs ---\n") << endl;

    results.push_back(run_test(U("get_stock_fundamental"), [&]() {
        TestQuoteClient::test_get_stock_fundamental(quote_client);
    }));

    results.push_back(run_test(U("get_trade_rank"), [&]() {
        TestQuoteClient::test_get_trade_rank(quote_client);
    }));

    results.push_back(run_test(U("get_quote_overnight"), [&]() {
        TestQuoteClient::test_get_quote_overnight(quote_client);
    }));

    results.push_back(run_test(U("get_broker_hold"), [&]() {
        TestQuoteClient::test_get_broker_hold(quote_client);
    }));

    results.push_back(run_test(U("get_market_scanner_tags"), [&]() {
        TestQuoteClient::test_get_market_scanner_tags(quote_client);
    }));
    results.push_back(run_test(U("get_corporate_symbol_change"), [&]() {
        TestQuoteClient::test_get_corporate_symbol_change(quote_client);
    }));
    results.push_back(run_test(U("get_corporate_delisting"), [&]() {
        TestQuoteClient::test_get_corporate_delisting(quote_client);
    }));
    results.push_back(run_test(U("get_corporate_ipo"), [&]() {
        TestQuoteClient::test_get_corporate_ipo(quote_client);
    }));

    // ===================================================================
    // ===== TRADE CLIENT TESTS =====
    // ===================================================================
    ucout << U("\n===== Initializing TradeClient =====\n") << endl;
    std::shared_ptr<TradeClient> trade_client;
    try {
        trade_client = std::make_shared<TradeClient>(config);
    } catch (const std::exception& e) {
        TestResult init_result;
        init_result.name = U("TradeClient::init");
        init_result.skipped = false;
        init_result.passed = false;
        init_result.error_msg = utility::conversions::to_string_t(e.what());
        ucout << U("[FAIL] ") << init_result.name << U(" - ") << init_result.error_msg << endl;
        results.push_back(init_result);
        ucout << U("\n===== Aborting remaining real-config tests: TradeClient failed to initialize =====\n") << endl;
        print_report(results, "test_report.txt");
        return 1;
    }

    // --- 9. Account Query APIs ---
    ucout << U("\n--- Trade: Account Query APIs ---\n") << endl;

    results.push_back(run_test(U("get_accounts"), [&]() {
        TestTradeClient::test_get_accounts(trade_client);
    }));

    results.push_back(run_test(U("get_prime_asset"), [&]() {
        TestTradeClient::test_get_prime_asset(trade_client);
    }));

    results.push_back(run_test(U("get_prime_portfolio"), [&]() {
        TestTradeClient::test_get_prime_portfolio(trade_client);
    }));

    results.push_back(run_test(U("get_asset"), [&]() {
        TestTradeClient::test_get_asset(trade_client);
    }));

    results.push_back(run_test(U("get_positions"), [&]() {
        TestTradeClient::test_get_position(trade_client);
    }));

    results.push_back(run_test(U("get_analytics_asset"), [&]() {
        TestTradeClient::test_get_analytics_asset(trade_client);
    }));

    results.push_back(skip_test(U("get_aggregate_assets"), U("Only supported for institution accounts")));

    // --- 10. Order Query APIs ---
    ucout << U("\n--- Trade: Order Query APIs ---\n") << endl;

    results.push_back(run_test(U("get_orders"), [&]() {
        TestTradeClient::test_get_orders(trade_client);
    }));

    results.push_back(run_test(U("get_active_orders"), [&]() {
        TestTradeClient::test_get_active_orders(trade_client);
    }));

    results.push_back(run_test(U("get_filled_orders"), [&]() {
        TestTradeClient::test_get_filled_orders(trade_client);
    }));

    results.push_back(run_test(U("get_inactive_orders"), [&]() {
        TestTradeClient::test_get_inactive_orders(trade_client);
    }));

    results.push_back(run_test(U("get_transactions"), [&]() {
        TestTradeClient::test_get_transactions(trade_client);
    }));

    // --- 11. Contract Query APIs ---
    ucout << U("\n--- Trade: Contract Query APIs ---\n") << endl;

    results.push_back(run_test(U("get_contract"), [&]() {
        TestTradeClient::test_get_contract(trade_client);
    }));

    results.push_back(run_test(U("get_contracts"), [&]() {
        TestTradeClient::test_get_contracts(trade_client);
    }));

    results.push_back(run_test(U("get_estimate_tradable_quantity"), [&]() {
        TestTradeClient::test_get_estimate_tradable_quantity(trade_client);
    }));

    results.push_back(run_test(U("price_util"), [&]() {
        TestTradeClient::test_price_util(trade_client);
    }));

    // --- 12. Fund Management APIs ---
    ucout << U("\n--- Trade: Fund Management APIs ---\n") << endl;

    results.push_back(run_test(U("get_segment_fund_history"), [&]() {
        TestTradeClient::test_get_segment_fund_history(trade_client);
    }));

    results.push_back(run_test(U("get_segment_fund_available"), [&]() {
        TestTradeClient::test_get_segment_fund_available(trade_client);
    }));

    results.push_back(run_test(U("get_funding_history"), [&]() {
        TestTradeClient::test_get_funding_history(trade_client);
    }));

    results.push_back(run_test(U("get_fund_details"), [&]() {
        TestTradeClient::test_get_fund_details(trade_client);
    }));

    results.push_back(run_test(U("get_position_transfer_records"), [&]() {
        TestTradeClient::test_get_position_transfer_records(trade_client);
    }));

    // --- 13. Order Preview API ---
    ucout << U("\n--- Trade: Order Preview API ---\n") << endl;

    results.push_back(run_test(U("preview_order"), [&]() {
        TestTradeClient::test_preview_order(trade_client);
    }));

    ucout << U("\n--- Trade: Option Exercise API ---\n") << endl;

    results.push_back(run_test(U("option_exercise_positions"), [&]() {
        TestTradeClient::test_option_exercise(trade_client);
    }));

    // ===================================================================
    // ===== PUSH CLIENT TESTS =====
    // ===================================================================
    ucout << U("\n===== Initializing PushClient =====\n") << endl;

    results.push_back(run_test(U("push_client (connect/subscribe/receive 10s)"), [&]() {
        std::shared_ptr<IPushClient> push_client = IPushClient::create_push_client(config);
        TestPushClient::test_push_client_timed(push_client, config, 10);
    }));

    // ===================================================================
    // Print final report and save to file
    // ===================================================================
    print_report(results, "test_report.txt");

    return 0;
}