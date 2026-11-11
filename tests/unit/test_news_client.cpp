// Wire-key / routing / lang contract tests for NewsClient
// (news_stock_list / news_highlight_list).
//
// Requests are captured through a loopback http_listener standing in for the
// main gateway URL (no real network). The listener signs its responses with a
// locally generated RSA key so the client's response-signature verification
// passes and the full success path (request -> response -> NewsPage) runs.

#include <gtest/gtest.h>
#include <openssl/evp.h>
#include <openssl/pem.h>
#include "cpprest/http_listener.h"
#include "cpprest/json.h"
#include "support/test_support.h"
#include "tigerapi/constants.h"
#include "tigerapi/enums.h"
#include "tigerapi/news_client.h"
#include "tigerapi/quote_client.h"
#include "tigerapi/service_types.h"
#include "tigerapi/utils.h"

using namespace TIGER_API;
using namespace web;
using json::value;

namespace {

struct TestKeyPair {
    utility::string_t private_pem;
    utility::string_t public_pem;
};

// Generate one RSA keypair for the whole fixture (signing responses).
const TestKeyPair &test_key_pair() {
    static const TestKeyPair keys = [] {
        EVP_PKEY_CTX *ctx = EVP_PKEY_CTX_new_id(EVP_PKEY_RSA, nullptr);
        EVP_PKEY *key = nullptr;
        if (!ctx || EVP_PKEY_keygen_init(ctx) <= 0 ||
            EVP_PKEY_CTX_set_rsa_keygen_bits(ctx, 2048) <= 0 ||
            EVP_PKEY_keygen(ctx, &key) <= 0) {
            EVP_PKEY_CTX_free(ctx);
            throw std::runtime_error("failed to generate test RSA key");
        }
        EVP_PKEY_CTX_free(ctx);

        BIO *priv_bio = BIO_new(BIO_s_mem());
        BIO *pub_bio = BIO_new(BIO_s_mem());
        if (!priv_bio || !pub_bio ||
            PEM_write_bio_PrivateKey(priv_bio, key, nullptr, nullptr, 0, nullptr, nullptr) != 1 ||
            PEM_write_bio_PUBKEY(pub_bio, key) != 1) {
            BIO_free(priv_bio);
            BIO_free(pub_bio);
            EVP_PKEY_free(key);
            throw std::runtime_error("failed to encode test RSA key");
        }
        char *priv_data = nullptr;
        char *pub_data = nullptr;
        const long priv_size = BIO_get_mem_data(priv_bio, &priv_data);
        const long pub_size = BIO_get_mem_data(pub_bio, &pub_data);
        TestKeyPair pair{
            Utils::str8to16(std::string(priv_data, static_cast<size_t>(priv_size))),
            Utils::str8to16(std::string(pub_data, static_cast<size_t>(pub_size)))};
        BIO_free(priv_bio);
        BIO_free(pub_bio);
        EVP_PKEY_free(key);
        return pair;
    }();
    return keys;
}

class NewsRequestCaptureTest : public ::testing::Test {
protected:
    NewsRequestCaptureTest()
        : listener_(U("http://127.0.0.1:39093")) {
        listener_.support(methods::POST, [this](http_request request) {
            captured_ = request.extract_json().get();
            value response = value::object(true);
            response[P_CODE] = value::number(response_code_);
            if (response_code_ == 0) {
                response[U("message")] = value::string(U("success"));
                response[P_DATA] = response_data_;
                // Mirror the real gateway: sign the request's timestamp param
                // so the client's response-signature verification succeeds.
                response[P_SIGN] = value::string(
                    Utils::get_sign(test_key_pair().private_pem,
                                    captured_.at(P_TIMESTAMP).as_string()));
            } else {
                response[U("message")] = value::string(U("biz param error"));
            }
            request.reply(status_codes::OK, response).wait();
        });
        listener_.open().wait();
    }

    ~NewsRequestCaptureTest() override {
        listener_.close().wait();
    }

