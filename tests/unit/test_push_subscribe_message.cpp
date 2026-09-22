// Covers the SUBSCRIBE frame builder used by subscribe_asset / subscribe_position.

#include <gtest/gtest.h>
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
