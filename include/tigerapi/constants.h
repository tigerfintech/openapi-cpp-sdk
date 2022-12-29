
#ifndef CPPSDK_CONSTANTS_H
#define CPPSDK_CONSTANTS_H

static std::string GET = "GET";
static std::string POST = "POST";

static std::string P_TIGER_ID = "tiger_id";
static std::string P_METHOD = "method";
static std::string P_CHARSET = "charset";
static std::string P_SIGN_TYPE = "sign_type";
static std::string P_SIGN = "sign";
static std::string P_TIMESTAMP = "timestamp";
static std::string P_VERSION = "version";
static std::string P_ITEMS = "items";
static std::string P_DATA = "data";
static std::string P_CODE = "code";
static std::string P_NOTIFY_URL = "notify_url";
static std::string P_DEVICE_ID = "device_id";
//static std::string P_SDK_VERSION = "sdk-version";
static std::string P_SDK_VERSION_PREFIX = "openapi-cpp-sdk-";
static std::string P_USER_AGENT = "User-Agent";
static std::string P_BIZ_CONTENT = "biz_content";
static std::string P_ACCOUNT = "account";
static std::string P_MARKET = "market";
static std::string P_SYMBOLS = "symbols";
static std::string P_SYMBOL = "symbol";
static std::string P_CONTRACT_CODES = "contract_codes";
static std::string P_CONTRACT_CODE = "contract_code";
static std::string P_PERIOD = "period";
static std::string P_BEGIN_TIME = "begin_time";
static std::string P_START_TIME = "start_time";
static std::string P_END_TIME = "end_time";
static std::string P_BEGIN_DATE = "begin_date";
static std::string P_END_DATE = "end_date";
static std::string P_BEGIN_INDEX = "begin_index";
static std::string P_END_INDEX = "end_index";
static std::string P_TRADE_SESSION = "trade_session";
static std::string P_EXPIRY = "expiry";
static std::string P_STRIKE = "strike";
static std::string P_RIGHT = "right";
static std::string P_DATE = "date";
static std::string P_TRADING_DATE = "trading_date";
static std::string P_LIMIT = "limit";
static std::string P_PAGE_TOKEN = "page_token";
static std::string P_SEC_TYPE = "sec_type";
static std::string P_SEG_TYPE = "seg_type";
static std::string P_CURRENCY = "currency";
static std::string P_EXCHANGE = "exchange";
static std::string P_TYPE = "type";
static std::string P_EXCHANGE_CODE = "exchange_code";

static std::string OPEN_API_SERVICE_VERSION = "3.0";

// 老虎证券开放平台公钥
static std::string TIGER_PUBLIC_KEY = "MIGfMA0GCSqGSIb3DQEBAQUAA4GNADCBiQKBgQDNF3G8SoEcCZh2rshUbayDgLLrj6rKgzNMxDL2HS" \
                   "nKcB0+GPOsndqSv+a4IBu9+I3fyBp5hkyMMG2+AXugd9pMpy6VxJxlNjhX1MYbNTZJUT4nudki4uh+LM" \
                   "OkIBHOceGNXjgB+cXqmlUnjlqha/HgboeHSnSgpM3dKSJQlIOsDwIDAQAB";
static std::string SANDBOX_TIGER_PUBLIC_KEY = "MIGfMA0GCSqGSIb3DQEBAQUAA4GNADCBiQKBgQCbm21i11hgAENGd3/f280PSe4g9YGkS3TEXBY" \
                           "MidihTvHHf+tJ0PYD0o3PruI0hl3qhEjHTAxb75T5YD3SGK4IBhHn/Rk6mhqlGgI+bBrBVYaXixm" \
                           "HfRo75RpUUuWACyeqQkZckgR0McxuW9xRMIa2cXZOoL1E4SL4lXKGhKoWbwIDAQAB";

/** http 接口服务地址 **/
static std::string TIGER_SERVER_URL = "https://openapi.tigerfintech.com/gateway";
static std::string SANDBOX_TIGER_SERVER_URL = "https://openapi-sandbox.tigerfintech.com/gateway";

                           
#endif //CPPSDK_CONSTANTS_H