    std::unique_ptr<NewsClient> make_client(const utility::string_t &lang = U("")) {
        ClientConfig config(U("test_tiger_id"), test_key_pair().private_pem,
                            U("test_account"), /*sandbox_debug=*/false, lang);
        // The main-gateway URL: exactly what trade-domain methods use.
        config.set_server_url(U("http://127.0.0.1:39093"));
        config.set_server_public_key(test_key_pair().public_pem);
        return std::make_unique<NewsClient>(config);
    }

    value biz_content() const {
        return value::parse(captured_.at(P_BIZ_CONTENT).as_string());
    }

    web::http::experimental::listener::http_listener listener_;
    value captured_;
    value response_data_ = value::object(true);
    int response_code_ = 0;
};

value sample_page_data() {
    return value::parse(U("{\"items\":[{\"id\":\"123456\",\"title\":\"Apple Q2\","
                           "\"summary\":\"Earnings beat\",\"pubTimestamp\":1729413600,"
                           "\"url\":\"https://example.com/news/123456\"}],"
                           "\"pageCount\":1,\"pageSize\":20,\"totalPage\":10,\"totalSize\":200}"));
}

} // namespace

// ── news_stock_list wire keys ─────────────────────────────────────────────────

TEST_F(NewsRequestCaptureTest, GetStockNewsFullWireAndParse) {
    response_data_ = sample_page_data();
    auto client = make_client();

    NewsPage page;
    EXPECT_NO_THROW(page = client->get_stock_news(U("AAPL"), U("zh_CN"), 1, 20));

    EXPECT_EQ(captured_.at(P_METHOD).as_string(), U("news_stock_list"));
    // No per-endpoint version override for news: global default applies.
    EXPECT_EQ(captured_.at(P_VERSION).as_string(), OPEN_API_SERVICE_VERSION);
    const value content = biz_content();
    // Single scalar `symbol` — never a `symbols` array (no batch support).
    EXPECT_EQ(content.at(P_SYMBOL).as_string(), U("AAPL"));
    EXPECT_FALSE(content.has_field(P_SYMBOLS));
    EXPECT_EQ(content.at(P_PAGE_COUNT).as_integer(), 1);
    EXPECT_EQ(content.at(P_PAGE_SIZE).as_integer(), 20);
    EXPECT_EQ(content.at(P_LANG).as_string(), U("zh_CN"));

    // camelCase `data` parsed into the typed page end-to-end.
    EXPECT_EQ(page.page_count, 1);
    EXPECT_EQ(page.page_size, 20);
    EXPECT_EQ(page.total_page, 10);
    EXPECT_EQ(page.total_size, 200);
    ASSERT_EQ(page.items.size(), 1u);
    EXPECT_EQ(page.items[0].id, U("123456"));
    EXPECT_EQ(page.items[0].title, U("Apple Q2"));
    EXPECT_EQ(page.items[0].summary, U("Earnings beat"));
    EXPECT_EQ(page.items[0].pub_timestamp, 1729413600LL);
    EXPECT_EQ(page.items[0].url, U("https://example.com/news/123456"));
}

TEST_F(NewsRequestCaptureTest, GetStockNewsOmitsUnsetOptionalParams) {
    auto client = make_client();  // config lang empty

    response_data_ = value::parse(U("{\"items\":[],\"pageCount\":1,\"pageSize\":20,"
                                    "\"totalPage\":0,\"totalSize\":0}"));
    NewsPage page;
    EXPECT_NO_THROW(page = client->get_stock_news(U("AAPL")));

    EXPECT_EQ(captured_.at(P_METHOD).as_string(), U("news_stock_list"));
    const value content = biz_content();
    EXPECT_EQ(content.at(P_SYMBOL).as_string(), U("AAPL"));
    // Unset optionals are omitted: server applies its defaults (1 / 20,
    // mixed-language content) — never sent as nulls or zeros.
    EXPECT_FALSE(content.has_field(P_PAGE_COUNT));
    EXPECT_FALSE(content.has_field(P_PAGE_SIZE));
    EXPECT_FALSE(content.has_field(P_LANG));
    EXPECT_TRUE(page.items.empty());
}

