
#ifndef CPPSDK_CONSTANTS_H
#define CPPSDK_CONSTANTS_H

static std::string GET = U("GET");
static std::string POST = U("POST");

static std::string P_TIGER_ID = U("tiger_id");
static std::string P_METHOD = U("method");
static std::string P_CHARSET = U("charset");
static std::string P_SIGN_TYPE = U("sign_type");
static std::string P_SIGN = U("sign");
static std::string P_TIMESTAMP = U("timestamp");
static std::string P_VERSION = U("version");
static std::string P_ITEMS = U("items");
static std::string P_DATA = U("data");
static std::string P_CODE = U("code");
static std::string P_NOTIFY_URL = U("notify_url");
static std::string P_DEVICE_ID = U("device_id");
//static std::string P_SDK_VERSION = U("sdk-version");
static std::string P_SDK_VERSION_PREFIX = U("openapi-cpp-sdk-");
static std::string P_USER_AGENT = U("User-Agent");
static std::string P_BIZ_CONTENT = U("biz_content");
static std::string P_ACCOUNT = U("account");
static std::string P_MARKET = U("market");
static std::string P_SYMBOLS = U("symbols");
static std::string P_SYMBOL = U("symbol");
static std::string P_CONTRACT_CODES = U("contract_codes");
static std::string P_CONTRACT_CODE = U("contract_code");
static std::string P_PERIOD = U("period");
static std::string P_BEGIN_TIME = U("begin_time");
static std::string P_START_TIME = U("start_time");
static std::string P_END_TIME = U("end_time");
static std::string P_BEGIN_DATE = U("begin_date");
static std::string P_END_DATE = U("end_date");
static std::string P_BEGIN_INDEX = U("begin_index");
static std::string P_END_INDEX = U("end_index");
static std::string P_TRADE_SESSION = U("trade_session");
static std::string P_EXPIRY = U("expiry");
static std::string P_STRIKE = U("strike");
static std::string P_RIGHT = U("right");
static std::string P_DATE = U("date");
static std::string P_TRADING_DATE = U("trading_date");
static std::string P_LIMIT = U("limit");
static std::string P_PAGE_TOKEN = U("page_token");
static std::string P_SEC_TYPE = U("sec_type");
static std::string P_SEG_TYPE = U("seg_type");
static std::string P_CURRENCY = U("currency");
static std::string P_EXCHANGE = U("exchange");
static std::string P_TYPE = U("type");
static std::string P_EXCHANGE_CODE = U("exchange_code");

static std::string OPEN_API_SERVICE_VERSION = U("3.0");

// 老虎证券开放平台公钥
static std::string TIGER_PUBLIC_KEY = U("MIGfMA0GCSqGSIb3DQEBAQUAA4GNADCBiQKBgQDNF3G8SoEcCZh2rshUbayDgLLrj6rKgzNMxDL2HS") \
                   U("nKcB0+GPOsndqSv+a4IBu9+I3fyBp5hkyMMG2+AXugd9pMpy6VxJxlNjhX1MYbNTZJUT4nudki4uh+LM") \
                   U("OkIBHOceGNXjgB+cXqmlUnjlqha/HgboeHSnSgpM3dKSJQlIOsDwIDAQAB");
static std::string SANDBOX_TIGER_PUBLIC_KEY = U("MIGfMA0GCSqGSIb3DQEBAQUAA4GNADCBiQKBgQCbm21i11hgAENGd3/f280PSe4g9YGkS3TEXBY") \
                           U("MidihTvHHf+tJ0PYD0o3PruI0hl3qhEjHTAxb75T5YD3SGK4IBhHn/Rk6mhqlGgI+bBrBVYaXixm") \
                           U("HfRo75RpUUuWACyeqQkZckgR0McxuW9xRMIa2cXZOoL1E4SL4lXKGhKoWbwIDAQAB");

/** http 接口服务地址 **/
static std::string TIGER_SERVER_URL = U("https://openapi.tigerfintech.com/gateway");
static std::string SANDBOX_TIGER_SERVER_URL = U("https://openapi-sandbox.tigerfintech.com/gateway");

                           
#endif //CPPSDK_CONSTANTS_H
