//
// News response models for news_stock_list / news_highlight_list.
// Wire contract (stock-openapi docs/news_api_design.md §3.3): the `data`
// envelope uses camelCase field names; items expose the 5-field whitelist
// id / title / summary / pubTimestamp / url.
//

#ifndef TIGERAPI_NEWS_MODEL_H
#define TIGERAPI_NEWS_MODEL_H

#include <string>
#include <vector>
#include "cpprest/json.h"
#include "cpprest/details/basic_types.h"
#include "win32.h"

using namespace std;

namespace TIGER_API {

    /** One news entry. Shared by news_stock_list and news_highlight_list. */
    class OPENAPI_EXPORT NewsItem {
    public:
        utility::string_t id;
        utility::string_t title;
        utility::string_t summary;
        // Publish timestamp in seconds (wire field `pubTimestamp`).
        long long pub_timestamp = 0;
        utility::string_t url;

        void from_json(const web::json::value &j) {
            if (j.is_null() || !j.is_object()) {
                return;
            }
            if (j.has_field(U("id")) && !j.at(U("id")).is_null()) {
                id = j.at(U("id")).as_string();
            }
            if (j.has_field(U("title")) && !j.at(U("title")).is_null()) {
                title = j.at(U("title")).as_string();
            }
            if (j.has_field(U("summary")) && !j.at(U("summary")).is_null()) {
                summary = j.at(U("summary")).as_string();
            }
            if (j.has_field(U("pubTimestamp")) && !j.at(U("pubTimestamp")).is_null()) {
                pub_timestamp = j.at(U("pubTimestamp")).as_number().to_int64();
            }
            if (j.has_field(U("url")) && !j.at(U("url")).is_null()) {
                url = j.at(U("url")).as_string();
            }
        }

        utility::string_t to_string() const {
            utility::stringstream_t ss;
            ss << U("NewsItem<{id: ") << id
               << U(", title: ") << title
               << U(", summary: ") << summary
               << U(", pub_timestamp: ") << pub_timestamp
               << U(", url: ") << url
               << U("}>");
            return ss.str();
        }
    };

    /**
     * Paged news envelope (`data` of both news APIs). Shared by
     * news_stock_list and news_highlight_list. For news_highlight_list the
     * cursor (last_id) is the id of the last item of the previous page, and
     * page_count is always 1 (cursor-only pagination).
     */
    class OPENAPI_EXPORT NewsPage {
    public:
        vector<NewsItem> items;
        int page_count = 0;
        int page_size = 0;
        int total_page = 0;
        int total_size = 0;

        void from_json(const web::json::value &j) {
            if (j.is_null() || !j.is_object()) {
                return;
            }
            if (j.has_field(U("items")) && j.at(U("items")).is_array()) {
                for (const auto &item: j.at(U("items")).as_array()) {
                    NewsItem news_item;
                    news_item.from_json(item);
                    items.push_back(news_item);
                }
            }
            if (j.has_field(U("pageCount")) && !j.at(U("pageCount")).is_null()) {
                page_count = j.at(U("pageCount")).as_integer();
            }
            if (j.has_field(U("pageSize")) && !j.at(U("pageSize")).is_null()) {
                page_size = j.at(U("pageSize")).as_integer();
            }
            if (j.has_field(U("totalPage")) && !j.at(U("totalPage")).is_null()) {
                total_page = j.at(U("totalPage")).as_integer();
            }
            if (j.has_field(U("totalSize")) && !j.at(U("totalSize")).is_null()) {
                total_size = j.at(U("totalSize")).as_integer();
            }
        }

        utility::string_t to_string() const {
            utility::stringstream_t ss;
            ss << U("NewsPage<{items: ") << items.size()
               << U(", page_count: ") << page_count
               << U(", page_size: ") << page_size
               << U(", total_page: ") << total_page
               << U(", total_size: ") << total_size
               << U("}>");
            return ss.str();
        }
    };
}

#endif //TIGERAPI_NEWS_MODEL_H
