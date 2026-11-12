// Covers the SUBSCRIBE frame builder used by subscribe_asset / subscribe_position.

#include <gtest/gtest.h>
#include <vector>
#include "tigerapi/push_socket/subscribe_request.h"

using namespace TIGER_API;
using tigeropen::push::pb::Request;
using tigeropen::push::pb::SocketCommon_Command_SUBSCRIBE;
using tigeropen::push::pb::SocketCommon_Command_UNSUBSCRIBE;
using tigeropen::push::pb::SocketCommon_DataType_Asset;
using tigeropen::push::pb::SocketCommon_DataType_Position;

// ── Frame shape ───────────────────────────────────────────────────────────────

TEST(PushSubscribeMessageTest, SetsCommandDataTypeAndAccount) {
    Request req = build_trade_request(SocketCommon_Command_SUBSCRIBE,
                                      SocketCommon_DataType_Asset,
                                      "ACCT", "", 42u);

    EXPECT_EQ(req.command(), SocketCommon_Command_SUBSCRIBE);
    EXPECT_EQ(req.id(), 42u);
    EXPECT_EQ(req.subscribe().datatype(), SocketCommon_DataType_Asset);
    EXPECT_EQ(req.subscribe().account(), "ACCT");
}

TEST(PushSubscribeMessageTest, UnsubscribeKeepsCommand) {
    Request req = build_trade_request(SocketCommon_Command_UNSUBSCRIBE,
                                      SocketCommon_DataType_Position,
                                      "ACCT", "", 7u);

    EXPECT_EQ(req.command(), SocketCommon_Command_UNSUBSCRIBE);
    EXPECT_EQ(req.subscribe().datatype(), SocketCommon_DataType_Position);
}

// ── assetQuoteType (field 5) ──────────────────────────────────────────────────

TEST(PushSubscribeMessageTest, AssetQuoteType_Overnight) {
    Request req = build_trade_request(SocketCommon_Command_SUBSCRIBE,
                                      SocketCommon_DataType_Asset,
                                      "ACCT", "OVERNIGHT", 1u);

    EXPECT_EQ(req.subscribe().assetquotetype(), "OVERNIGHT");
}

TEST(PushSubscribeMessageTest, AssetQuoteType_Rth) {
    Request req = build_trade_request(SocketCommon_Command_SUBSCRIBE,
                                      SocketCommon_DataType_Position,
                                      "ACCT", "RTH", 1u);

    EXPECT_EQ(req.subscribe().assetquotetype(), "RTH");
}

// An empty quote type must leave the field off the wire entirely, so a client
// that never passes one keeps producing the exact bytes it produced before the
// field existed.
TEST(PushSubscribeMessageTest, EmptyQuoteType_NotSerialized) {
    Request with_type = build_trade_request(SocketCommon_Command_SUBSCRIBE,
                                            SocketCommon_DataType_Asset,
                                            "ACCT", "OVERNIGHT", 1u);
    Request without_type = build_trade_request(SocketCommon_Command_SUBSCRIBE,
                                               SocketCommon_DataType_Asset,
                                               "ACCT", "", 1u);

    EXPECT_TRUE(without_type.subscribe().assetquotetype().empty());

    const std::string with_bytes = with_type.SerializeAsString();
    const std::string without_bytes = without_type.SerializeAsString();
    EXPECT_NE(with_bytes.find("OVERNIGHT"), std::string::npos);
    // Nothing is emitted for the empty field: the frame is exactly the tag,
    // length and payload of "OVERNIGHT" shorter (1 + 1 + 9). Both nested-message
    // lengths still fit in one varint byte, so no further bytes are added.
    EXPECT_EQ(without_bytes.size() + 11, with_bytes.size());

    // Round-trip: a server reading the shorter frame sees no quote type at all.
    Request reparsed;
    ASSERT_TRUE(reparsed.ParseFromString(without_bytes));
    EXPECT_TRUE(reparsed.subscribe().assetquotetype().empty());
}

TEST(PushSubscribeMessageTest, EmptyAccount_NotSerialized) {
    Request req = build_trade_request(SocketCommon_Command_SUBSCRIBE,
                                      SocketCommon_DataType_Asset,
                                      "", "", 1u);

    EXPECT_TRUE(req.subscribe().account().empty());
}

