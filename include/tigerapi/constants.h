
#ifndef CPPSDK_CONSTANTS_H
#define CPPSDK_CONSTANTS_H
#include "cpprest/details/basic_types.h"

static utility::string_t GET = U("GET");
static utility::string_t POST = U("POST");

static utility::string_t P_TIGER_ID = U("tiger_id");
static utility::string_t P_METHOD = U("method");
static utility::string_t P_CHARSET = U("charset");
static utility::string_t P_SIGN_TYPE = U("sign_type");
static utility::string_t P_SIGN = U("sign");
static utility::string_t P_LANG = U("lang");
static utility::string_t P_TIMESTAMP = U("timestamp");
static utility::string_t P_VERSION = U("version");
static utility::string_t P_ITEMS = U("items");
static utility::string_t P_DATA = U("data");
static utility::string_t P_CODE = U("code");
static utility::string_t P_NOTIFY_URL = U("notify_url");
static utility::string_t P_DEVICE_ID = U("device_id");
//static utility::string_t P_SDK_VERSION = U("sdk-version");
static utility::string_t P_SDK_VERSION_PREFIX = U("openapi-cpp-sdk-");
static utility::string_t P_USER_AGENT = U("User-Agent");
static utility::string_t P_BIZ_CONTENT = U("biz_content");
static utility::string_t P_ACCOUNT = U("account");
static utility::string_t P_SECRET_KEY =  U("secret_key");
static utility::string_t P_MARKET = U("market");
static utility::string_t P_SYMBOLS = U("symbols");
static utility::string_t P_SYMBOL = U("symbol");
static utility::string_t P_INCLUDE_OTC = U("include_otc");
static utility::string_t P_CONTRACT_CODES = U("contract_codes");
static utility::string_t P_CONTRACT_CODE = U("contract_code");
static utility::string_t P_PERIOD = U("period");
static utility::string_t P_BEGIN_TIME = U("begin_time");
static utility::string_t P_START_TIME = U("start_time");
static utility::string_t P_END_TIME = U("end_time");
static utility::string_t P_START_DATE = U("start_date");
static utility::string_t P_BEGIN_DATE = U("begin_date");
static utility::string_t P_END_DATE = U("end_date");
static utility::string_t P_BEGIN_INDEX = U("begin_index");
static utility::string_t P_END_INDEX = U("end_index");
static utility::string_t P_TRADE_SESSION = U("trade_session");
static utility::string_t P_EXPIRY = U("expiry");
static utility::string_t P_STRIKE = U("strike");
static utility::string_t P_RIGHT = U("right");
static utility::string_t P_DATE = U("date");
static utility::string_t P_TRADING_DATE = U("trading_date");
static utility::string_t P_LIMIT = U("limit");
static utility::string_t P_PAGE_TOKEN = U("page_token");
static utility::string_t P_SEC_TYPE = U("sec_type");
static utility::string_t P_SEG_TYPE = U("seg_type");
static utility::string_t P_CURRENCY = U("currency");
static utility::string_t P_EXCHANGE = U("exchange");
static utility::string_t P_TYPE = U("type");
static utility::string_t P_BEGIN  = U("begin");
static utility::string_t P_END  = U("end");
static utility::string_t P_TICK_SIZE = U("tick_size");
static utility::string_t P_EXCHANGE_CODE = U("exchange_code");
static utility::string_t P_PAGE = U("page");
static utility::string_t P_PAGE_SIZE = U("page_size");
static utility::string_t P_SORT_DIR = U("sort_dir");
static utility::string_t P_SORT_FIELD_NAME = U("sort_field_name");
static utility::string_t P_WITH_DETAILS = U("with_details");


static utility::string_t OPEN_API_SERVICE_VERSION = U("3.0");

// Tiger Brokers public key
static utility::string_t TIGER_PUBLIC_KEY = U("MIGfMA0GCSqGSIb3DQEBAQUAA4GNADCBiQKBgQDNF3G8SoEcCZh2rshUbayDgLLrj6rKgzNMxDL2HS"
                   "nKcB0+GPOsndqSv+a4IBu9+I3fyBp5hkyMMG2+AXugd9pMpy6VxJxlNjhX1MYbNTZJUT4nudki4uh+LM"
                   "OkIBHOceGNXjgB+cXqmlUnjlqha/HgboeHSnSgpM3dKSJQlIOsDwIDAQAB");
static utility::string_t SANDBOX_TIGER_PUBLIC_KEY = U("MIGfMA0GCSqGSIb3DQEBAQUAA4GNADCBiQKBgQCbm21i11hgAENGd3/f280PSe4g9YGkS3TEXBY"
                           "MidihTvHHf+tJ0PYD0o3PruI0hl3qhEjHTAxb75T5YD3SGK4IBhHn/Rk6mhqlGgI+bBrBVYaXixm"
                           "HfRo75RpUUuWACyeqQkZckgR0McxuW9xRMIa2cXZOoL1E4SL4lXKGhKoWbwIDAQAB");

/** HTTP interface service address **/
static utility::string_t TIGER_SERVER_URL = U("https://openapi.tigerfintech.com/gateway");
static utility::string_t SANDBOX_TIGER_SERVER_URL = U("https://openapi-sandbox.tigerfintech.com/gateway");

/** Push service address **/ 
static utility::string_t TIGER_SOCKET_HOST = U("openapi.tigerfintech.com");
static utility::string_t SANDBOX_TIGER_SOCKET_HOST = U("openapi-sandbox.tigerfintech.com");
static utility::string_t TIGER_SOCKET_PORT = U("9883");
static utility::string_t SANDBOX_TIGER_SOCKET_PORT = U("9885");

#endif //CPPSDK_CONSTANTS_H
