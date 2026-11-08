#ifndef TIGERAPI_QUOTE_DATA_UTIL_H
#define TIGERAPI_QUOTE_DATA_UTIL_H

#include <memory>
#include "openapi_pb/pb_source/QuoteData.pb.h"
#include "openapi_pb/pb_source/QuoteBasicData.pb.h"

namespace TIGER_API {
namespace detail {

// Preserve protobuf presence when converting the public basic-quote callback.
inline std::shared_ptr<tigeropen::push::pb::QuoteBasicData> convert_quote_basic_data(
    const tigeropen::push::pb::QuoteData& quote_data) {
    const auto type = quote_data.type();
    if (type != tigeropen::push::pb::SocketCommon_QuoteType_ALL &&
        type != tigeropen::push::pb::SocketCommon_QuoteType_BASIC) {
        return nullptr;
    }
    auto builder = std::make_shared<tigeropen::push::pb::QuoteBasicData>();
    builder->set_symbol(quote_data.symbol());
    builder->set_type(tigeropen::push::pb::SocketCommon_QuoteType_BASIC);
    builder->set_timestamp(quote_data.timestamp());
    if (quote_data.has_servertimestamp()) builder->set_servertimestamp(quote_data.servertimestamp());
    if (quote_data.has_avgprice()) builder->set_avgprice(quote_data.avgprice());
    if (quote_data.has_latestprice()) builder->set_latestprice(quote_data.latestprice());
    if (quote_data.has_latestpricetimestamp()) builder->set_latestpricetimestamp(quote_data.latestpricetimestamp());
    if (quote_data.has_latesttime()) builder->set_latesttime(quote_data.latesttime());
    if (quote_data.has_preclose()) builder->set_preclose(quote_data.preclose());
    if (quote_data.has_volume()) builder->set_volume(quote_data.volume());
    if (quote_data.has_volumedecimal()) builder->set_volumedecimal(quote_data.volumedecimal());
    if (quote_data.has_amount()) builder->set_amount(quote_data.amount());
    if (quote_data.has_open()) builder->set_open(quote_data.open());
    if (quote_data.has_high()) builder->set_high(quote_data.high());
    if (quote_data.has_low()) builder->set_low(quote_data.low());
    if (quote_data.has_hourtradingtag()) builder->set_hourtradingtag(quote_data.hourtradingtag());
    if (quote_data.has_marketstatus()) builder->set_marketstatus(quote_data.marketstatus());
    if (quote_data.has_identifier()) builder->set_identifier(quote_data.identifier());
    if (quote_data.has_openint()) builder->set_openint(quote_data.openint());
    if (quote_data.has_tradetime()) builder->set_tradetime(quote_data.tradetime());
    if (quote_data.has_presettlement()) builder->set_presettlement(quote_data.presettlement());
    if (quote_data.has_mintick()) builder->set_mintick(quote_data.mintick());
    if (quote_data.has_cumvolume()) builder->set_cumvolume(quote_data.cumvolume());
    if (quote_data.has_cumamount()) builder->set_cumamount(quote_data.cumamount());
    if (quote_data.has_session()) builder->set_session(quote_data.session());
    if (quote_data.has_mi()) builder->mutable_mi()->CopyFrom(quote_data.mi());
    return builder;
}

} // namespace detail
} // namespace TIGER_API

#endif
