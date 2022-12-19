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

string TIGERAPI::get_sign(string timestamp, string method, string body) {
    string sign;
    unsigned char * mac = NULL;
    unsigned int mac_length = 0;
    string data = timestamp + method + body;
    string key = client_config.private_key;
    int ret = hmac_encode("sha256", key.c_str(), key.length(), data.c_str(), data.length(), mac, mac_length);
    sign = base64_encode(mac, mac_length);
    return sign;
}

string TIGERAPI::send_request(const string &method, value &params) {
    /************************** set request method ***************************/
    http_request request;
    /************************** set request uri ***************************/
//    uri_builder builder;
//    builder.append_path("");

    request.set_method(method);
//    request.set_request_uri(builder.to_uri());
//    request._set_base_uri(client_config.server_url);

    /************************** set request headers ***************************/
    char * timestamp = new char[32];
    timestamp = get_timestamp(timestamp, 32);
    string sign = get_sign(timestamp, method, params.serialize());
    params[U("sign")] = value::string(sign);
    params[U("charset")] = value::string("UTF-8");
    request.headers().clear();
    request.headers().add(U("Accept"), U("application/json"));
    request.headers().set_content_type(U("application/json; charset=UTF-8"));

    /************************** set request body ***************************/
    request.set_body(params.serialize(),"application/json; charset=UTF-8");

    /************************** get response ***************************/
    http_response response;
    string str;
    try {
        http_client client(client_config.server_url);

        cout << "request:\n" << "Server: " << client.base_uri().to_string() << "\n" << request.to_string().c_str() << endl;
        if (!params.is_null()) {
            cout << "body:\n" << json_format(params.serialize()) << endl;
        }


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
    fputs(params.serialize().c_str(), fp);
    fputs("\nresponse:\n", fp);
    fputs(str.c_str(), fp);
    fputs("\n\n", fp);
    fclose(fp);


    delete []timestamp;
    return str;
}