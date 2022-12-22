

#ifndef CPPSDK_UTILS_H
#define CPPSDK_UTILS_H

#include <ctime>
#include <string>
#include <map>
#include <zlib.h>

std::string get_timestamp();
std::string get_device_id();
std::string build_params(std::string req_path, std::map<std::string,std::string> m);
std::string json_format(std::string json_str);
int gz_decompress(Byte *zdata, uLong nzdata, Byte *data, uLong *ndata);
std::string get_sign(std::string key, std::string content);
std::string get_sign(unsigned char * private_key, unsigned char * content);

unsigned int str_hex(unsigned char *str,unsigned char *hex);
void hex_str(unsigned char *inchar, unsigned int len, unsigned char *outtxt);

#include <locale> 
#include <codecvt>

inline std::string w2s(std::wstring const& src)
{
    std::wstring_convert<std::codecvt_utf8<wchar_t> > converter;
    return converter.to_bytes(src);
}

inline std::wstring s2w(std::string const& src)
{
    std::wstring_convert<std::codecvt_utf8<wchar_t> > converter;
    return converter.from_bytes(src);
}

#endif //CPPSDK_UTILS_H
