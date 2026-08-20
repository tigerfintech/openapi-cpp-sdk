#include "tigerapi/tiger_client.h"
#include "tigerapi/version.h"
#include "tigerapi/contract_util.h"
#include "tigerapi/logger.h"
#include <map>
#include <memory>

namespace TIGER_API {

    TigerClient::TigerClient(const ClientConfig &cf)
        : client_config(cf), http_client_(std::make_unique<http_client>(cf.get_server_url())) {
        client_config.check();
    }

    TigerClient::~TigerClient() {
        stop_token_refresh();  // Stop refresh thread before any member is destroyed
    }

    utility::string_t TigerClient::build_sign_content(const value &obj) {
        json::object obj_obj = obj.as_object();

        // Create a vector to hold the keys.
        std::vector<utility::string_t > keys;
        // Iterate over the key-value pairs and add the keys to the vector.
        for (const auto &pair: obj_obj) {
            keys.push_back(pair.first);
        }
        // Sort the keys in ascending order using the std::sort function.
        std::sort(keys.begin(), keys.end());

        utility::string_t  str;
        // Iterate through the sorted list of keys and concat string.
        for (const auto &key: keys) {
            if (!str.empty()) {
                str += U("&");
            }
            const auto &val = obj_obj[key];
            str += key + U("=") + (val.is_string() ? val.as_string() : val.serialize());
        }
        return str;
    }

    // Endpoints pinned to a specific gateway version. Matches the Java SDK request classes:
    //   QuoteTimelineRequest                                      -> V3_0
    //   QuoteKlineRequest / QuoteDepthRequest / QuoteTradeTickRequest -> V2_0
    //   FinancialReportRequest / FinancialDailyRequest                -> V2_0
    //   QuoteTradeCalendarRequest                                     -> V2_0
    // Rust SDK pins the same set via call_into_versioned(..., VERSION_V2).
    // Without the override the gateway answers 'biz param error(failed to parse
    // parameters in biz_content)' (financial_report / financial_daily) or returns
    // an empty market field (trading_calendar).
    //
    // quote_overnight is independently pinned to 1.0 (not the V2_0 group above):
    // the gateway rejects V3.0 for this endpoint and 2.0 is also rejected; 1.0 is
    // the only accepted version (confirmed via live testing; see CHANGELOG 1.3.1
    // "get_quote_overnight now uses its required API version 1.0").
    static const std::map<utility::string_t, utility::string_t>&
    endpoint_version_overrides() {
        static const std::map<utility::string_t, utility::string_t> table = {
            {U("quote_overnight"), U("1.0")},
            {U("timeline"), U("3.0")},
            {U("kline"), U("2.0")},
            {U("quote_depth"), U("2.0")},
            {U("trade_tick"), U("2.0")},
            {U("financial_report"), U("2.0")},
            {U("financial_daily"), U("2.0")},
            {U("trading_calendar"), U("2.0")},
        };
        return table;
    }

    value TigerClient::build_common_params(const utility::string_t &api_method) {
        value common_params;
        common_params[P_TIGER_ID] = value::string(client_config.tiger_id);
        common_params[P_CHARSET] = value::string(client_config.charset);
        // Use per-endpoint version override if present; otherwise the global default.
        utility::string_t version = OPEN_API_SERVICE_VERSION;
        if (!api_method.empty()) {
            const auto &table = endpoint_version_overrides();
            auto it = table.find(api_method);
            if (it != table.end()) {
                version = it->second;
            }
        }
        common_params[P_VERSION] = value::string(version);
        common_params[P_SIGN_TYPE] = value::string(client_config.sign_type);
        common_params[P_DEVICE_ID] = value::string(client_config.device_id);
        return common_params;
    }

    value TigerClient::post(const utility::string_t &api_method, value &params) {
        return send_request(POST, api_method, params);
    }

    value TigerClient::get(const utility::string_t &api_method, value &params) {
        return send_request(GET, api_method, params);
    }

