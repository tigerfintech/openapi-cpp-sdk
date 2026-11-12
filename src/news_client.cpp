//
// NewsClient implementation. Request bodies follow the wire contract
// (snake_case biz_content params, optional params omitted when unset);
// the camelCase `data` envelope is parsed by NewsPage::from_json.
//
#include "tigerapi/news_client.h"

namespace TIGER_API {

    NewsClient::NewsClient(const ClientConfig &cf) : TigerClient(cf) {
        // No quote permission grab: news is public market data (server-side
        // decision #3 — no entitlement gate), so construction performs no
        // network call, unlike QuoteClient.
    }

    NewsPage NewsClient::get_stock_news(const utility::string_t &symbol, const utility::string_t &lang,
                                        int page_count, int page_size) {
        value obj = value::object(true);
        // Single symbol only (wire key `symbol`, scalar — never `symbols`):
        // the downstream news service does not support batch queries.
        obj[P_SYMBOL] = value::string(symbol);
        if (!lang.empty()) {
            obj[P_LANG] = value::string(lang);
        }
        // 0 = unset → omit and let the server apply its defaults (1 / 20).
        // Any other explicit value is forwarded as-is; out-of-range values
        // are rejected by the server with a param error.
        if (page_count != 0) {
            obj[P_PAGE_COUNT] = value::number(page_count);
        }
        if (page_size != 0) {
            obj[P_PAGE_SIZE] = value::number(page_size);
        }
        value result = post(NEWS_STOCK_LIST, obj);
        NewsPage page;
        page.from_json(result);
        return page;
    }

    NewsPage NewsClient::get_stock_news(const utility::string_t &symbol, Language lang,
                                        int page_count, int page_size) {
        return get_stock_news(symbol, enum_to_str(lang), page_count, page_size);
    }

    NewsPage NewsClient::get_market_news(const utility::string_t &lang, const utility::string_t &last_id) {
        value obj = value::object(true);
        if (!lang.empty()) {
            obj[P_LANG] = value::string(lang);
        }
        // Cursor pagination: last_id is the id of the last item of the
        // previous page; empty = start from the newest. This API has no
        // page_count / page_size params (server decision #14).
        if (!last_id.empty()) {
            obj[P_LAST_ID] = value::string(last_id);
        }
        value result = post(NEWS_HIGHLIGHT_LIST, obj);
        NewsPage page;
        page.from_json(result);
        return page;
    }

    NewsPage NewsClient::get_market_news(Language lang, const utility::string_t &last_id) {
        return get_market_news(enum_to_str(lang), last_id);
    }
}
