

#ifndef _UTIL_CPP_
#define _UTIL_CPP_

#include "utils.h"
#include "algo_hmac.h"
#include "base64.hpp"
#include <time.h>
#include <cpprest/http_client.h>
#include <cpprest/json.h>

using namespace std;
using namespace web;
using namespace websocketpp;

char * get_timestamp(char *timestamp, int len) {
//    time_t t;
//    time(&t);
//    struct tm tm;
//    gmtime_s(&tm, &t);

    struct tm tm;
    time_t t = time(NULL);
    gmtime_r(&t, &tm);

    strftime(timestamp,len,"%FT%T.123Z", &tm);
    return timestamp;
}


string get_sign(string key, string timestamp, string method, string req_path, string body) {
    string sign;
    unsigned char * mac = NULL;
    unsigned int mac_length = 0;
    string data = timestamp + method + req_path + body;
    int ret = hmac_encode("sha256", key.c_str(), key.length(), data.c_str(), data.length(), mac, mac_length);
    sign = base64_encode(mac, mac_length);
    return sign;
}

string build_params(string req_path, map<string,string> m) {
    string str = req_path;
    bool first = true;
    for(auto i=m.begin();i!=m.end();i++) {
        if (first) {
            str += "?";
            first = false;
        } else {
            str += "&";
        }
        str += i->first;
        str += "=";
        str += i->second;
    }
    return str;
}

string get_level_str(int level) {
    string level_str;
    for(int levelI = 0;levelI<level ; levelI++){
        level_str.append("\t");
    }
    return level_str;
}


string json_format(string json_str) {
    int level = 0;
    string json_format_str;
    for(int i=0; i < json_str.length(); i++){
        char c = json_str.at(i);
        if(level>0&& '\n' == json_format_str.at(json_format_str.length() - 1)){
            json_format_str += get_level_str(level);
        }
        switch (c) {
            case '{':
            case '[':
                json_format_str += c;
                json_format_str += '\n';
                level++;
                break;
            case ',':
                json_format_str += c;
                json_format_str += '\n';
                break;
            case '}':
            case ']':
                json_format_str += '\n';
                level--;
                json_format_str += get_level_str(level);
                json_format_str += c;
                break;
            default:
                json_format_str += c;
                break;
        }
    }

    return json_format_str;

}


// Demonstrates how to iterate over a JSON object.
void iterate_json_value()
{
    // Create a JSON object.
    json::value obj;
//    obj[L"key1"] = json::value::boolean(false);
    obj[U("key1")] = json::value::boolean(false);
    obj[U("key2")] = json::value::number(44);
    obj[U("key3")] = json::value::number(43.6);
    obj[U("key4")] = json::value::string(U("str"));


    // Loop over each element in the object.
    for (auto iter = obj.as_object().cbegin(); iter != obj.as_object().cend(); ++iter)
    {
        // Make sure to get the value as const reference otherwise you will end up copying
        // the whole JSON value recursively which can be expensive if it is a nested object.

        //const json::value &str = iter->first;
        //const json::value &v = iter->second;

        const auto &str = iter->first;
        const auto &v = iter->second;

        // Perform actions here to process each string and value in the JSON object...
//        std::cout << "String: " << str.c_str() << ", Value: " << w2s(v.serialize()) << endl;
    }

    /* Output:
    String: key1, Value: false
    String: key2, Value: 44
    String: key3, Value: 43.6
    String: key4, Value: str
    */
}

int gz_decompress(Byte *zdata, uLong nzdata, Byte *data, uLong *ndata)
{
    int err = 0;
    z_stream d_stream = {0}; /* decompression stream */

    static char dummy_head[2] = {
            0x8 + 0x7 * 0x10,
            (((0x8 + 0x7 * 0x10) * 0x100 + 30) / 31 * 31) & 0xFF,
    };

    d_stream.zalloc = NULL;
    d_stream.zfree = NULL;
    d_stream.opaque = NULL;
    d_stream.next_in = zdata;
    d_stream.avail_in = 0;
    d_stream.next_out = data;


    if (inflateInit2(&d_stream, -MAX_WBITS) != Z_OK) {
        return -1;
    }

    // if(inflateInit2(&d_stream, 47) != Z_OK) return -1;

    while (d_stream.total_out < *ndata && d_stream.total_in < nzdata) {
        d_stream.avail_in = d_stream.avail_out = 1; /* force small buffers */
        if((err = inflate(&d_stream, Z_NO_FLUSH)) == Z_STREAM_END)
            break;

        if (err != Z_OK) {
            if (err == Z_DATA_ERROR) {
                d_stream.next_in = (Bytef*) dummy_head;
                d_stream.avail_in = sizeof(dummy_head);
                if((err = inflate(&d_stream, Z_NO_FLUSH)) != Z_OK) {
                    return -1;
                }
            } else {
                return -1;
            }
        }
    }

    if (inflateEnd(&d_stream)!= Z_OK)
        return -1;
    *ndata = d_stream.total_out;
    return 0;
}


unsigned int str_hex(unsigned char *str,unsigned char *hex)
{
    unsigned char ctmp, ctmp1,half;
    unsigned int num=0;
    do{
        do{
            half = 0;
            ctmp = *str;
            if(!ctmp) break;
            str++;
        }while((ctmp == 0x20)||(ctmp == 0x2c)||(ctmp == '\t'));
        if(!ctmp) break;
        if(ctmp>='a') ctmp = ctmp -'a' + 10;
        else if(ctmp>='A') ctmp = ctmp -'A'+ 10;
        else ctmp=ctmp-'0';
        ctmp=ctmp<<4;
        half = 1;
        ctmp1 = *str;
        if(!ctmp1) break;
        str++;
        if((ctmp1 == 0x20)||(ctmp1 == 0x2c)||(ctmp1 == '\t'))
        {
            ctmp = ctmp>>4;
            ctmp1 = 0;
        }
        else if(ctmp1>='a') ctmp1 = ctmp1 - 'a' + 10;
        else if(ctmp1>='A') ctmp1 = ctmp1 - 'A' + 10;
        else ctmp1 = ctmp1 - '0';
        ctmp += ctmp1;
        *hex = ctmp;
        hex++;
        num++;
    }while(1);
    if(half)
    {
        ctmp = ctmp>>4;
        *hex = ctmp;
        num++;
    }
    return(num);
}

void hex_str(unsigned char *inchar, unsigned int len, unsigned char *outtxt)
{
    unsigned char hbit,lbit;
    unsigned int i;
    for(i=0;i<len;i++)
    {
        hbit = (*(inchar+i)&0xf0)>>4;
        lbit = *(inchar+i)&0x0f;
        if (hbit>9) outtxt[2*i]='A'+hbit-10;
        else outtxt[2*i]='0'+hbit;
        if (lbit>9) outtxt[2*i+1]='A'+lbit-10;
        else    outtxt[2*i+1]='0'+lbit;
    }
    outtxt[2*i] = 0;
}

#endif