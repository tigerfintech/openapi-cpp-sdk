#include "tigerapi.h"
#include <ctime>
#include "algo_hmac.h"
#include "base64.hpp"
#include "utils.h"
#include "constants.h"

using namespace websocketpp;

void TIGERAPI::set_config(struct Config &cf) {
    client_config.account = cf.account;
    client_config.tiger_id = cf.tiger_id;
    client_config.private_key = cf.private_key;
    client_config.server_url = cf.server_url;
}

string TIGERAPI::build_sign_content(const value &obj) {
    json::object obj_obj = obj.as_object();

    // Create a vector to hold the keys.
    std::vector<std::string> keys;
    // Iterate over the key-value pairs and add the keys to the vector.
    for (const auto &pair: obj_obj) {
        keys.push_back(pair.first);
    }
    // Sort the keys in ascending order using the std::sort function.
    std::sort(keys.begin(), keys.end());

    std::string str;
    // Iterate through the sorted list of keys and concat string.
    for (const auto &key: keys) {
        if (!str.empty()) {
            str += "&";
        }
        str += key + "=" + obj_obj[key].as_string();
    }
    return str;
}

value TIGERAPI::build_common_params() {
    value common_params;
    common_params[P_TIGER_ID] = value::string(client_config.tiger_id);
    common_params[P_CHARSET] = value::string(client_config.charset);
    common_params[P_VERSION] = value::string(OPEN_API_SERVICE_VERSION);
    common_params[P_SIGN_TYPE] = value::string(client_config.sign_type);
    common_params[P_DEVICE_ID] = value::string(get_device_id());
    return common_params;
}

string TIGERAPI::post(const string &api_method, value &params) {
    return send_request(POST, api_method, params);
}

string TIGERAPI::get(const string &api_method, value &params) {
    return send_request(GET, api_method, params);
}

string TIGERAPI::send_request(const string &http_method, const string &api_method, value &body) {
    http_request request;
    request.set_method(http_method);

    /************************** set request headers ***************************/
    request.headers().clear();
    request.headers().add(U("Accept"), U("application/json"));
    request.headers().set_content_type(U("application/json; charset=UTF-8"));

    /************************** set request body ***************************/
    char *timestamp = new char[32];
    timestamp = get_timestamp(timestamp, 32);
    value params;
    value common_params = build_common_params();
    for (const auto &kvp: common_params.as_object()) {
        params[kvp.first] = kvp.second;
    }
    if (!body.is_null() && body.size() > 0) {
        params[P_BIZ_CONTENT] = value::string(body.serialize());
    }
    params[P_METHOD] = value::string(api_method);
    params[P_TIMESTAMP] = value::string(timestamp);

    string sign_content = build_sign_content(params);
    string sign = get_sign(client_config.private_key, sign_content);
    params[P_SIGN] = value::string(sign);
    request.set_body(params.serialize(), "application/json; charset=UTF-8");

    /************************** get response ***************************/
    http_response response;
    string str;
    try {
        http_client client(client_config.server_url);

        cout << "request:\n" << "Server: " << client.base_uri().to_string() << "\n" << request.to_string().c_str()
             << endl;
        if (!params.is_null()) {
            cout << "body:\n" << json_format(params.serialize()) << endl;
        }


        // Wait for headers
        response = client.request(request).get();

        // Wait for data
        response.content_ready().wait();
    }
    catch (std::exception &ex) {
        cout << "Exception: " << ex.what() << endl;
        exit(0);
    }
    try {
        str = response.extract_string(true).get();  // get() is needed!
    }
    catch (const std::exception &e) {
        cout << e.what() << endl;
    }

    cout << "response:\n" << str << endl;
    // json format
    cout << "body:\n" << json_format(str) << endl;
    cout << endl << endl;

    /************************** print response ***************************/
    auto fp = fopen("result.txt", "a");
    fputs("request:\n", fp);
    fputs(http_method.c_str(), fp);
    fputs(params.serialize().c_str(), fp);
    fputs("\nresponse:\n", fp);
    fputs(str.c_str(), fp);
    fputs("\n\n", fp);
    fclose(fp);


    delete[]timestamp;
    return str;
}