TEST_F(NewsRequestCaptureTest, GetStockNewsPassesExplicitPagingThrough) {
    auto client = make_client();

    EXPECT_NO_THROW(client->get_stock_news(U("TSLA"), U(""), 3, 50));

    const value content = biz_content();
    EXPECT_EQ(content.at(P_SYMBOL).as_string(), U("TSLA"));
    EXPECT_EQ(content.at(P_PAGE_COUNT).as_integer(), 3);
    EXPECT_EQ(content.at(P_PAGE_SIZE).as_integer(), 50);
    // Explicit values are forwarded as-is; out-of-range validation happens
    // server-side (contract §3.1).
    EXPECT_NO_THROW(client->get_stock_news(U("TSLA"), U(""), 0, 51));
    EXPECT_EQ(biz_content().at(P_PAGE_SIZE).as_integer(), 51);
}

TEST_F(NewsRequestCaptureTest, GetStockNewsEnumLangOverload) {
    auto client = make_client();

    EXPECT_NO_THROW(client->get_stock_news(U("AAPL"), Language::zh_TW));

    EXPECT_EQ(biz_content().at(P_LANG).as_string(), U("zh_TW"));
}

TEST_F(NewsRequestCaptureTest, GetStockNewsServerParamErrorPropagates) {
    response_code_ = 4;  // server biz param error
    auto client = make_client();

    EXPECT_THROW(client->get_stock_news(U(""), U(""), 0, 0), std::runtime_error);
    // The request was still sent through the gateway (server-side validation).
    EXPECT_EQ(captured_.at(P_METHOD).as_string(), U("news_stock_list"));
    EXPECT_EQ(biz_content().at(P_SYMBOL).as_string(), U(""));
}

// ── news_highlight_list wire keys ─────────────────────────────────────────────

TEST_F(NewsRequestCaptureTest, GetMarketNewsCursorWireFormat) {
    response_data_ = sample_page_data();
    auto client = make_client();

    NewsPage page;
    EXPECT_NO_THROW(page = client->get_market_news(U("en_US"), U("789")));

    EXPECT_EQ(captured_.at(P_METHOD).as_string(), U("news_highlight_list"));
    EXPECT_EQ(captured_.at(P_VERSION).as_string(), OPEN_API_SERVICE_VERSION);
    const value content = biz_content();
    EXPECT_EQ(content.at(P_LAST_ID).as_string(), U("789"));
    EXPECT_EQ(content.at(P_LANG).as_string(), U("en_US"));
    // Cursor-only pagination: this API must never carry page_count/page_size.
    EXPECT_FALSE(content.has_field(P_PAGE_COUNT));
    EXPECT_FALSE(content.has_field(P_PAGE_SIZE));
    EXPECT_FALSE(content.has_field(P_PAGE));

    EXPECT_EQ(page.page_count, 1);
    ASSERT_EQ(page.items.size(), 1u);
    EXPECT_EQ(page.items[0].id, U("123456"));
}

TEST_F(NewsRequestCaptureTest, GetMarketNewsOmitsEmptyCursorAndLang) {
    auto client = make_client();  // config lang empty

    NewsPage page;
    EXPECT_NO_THROW(page = client->get_market_news());

    EXPECT_EQ(captured_.at(P_METHOD).as_string(), U("news_highlight_list"));
    // Empty object body → biz_content omitted entirely (SDK-wide behavior for
    // empty params): first page from the newest, mixed-language content.
    EXPECT_FALSE(captured_.has_field(P_BIZ_CONTENT));
    EXPECT_TRUE(page.items.empty());
}

TEST_F(NewsRequestCaptureTest, GetMarketNewsEnumLangOverload) {
    auto client = make_client();

    EXPECT_NO_THROW(client->get_market_news(Language::zh_CN, U("42")));

    const value content = biz_content();
    EXPECT_EQ(content.at(P_LANG).as_string(), U("zh_CN"));
    EXPECT_EQ(content.at(P_LAST_ID).as_string(), U("42"));
}

