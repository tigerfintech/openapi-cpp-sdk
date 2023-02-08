#include "../include/tigerapi/tiger_client.h"
#include "../include/tigerapi/version.h"
#include "../include/tigerapi/contract_util.h"
#include "../include/tigerapi/easylogging++.h"

namespace TIGER_API {

    void TigerClient::set_config(const ClientConfig &cf) {
        client_config = cf;
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
            str += key + U("=") + obj_obj[key].as_string();
        }
        return str;
    }

    value TigerClient::build_common_params() {
        value common_params;
        common_params[P_TIGER_ID] = value::string(client_config.tiger_id);
        common_params[P_CHARSET] = value::string(client_config.charset);
        common_params[P_VERSION] = value::string(OPEN_API_SERVICE_VERSION);
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

        /************************** set request body ***************************/

        value params = value::object(true);
        value common_params = build_common_params();
        for (const auto &kvp: common_params.as_object()) {
            params[kvp.first] = kvp.second;
        }
        if (!client_config.lang.empty()) {
            body[P_LANG] = value::string(client_config.lang);
        }
        if (!body.is_null() && body.size() > 0) {
            params[P_BIZ_CONTENT] = value::string(body.serialize());
        }
        params[P_METHOD] = value::string(api_method);
        params[P_TIMESTAMP] = value::string(get_timestamp());

        utility::string_t sign_content = build_sign_content(params);
        utility::string_t sign = get_sign(client_config.private_key, sign_content);
        params[P_SIGN] = value::string(sign);
//        request.set_body(params.serialize(), U("application/json; charset=UTF-8"));
        request.set_body(params);
        /************************** get response ***************************/
        http_response response;
        value result;
        value result_data;
        utility::string_t result_str;
        try {
            http_client client(client_config.get_server_url());
            LOG(DEBUG) << U("request:\n") << U("Server: ") << client.base_uri().to_string() << U("\n") << request.to_string();
            if (!params.is_null()) {
               LOG(DEBUG)  << U("body:\n") << json_format(params.serialize());
            }
            // Wait for headers
            response = client.request(request).get();

            // Wait for data
            response.content_ready().wait();

        }
        catch (std::exception &ex) {
            LOG(ERROR) << U("Exception: ") << ex.what() << endl;
            exit(0);
        }
        try {
            result = response.extract_json().get();
            result_str = result.serialize();
            if (result[P_CODE].is_null()) {
                LOG(ERROR) << U("Exception: api error, response: ") << result.serialize();
                exit(-1);
            }
            int code = result[P_CODE].as_integer();
            if (code != 0) {
                LOG(ERROR) << U("Exception: api code error, response: ") << result.serialize();
                exit(code);
            }
            utility::string_t res_sign = result[P_SIGN].as_string();
            bool is_sign_ok = verify_sign(client_config.get_server_pub_key(), params[P_TIMESTAMP].as_string(), res_sign);
            if (!is_sign_ok) {
                LOG(ERROR) << U("Exception: response sign verify failed. ");
                exit(-1);
            }
            result_data = result[P_DATA];
        }
        catch (const std::exception &e) {
            LOG(ERROR) << U("get response error :") << e.what() << endl;
        }
        LOG(DEBUG) << U("response:\n") << result.serialize();
        // json format
        LOG(DEBUG) << U("body:\n") << json_format(result_str);

        return result_data;
    }

    value TigerClient::identifiers_to_options(value identifiers) {
        value options = value::array();
        for (size_t i = 0; i < identifiers.size(); ++i) {
            auto identifier = identifiers[i];
            utility::string_t  symbol, expiry, right, strike;
            std::tie(symbol, expiry, right, strike) = ContractUtil::extract_option_info(identifier.as_string());
            if (symbol.empty() || expiry.empty() || right.empty()) {
                continue;
            }
            value obj = value::object(true);
            obj[P_EXPIRY] = date_string_to_timestamp(expiry);
            obj[P_RIGHT] = value::string(right);
            obj[P_STRIKE] = value::string(strike);
            obj[P_SYMBOL] = value::string(symbol);
            options[i] = obj;
        }
        return options;
    }
}