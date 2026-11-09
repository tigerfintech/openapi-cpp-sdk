// Response parsing tests for the news models (news_stock_list /
// news_highlight_list). The `data` envelope is camelCase on the wire
// (stock-openapi docs/news_api_design.md §3.3); these tests pin that
// NewsItem / NewsPage parse it exactly and stay default on missing fields.

#include <gtest/gtest.h>
#include "cpprest/json.h"
#include "tigerapi/news_model.h"

using namespace TIGER_API;
using web::json::value;

namespace {

value item_json() {
    return value::parse(U("{\"id\":\"123456\",\"title\":\"Apple Q2\","
                           "\"summary\":\"Earnings beat\","
                           "\"pubTimestamp\":1729413600,"
                           "\"url\":\"https://example.com/news/123456\"}"));
}

value page_json() {
    return value::parse(U("{\"items\":[{\"id\":\"1\",\"title\":\"t1\",\"summary\":\"s1\","
                           "\"pubTimestamp\":100,\"url\":\"https://a/1\"},"
                           "{\"id\":\"2\",\"title\":\"t2\",\"summary\":\"s2\","
                           "\"pubTimestamp\":200,\"url\":\"https://a/2\"}],"
                           "\"pageCount\":1,\"pageSize\":20,\"totalPage\":10,\"totalSize\":200}"));
}

} // namespace

// ── NewsItem parsing ──────────────────────────────────────────────────────────

TEST(NewsModelTest, NewsItem_ParsesAllCamelCaseFields) {
    NewsItem item;
    item.from_json(item_json());

    EXPECT_EQ(item.id, U("123456"));
    EXPECT_EQ(item.title, U("Apple Q2"));
    EXPECT_EQ(item.summary, U("Earnings beat"));
    EXPECT_EQ(item.pub_timestamp, 1729413600LL);
    EXPECT_EQ(item.url, U("https://example.com/news/123456"));
}

TEST(NewsModelTest, NewsItem_MissingFieldsKeepDefaults) {
    NewsItem item;
    item.from_json(value::object(true));

    EXPECT_EQ(item.id, U(""));
    EXPECT_EQ(item.title, U(""));
    EXPECT_EQ(item.summary, U(""));
    EXPECT_EQ(item.pub_timestamp, 0LL);
    EXPECT_EQ(item.url, U(""));
}

TEST(NewsModelTest, NewsItem_NullValueKeepsDefaults) {
    // data:null is a valid empty success envelope — parsing must not throw.
    NewsItem item;
    EXPECT_NO_THROW(item.from_json(value::null()));
    EXPECT_EQ(item.id, U(""));
    EXPECT_EQ(item.pub_timestamp, 0LL);
}

TEST(NewsModelTest, NewsItem_NullFieldsTreatedAsMissing) {
    NewsItem item;
    item.from_json(value::parse(U("{\"id\":\"1\",\"title\":null,\"summary\":null,"
                                  "\"pubTimestamp\":null,\"url\":null}")));

    EXPECT_EQ(item.id, U("1"));
    EXPECT_EQ(item.title, U(""));
    EXPECT_EQ(item.summary, U(""));
    EXPECT_EQ(item.pub_timestamp, 0LL);
    EXPECT_EQ(item.url, U(""));
}

TEST(NewsModelTest, NewsItem_IgnoresNonWhitelistedFields) {
    // The upstream carries ~37 fields; the published whitelist is 5.
    // Extra fields must be ignored, not crash the parser.
    NewsItem item;
    EXPECT_NO_THROW(item.from_json(value::parse(U("{\"id\":\"7\",\"sentiment\":1,"
                                                  "\"symbols\":[\"AAPL\"],\"pubTime\":\"2024-10-20\","
                                                  "\"thumbnails\":{},\"video\":{\"duration\":10}}"))));
    EXPECT_EQ(item.id, U("7"));
    EXPECT_EQ(item.pub_timestamp, 0LL);
}