#include "tigerapi/push_socket/quote_data_util.h"
// Fixtures generated from stock-openapi feat_gth commit 33df7142f.
TEST(PushSubscribeMessageTest, GthServerWireSurvivesBasicCallbackConversion) {
    struct Fixture { std::string hex; double price; uint64_t time; int64_t volume; double amount; };
    for (const auto& f : std::vector<Fixture>{
        {"0a185350585720323032363037323320373338302e3020505554100118a4fb9df1f83331000000000000f83f3889fa9df1f833499a9999999999f13f500e5900000000000029407a095072654d61726b6574e9010000000000001c40f001d00ff901000000008087c340820203475448", 1.5, 1784796380425ULL, 7, 12.5},
        {"0a185350585720323032363037323320373338302e3020505554100118a4fb9df1f8333100000000000000003800499a9999999999f13f50005900000000000000007a095072654d61726b6574e9010000000000000000f001d00ff901000000008087c340820203475448", 0, 0, 0, 0},
    }) {
        std::string bytes;
        for (size_t i = 0; i < f.hex.size(); i += 2)
            bytes.push_back(static_cast<char>(std::stoi(f.hex.substr(i, 2), nullptr, 16)));
        tigeropen::push::pb::QuoteData q;
        ASSERT_TRUE(q.ParseFromString(bytes));
        auto basic = detail::convert_quote_basic_data(q);
        ASSERT_NE(basic, nullptr);
        EXPECT_EQ(basic->type(), tigeropen::push::pb::SocketCommon_QuoteType_BASIC);
        EXPECT_TRUE(basic->has_latestprice());
        EXPECT_DOUBLE_EQ(basic->latestprice(), f.price);
        EXPECT_TRUE(basic->has_latestpricetimestamp());
        EXPECT_EQ(basic->latestpricetimestamp(), f.time);
        EXPECT_TRUE(basic->has_volume());
        EXPECT_EQ(basic->volume(), f.volume);
        EXPECT_TRUE(basic->has_volumedecimal());
        EXPECT_DOUBLE_EQ(basic->volumedecimal(), f.volume);
        EXPECT_TRUE(basic->has_amount());
        EXPECT_DOUBLE_EQ(basic->amount(), f.amount);
        EXPECT_TRUE(basic->has_cumvolume());
        EXPECT_EQ(basic->cumvolume(), 1000);
        EXPECT_TRUE(basic->has_cumamount());
        EXPECT_DOUBLE_EQ(basic->cumamount(), 9999);
        EXPECT_EQ(basic->session(), "GTH");
        EXPECT_EQ(basic->hourtradingtag(), "PreMarket");
        EXPECT_DOUBLE_EQ(basic->preclose(), 1.1);
        EXPECT_EQ(q.GetDescriptor()->FindFieldByName("hourTrading"), nullptr);
        EXPECT_EQ(basic->GetDescriptor()->FindFieldByName("hourTrading"), nullptr);
    }
}

TEST(PushSubscribeMessageTest, AllQuoteProducesBasicCallbackType) {
    tigeropen::push::pb::QuoteData quote;
    quote.set_type(tigeropen::push::pb::SocketCommon_QuoteType_ALL);
    quote.set_symbol("AAPL");
    quote.set_latestprice(1.5);
    quote.set_latestpricetimestamp(1784796380425ULL);
    quote.set_volume(7);
    quote.set_amount(12.5);
    quote.set_askprice(1.6);
    quote.set_bidprice(1.4);

    const auto basic = detail::convert_quote_basic_data(quote);

    ASSERT_NE(basic, nullptr);
    EXPECT_EQ(basic->type(), tigeropen::push::pb::SocketCommon_QuoteType_BASIC);
    EXPECT_EQ(basic->symbol(), quote.symbol());
    EXPECT_TRUE(basic->has_latestprice());
    EXPECT_DOUBLE_EQ(basic->latestprice(), 1.5);
    EXPECT_EQ(basic->latestpricetimestamp(), 1784796380425ULL);
    EXPECT_EQ(basic->volume(), 7);
    EXPECT_DOUBLE_EQ(basic->amount(), 12.5);
    EXPECT_FALSE(basic->has_cumvolume());
    EXPECT_FALSE(basic->has_cumamount());
    EXPECT_FALSE(basic->has_session());
    EXPECT_FALSE(basic->has_hourtradingtag());
    EXPECT_EQ(quote.type(), tigeropen::push::pb::SocketCommon_QuoteType_ALL);
}

TEST(PushSubscribeMessageTest, GthBasicConversionKeepsMissingFieldsAbsent) {
    tigeropen::push::pb::QuoteData q;
    q.set_type(tigeropen::push::pb::SocketCommon_QuoteType_BASIC);
    auto basic = detail::convert_quote_basic_data(q);
    ASSERT_NE(basic, nullptr);
    EXPECT_FALSE(basic->has_latestprice());
    EXPECT_FALSE(basic->has_latestpricetimestamp());
    EXPECT_FALSE(basic->has_latesttime());
    EXPECT_FALSE(basic->has_preclose());
    EXPECT_FALSE(basic->has_volume());
    EXPECT_FALSE(basic->has_volumedecimal());
    EXPECT_FALSE(basic->has_amount());
    EXPECT_FALSE(basic->has_cumvolume());
    EXPECT_FALSE(basic->has_cumamount());
    EXPECT_FALSE(basic->has_session());
    EXPECT_FALSE(basic->has_hourtradingtag());
}