    value TigerClient::send_request(const utility::string_t &http_method, const utility::string_t &api_method, value &body) {
        http_request request;
        request.set_method(http_method);

        /************************** set request headers ***************************/
        request.headers().clear();
        request.headers().add(U("Accept"), U("application/json"));
        request.headers().set_content_type(U("application/json; charset=UTF-8"));
        request.headers().add(P_USER_AGENT, P_SDK_VERSION_PREFIX + U(PROJECT_VERSION));
        {
            utility::string_t tok = client_config.get_token();
            if (!tok.empty()) {
                request.headers().add(U("Authorization"), tok);
            }
        }

        /************************** set request body ***************************/

        value params = value::object(true);
        // Pass api_method so build_common_params can apply per-endpoint version overrides
        // (e.g. financial_report / trading_calendar require V2.0, not the global V3.0).
        value common_params = build_common_params(api_method);
        for (const auto &kvp: common_params.as_object()) {
            params[kvp.first] = kvp.second;
        }
        // `body` may be a top-level array (v1-style batch endpoints like
        // option_brief, option_kline, option_trade_tick — which serialize
        // biz_content as `[{...}, ...]`). cpprestsdk throws "not an object"
        // when you call `array[stringKey]`, so only attach `lang` for object
        // bodies. v1 batch endpoints have no request-level `lang` field
        // anyway (Java `BatchApiModel` serializes `items` directly).
        if (!client_config.lang.empty() && body.is_object()) {
            body[P_LANG] = value::string(client_config.lang);
        }
        if (!body.is_null() && body.size() > 0) {
            params[P_BIZ_CONTENT] = value::string(body.serialize());
        }
        params[P_METHOD] = value::string(api_method);
        params[P_TIMESTAMP] = value::string(Utils::get_timestamp());

        utility::string_t sign_content = build_sign_content(params);
        utility::string_t sign = Utils::get_sign(client_config.private_key, sign_content);
        params[P_SIGN] = value::string(sign);
//        request.set_body(params.serialize(), U("application/json; charset=UTF-8"));
        request.set_body(params);
        /************************** get response ***************************/
        http_response response;
        value result;
        value result_data;
        utility::string_t result_str;
        try {
            LOG(DEBUG) << U("request:\n") << U("Server: ") << http_client_->base_uri().to_string() << U("\n") << request.to_string();
            if (!params.is_null()) {
                utility::string_t req_body = Utils::json_format(params.serialize());
                LOG(DEBUG) << U("body:\n") << Utils::truncate_for_log(req_body);
            }
            // Wait for headers
            response = http_client_->request(request).get();

            // Wait for data
            response.content_ready().wait();

        }
        catch (const std::exception &ex) {
            LOG(ERROR) << U("Exception: ") << ex.what() << endl;
            throw;
        }
        try {
            result = response.extract_json().get();
            result_str = result.serialize();
            if (result[P_CODE].is_null()) {
                LOG(ERROR) << U("Exception: api error, response: ") << result.serialize();
                throw std::runtime_error(Utils::str16to8(result.serialize()).c_str());
            }
            int code = result[P_CODE].as_integer();
            if (code != 0) {
                LOG(ERROR) << U("Exception: api code error, response: ") << result.serialize();
                throw std::runtime_error(Utils::str16to8(result.serialize()).c_str());
            }
            if (!result.has_field(P_SIGN) || !result[P_SIGN].is_string()) {
                LOG(ERROR) << U("Exception: response missing sign field, response: ") << result_str;
                throw std::runtime_error("Exception: response missing sign field");
            }
            utility::string_t res_sign = result[P_SIGN].as_string();
            bool is_sign_ok = Utils::verify_sign(client_config.get_server_pub_key(), params[P_TIMESTAMP].as_string(), res_sign);
            if (!is_sign_ok) {
                LOG(ERROR) << U("Exception: response sign verify failed. ");
                throw std::runtime_error("Exception: response sign verify failed");
            }
            if (!result.has_field(P_DATA)) {
                LOG(ERROR) << U("Exception: response missing data field, response: ") << result_str;
                throw std::runtime_error("Exception: response missing data field");
            }
            // Allow data:null on code==0 (valid empty success response).
            result_data = result[P_DATA];
        }
        catch (const std::exception &e) {
            LOG(ERROR) << U("get response error :") << e.what() << endl;
            throw;
        }
        // Truncate large responses in DEBUG logs to prevent CI log overflow
        // (e.g. get_all_symbols returns thousands of symbols, easily > 4 MB).
        // Reuse result_str (already serialized above) instead of calling
        // result.serialize() again — serialization of large responses is not
        // free, and doing it twice doubles the peak memory/CPU cost for this
        // log statement alone.
        LOG(DEBUG) << U("response:\n") << Utils::truncate_for_log(result_str);
        // Pretty-printed body — also truncated for the same reason.
        utility::string_t formatted = Utils::json_format(result_str);
        LOG(DEBUG) << U("body:\n") << Utils::truncate_for_log(formatted);

        return result_data;
    }

    value TigerClient::identifiers_to_options(value identifiers) {
        value options = value::array();
        size_t j = 0;
        for (size_t i = 0; i < identifiers.size(); ++i) {
            auto identifier = identifiers[i];
            if (!identifier.is_string()) { continue; }
            utility::string_t symbol, expiry, right, strike;
            std::tie(symbol, expiry, right, strike) = ContractUtil::extract_option_info(identifier.as_string());
            if (symbol.empty() || expiry.empty() || right.empty()) {
                LOG(WARNING) << U("identifiers_to_options: skipping unparseable identifier '")
                             << identifier.as_string() << U("' (ContractUtil::extract_option_info returned empty fields)") << endl;
                continue;
            }
            value obj = value::object(true);
            obj[P_EXPIRY] = value::number(static_cast<int64_t>(Utils::date_string_to_timestamp(expiry)));
            obj[P_RIGHT] = value::string(right);
            obj[P_STRIKE] = value::string(strike);
            obj[P_SYMBOL] = value::string(symbol);
            options[j++] = obj;
        }
        return options;
    }

    utility::string_t TigerClient::refresh_token()
    {
        utility::string_t new_token = query_token();
        if (!new_token.empty()) {
            client_config.save_token(new_token);
        }
        return new_token;
    }

    utility::string_t TigerClient::query_token()
    {
        try {
            value params = value::object();
            value result = post(U("user_token_refresh"), params);
            if (!result.is_null() && result.is_object()) {
                if (!result.has_field(U("token"))) {
                    LOG(WARNING) << U("query_token: response missing token field") << endl;
                    return U("");
                }
                return result[U("token")].as_string();
            }
        } catch (const std::exception& e) {
            LOG(ERROR) << U("query_token failed: ") << Utils::str8to16(e.what()) << endl;
        } catch (...) {
            LOG(ERROR) << U("query_token failed: network or platform exception") << endl;
        }
        return U("");
    }

    bool TigerClient::start_token_refresh(int interval_seconds)
    {
        return client_config.start_token_refresh(interval_seconds, [this]() {
            return query_token();
        });
    }

    void TigerClient::stop_token_refresh()
    {
        client_config.stop_token_refresh();
    }
}