// ── NewsPage parsing ──────────────────────────────────────────────────────────

TEST(NewsModelTest, NewsPage_ParsesEnvelopeAndItems) {
    NewsPage page;
    page.from_json(page_json());

    EXPECT_EQ(page.items.size(), 2u);
    EXPECT_EQ(page.page_count, 1);
    EXPECT_EQ(page.page_size, 20);
    EXPECT_EQ(page.total_page, 10);
    EXPECT_EQ(page.total_size, 200);

    EXPECT_EQ(page.items[0].id, U("1"));
    EXPECT_EQ(page.items[0].title, U("t1"));
    EXPECT_EQ(page.items[0].summary, U("s1"));
    EXPECT_EQ(page.items[0].pub_timestamp, 100LL);
    EXPECT_EQ(page.items[0].url, U("https://a/1"));
    EXPECT_EQ(page.items[1].id, U("2"));
    EXPECT_EQ(page.items[1].pub_timestamp, 200LL);
}

TEST(NewsModelTest, NewsPage_EmptyItemsIsValidSuccess) {
    // Empty items array is a normal success response, not an error.
    NewsPage page;
    page.from_json(value::parse(U("{\"items\":[],\"pageCount\":1,\"pageSize\":20,"
                                  "\"totalPage\":0,\"totalSize\":0}")));

    EXPECT_TRUE(page.items.empty());
    EXPECT_EQ(page.page_count, 1);
    EXPECT_EQ(page.total_size, 0);
}

TEST(NewsModelTest, NewsPage_MissingEnvelopeFieldsKeepDefaults) {
    NewsPage page;
    page.from_json(value::parse(U("{\"items\":[{\"id\":\"9\",\"title\":\"x\"}]}")));

    EXPECT_EQ(page.items.size(), 1u);
    EXPECT_EQ(page.items[0].id, U("9"));
    EXPECT_EQ(page.page_count, 0);
    EXPECT_EQ(page.page_size, 0);
    EXPECT_EQ(page.total_page, 0);
    EXPECT_EQ(page.total_size, 0);
}

TEST(NewsModelTest, NewsPage_NullDataYieldsEmptyPage) {
    NewsPage page;
    EXPECT_NO_THROW(page.from_json(value::null()));
    EXPECT_TRUE(page.items.empty());
    EXPECT_EQ(page.page_count, 0);
}

TEST(NewsModelTest, NewsPage_HighlightShapeIsSameEnvelope) {
    // news_highlight_list shares the envelope with news_stock_list; only
    // pageCount is always 1 (cursor-only pagination).
    NewsPage page;
    page.from_json(value::parse(U("{\"items\":[{\"id\":\"789\",\"title\":\"Fed cuts rates\","
                                  "\"summary\":\"FOMC decision\",\"pubTimestamp\":1761429600,"
                                  "\"url\":\"https://example.com/highlight/detail?id=789\"}],"
                                  "\"pageCount\":1,\"pageSize\":20,\"totalPage\":5,\"totalSize\":100}")));

    ASSERT_EQ(page.items.size(), 1u);
    EXPECT_EQ(page.items[0].id, U("789"));
    EXPECT_EQ(page.items[0].pub_timestamp, 1761429600LL);
    EXPECT_EQ(page.page_count, 1);
    EXPECT_EQ(page.total_size, 100);
}

// ── to_string smoke tests ─────────────────────────────────────────────────────

TEST(NewsModelTest, ToString_ContainsCoreFields) {
    NewsItem item;
    item.from_json(item_json());
    EXPECT_NE(item.to_string().find(U("123456")), utility::string_t::npos);
    EXPECT_NE(item.to_string().find(U("Apple Q2")), utility::string_t::npos);

    NewsPage page;
    page.from_json(page_json());
    EXPECT_NE(page.to_string().find(U("items: 2")), utility::string_t::npos);
}
