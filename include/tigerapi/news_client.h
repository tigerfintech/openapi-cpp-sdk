//
// NewsClient — news_stock_list (个股新闻列表) / news_highlight_list (市场要闻列表).
//
// Both APIs ride the unified gateway (POST /gateway, method + biz_content,
// same signing as every other HTTP method) through TigerClient's single
// main-gateway URL — the one trade-domain methods use. This SDK has no
// quote-cluster URL, so news traffic can never be routed to a quote server.
//
// Wire contract: request biz_content params are snake_case; response `data`
// (camelCase) is parsed into NewsPage / NewsItem (news_model.h).
//

#ifndef TIGERAPI_NEWS_CLIENT_H
#define TIGERAPI_NEWS_CLIENT_H

#include "tiger_client.h"
#include "client_config.h"
#include "enums.h"
#include "service_types.h"
#include "news_model.h"

namespace TIGER_API {
    class OPENAPI_EXPORT NewsClient : public TigerClient {
    public:
        explicit NewsClient(const ClientConfig &cf);

        /**
         * news_stock_list — query the news list of a single stock symbol.
         *
         * @param symbol     required, single ticker (e.g. "AAPL"). The
         *                   downstream news service does not support batch
         *                   queries, so this API accepts exactly one symbol.
         * @param lang       optional, "zh_CN" / "zh_TW" / "en_US".
         * @param page_count optional page number, 0 (default) = server
         *                   default 1; explicit values are sent as-is
         *                   (server validates page_count >= 1).
         * @param page_size  optional page size, 0 (default) = server default
         *                   20; explicit values are sent as-is (server
         *                   validates 1 <= page_size <= 50).
         * @return NewsPage with items and the paging envelope.
         */
        NewsPage get_stock_news(const utility::string_t &symbol, const utility::string_t &lang = U(""),
                                int page_count = 0, int page_size = 0);

        NewsPage get_stock_news(const utility::string_t &symbol, Language lang,
                                int page_count = 0, int page_size = 0);

        /**
         * news_highlight_list — query the market highlight news list.
         *
         * Cursor-only pagination (no page_count / page_size on this API):
         * pass the id of the last item of the previous page as last_id to
         * fetch the next page; an empty last_id starts from the newest.
         *
         * @param lang    optional, same semantics as get_stock_news.
         * @param last_id optional cursor; empty (default) starts from the
         *                newest items.
         * @return NewsPage; page_count is always 1 for this API.
         */
        NewsPage get_market_news(const utility::string_t &lang = U(""), const utility::string_t &last_id = U(""));

        NewsPage get_market_news(Language lang, const utility::string_t &last_id = U(""));
    };
}

#endif //TIGERAPI_NEWS_CLIENT_H