// ── lang semantics ────────────────────────────────────────────────────────────

TEST_F(NewsRequestCaptureTest, LangInheritsClientConfigWhenOmitted) {
    // Per-call lang empty → the config language is injected by the shared
    // send_request path (Python SDK parity: omitted lang inherits the client
    // config language).
    auto client = make_client(U("en_US"));

    EXPECT_NO_THROW(client->get_stock_news(U("AAPL")));
    EXPECT_EQ(biz_content().at(P_LANG).as_string(), U("en_US"));

    EXPECT_NO_THROW(client->get_market_news(U(""), U("5")));
    EXPECT_EQ(biz_content().at(P_LANG).as_string(), U("en_US"));
}

TEST_F(NewsRequestCaptureTest, LangAbsentWhenNeitherCallNorConfigSetsIt) {
    auto client = make_client();  // config lang empty, no per-call lang

    EXPECT_NO_THROW(client->get_stock_news(U("AAPL")));
    EXPECT_FALSE(biz_content().has_field(P_LANG));
    // No lang on the wire → server returns mixed-language content.
}

TEST_F(NewsRequestCaptureTest, LangConfigTakesPrecedenceOverPerCallValue) {
    // Existing SDK-wide behavior: send_request overwrites the body's lang
    // with ClientConfig::lang when the config language is non-empty. Pinned
    // here so news users are not surprised by it.
    auto client = make_client(U("en_US"));

    EXPECT_NO_THROW(client->get_stock_news(U("AAPL"), U("zh_CN")));
    EXPECT_EQ(biz_content().at(P_LANG).as_string(), U("en_US"));
}

// ── routing: main gateway, never a quote URL ──────────────────────────────────

TEST_F(NewsRequestCaptureTest, NewsRoutesToMainGatewayNotQuote) {
    // This SDK has a single gateway URL: TigerClient's http_client_ is built
    // from ClientConfig::get_server_url() — the main gateway trade-domain
    // methods use — and no quote-cluster URL exists anywhere in the SDK.
    // The listener above is registered as that main gateway URL; if news were
    // ever routed to any other (quote-domain) URL, the request would never
    // arrive here and the assertions below would fail.
    auto client = make_client();
    EXPECT_EQ(client->client_config.get_server_url(), U("http://127.0.0.1:39093"));

    EXPECT_NO_THROW(client->get_stock_news(U("AAPL")));
    EXPECT_EQ(captured_.at(P_METHOD).as_string(), U("news_stock_list"));

    EXPECT_NO_THROW(client->get_market_news());
    EXPECT_EQ(captured_.at(P_METHOD).as_string(), U("news_highlight_list"));

    // Control group (Java testNewsRoutesToMainGatewayNotQuote parity): a
    // quote-domain method travels through the exact same single main-gateway
    // http client in this SDK.
    ClientConfig quote_config(U("test_tiger_id"), test_key_pair().private_pem,
                              U("test_account"), /*sandbox_debug=*/false, U(""));
    quote_config.set_server_url(U("http://127.0.0.1:39093"));
    quote_config.set_server_public_key(test_key_pair().public_pem);
    QuoteClient quote_client(quote_config, /*is_grab_permission=*/false);
    EXPECT_NO_THROW(quote_client.get_market_state(U("US")));
    EXPECT_EQ(captured_.at(P_METHOD).as_string(), U("market_state"));
}

TEST(NewsClientRoutingTest, DefaultServerUrlIsMainGateway) {
    // Without any override the news client talks to the default main gateway
    // (https://openapi.tigerfintech.com/gateway) — the same constant every
    // trade/quote method of this SDK uses; there is no news-specific or
    // quote-cluster URL.
    auto cfg = std::make_shared<ClientConfig>(test::FAKE_TIGER_ID, test::FAKE_PRIVATE_KEY,
                                              test::FAKE_ACCOUNT, /*sandbox_debug=*/false);
    NewsClient client(*cfg);
    EXPECT_EQ(client.client_config.get_server_url(), TIGER_SERVER_URL);
}
