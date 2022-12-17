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
}

string TIGERAPI::get_sign(string timestamp, string method, string request_path, string body) {
    string sign;
    unsigned char * mac = NULL;
    unsigned int mac_length = 0;
    string data = timestamp + method + request_path + body;
    string key = client_config.private_key;
    int ret = hmac_encode("sha256", key.c_str(), key.length(), data.c_str(), data.length(), mac, mac_length);
    sign = base64_encode(mac, mac_length);
    return sign;
}

string TIGERAPI::send_request(const string &method, const string &request_path, const string &params) {
    /************************** set request method ***************************/
    http_request request;
    /************************** set request uri ***************************/
    uri_builder builder;
    builder.append_path(request_path);

    request.set_method(method);
    request.set_request_uri(builder.to_uri());
    request._set_base_uri("https://openapi.tigerfintech.com");

    /************************** set request headers ***************************/
    char * timestamp = new char[32];
    timestamp = get_timestamp(timestamp, 32);
    string sign = get_sign(timestamp, method, builder.to_string(), params);
    request.headers().clear();
    request.headers().add(U("Accept"), U("application/json"));
    request.headers().set_content_type(U("application/json; charset=UTF-8"));

    /************************** set request body ***************************/
    request.set_body(params,"application/json; charset=UTF-8");

    cout << "request:\n" << request.to_string().c_str() << endl;
    if (!params.empty()) {
        cout << "body:\n" << json_format(params) << endl;
    }

    /************************** get response ***************************/
    http_response response;
    string str;
    try {
        http_client client("");
        // Wait for headers
        response = client.request(request).get();

        // Wait for data
        response.content_ready().wait();
    }
    catch (std::exception& ex) {
        cout << "Exception: " << ex.what() << endl;
        exit(0);
    }
    try
    {
        str = response.extract_string(true).get();  // get() is needed!
    }
    catch (const std::exception& e)
    {
        cout << e.what() << endl;
    }

    cout << "response:\n" << str << endl;
    // json format
    cout << "body:\n" << json_format(str) << endl;
    cout << endl << endl;

    /************************** print response ***************************/
    auto fp = fopen("result.txt", "a");
    fputs("request:\n", fp);
    fputs(method.c_str(), fp);
    fputs(request_path.c_str(), fp);
    fputs("\nresponse:\n", fp);
    fputs(str.c_str(), fp);
    fputs("\n\n", fp);
    fclose(fp);


    delete []timestamp;
    return str;
}