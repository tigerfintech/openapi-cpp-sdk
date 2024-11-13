#include "tigerapi/push_socket/push_client_impl.h"
#include "tigerapi/client_config.h"
#include "tigerapi/push_socket/push_socket.h"
#include "tigerapi/tick_util.h"
#include "openapi_pb\pb_source\PushData.pb.h"
#include "google/protobuf/util/json_util.h"
#include <vector>
#include <memory>
#include <cmath>
#include <numeric>

std::shared_ptr<TIGER_API::PushClientImpl> TIGER_API::PushClientImpl::create_push_client_impl(const TIGER_API::ClientConfig& client_config)
{
	return std::shared_ptr<TIGER_API::PushClientImpl>(new TIGER_API::PushClientImpl(client_config));
}

TIGER_API::PushClientImpl::~PushClientImpl()
{
	io_service_.stop();

	if (worker_thread_)
	{
		worker_thread_->join();
	}
}

TIGER_API::PushClientImpl::PushClientImpl(const TIGER_API::ClientConfig& client_config)
{
	socket_ = PushSocket::create_push_socket(&io_service_, client_config);
}

void TIGER_API::PushClientImpl::connect()
{
	LOG(INFO) << "create a worker thread to perform asynchronous network connections";
	// create a worker thread to perform asynchronous network connections
	worker_thread_ = std::shared_ptr<std::thread>(new std::thread([this]
	{
		
		socket_->connect();

		LOG(INFO) << "io_service run on work thread";
		io_service_.run();
	}));
}

void TIGER_API::PushClientImpl::disconnect()
{
	// cross-thread call, need to post a task asynchronouly
	io_service_.post(boost::bind(&PushClientImpl::do_disconnect, this));
}

void TIGER_API::PushClientImpl::set_connected_callback(const std::function<void()>& cb)
{
	socket_->set_connected_callback(cb);
}

void TIGER_API::PushClientImpl::set_disconnected_callback(const std::function<void()>& cb)
{
	socket_->set_disconnected_callback(cb);
}

void TIGER_API::PushClientImpl::set_inner_error_callback(const std::function<void(std::string)>& cb)
{
	if (socket_)
	{
		socket_->set_inner_error_callback(cb);
	}
}

void TIGER_API::PushClientImpl::query_subscribed_symbols()
{
    tigeropen::push::pb::Request request;
    request.set_command(tigeropen::push::pb::SocketCommon_Command_SEND);
    request.set_id(socket_->get_next_id());
    send_frame(request);
}

void TIGER_API::PushClientImpl::set_query_subscribed_symbols_changed_callback(const std::function<void(const std::vector<std::string>& symbols)>& cb)
{
	query_subscribed_symbols_changed_ = cb;
}

void TIGER_API::PushClientImpl::set_asset_changed_callback(const std::function<void(const tigeropen::push::pb::AssetData&)>& cb)
{
	asset_changed_ = cb;
}

bool TIGER_API::PushClientImpl::subscribe_asset(const std::string& account)
{
	return send_trade_request(tigeropen::push::pb::SocketCommon_Command_SUBSCRIBE, tigeropen::push::pb::SocketCommon_DataType::SocketCommon_DataType_Asset, account);
}

bool TIGER_API::PushClientImpl::unsubscribe_asset(const std::string& account)
{
	return send_trade_request(tigeropen::push::pb::SocketCommon_Command_UNSUBSCRIBE, tigeropen::push::pb::SocketCommon_DataType::SocketCommon_DataType_Asset, account);
}

void TIGER_API::PushClientImpl::set_position_changed_callback(const std::function<void(const tigeropen::push::pb::PositionData&)>& cb)
{
	position_changed_ = cb;
}

bool TIGER_API::PushClientImpl::subscribe_position(const std::string& account)
{
	return send_trade_request(tigeropen::push::pb::SocketCommon_Command_SUBSCRIBE, tigeropen::push::pb::SocketCommon_DataType::SocketCommon_DataType_Position, account);
}

bool TIGER_API::PushClientImpl::unsubscribe_position(const std::string& account)
{
	return send_trade_request(tigeropen::push::pb::SocketCommon_Command_UNSUBSCRIBE, tigeropen::push::pb::SocketCommon_DataType::SocketCommon_DataType_Position, account);
}

void TIGER_API::PushClientImpl::set_order_changed_callback(const std::function<void(const tigeropen::push::pb::OrderStatusData&)>& cb)
{
	order_changed_ = cb;
}

bool TIGER_API::PushClientImpl::subscribe_order(const std::string& account)
{
	return send_trade_request(tigeropen::push::pb::SocketCommon_Command_SUBSCRIBE, tigeropen::push::pb::SocketCommon_DataType::SocketCommon_DataType_OrderStatus, account);
}

bool TIGER_API::PushClientImpl::unsubscribe_order(const std::string& account)
{
	return send_trade_request(tigeropen::push::pb::SocketCommon_Command_UNSUBSCRIBE, tigeropen::push::pb::SocketCommon_DataType::SocketCommon_DataType_OrderStatus  , account);
}

void TIGER_API::PushClientImpl::set_quote_changed_callback(const std::function<void(const tigeropen::push::pb::QuoteData&)>& cb)
{
	quote_changed_ = cb;
}

bool TIGER_API::PushClientImpl::subscribe_quote(const std::vector<std::string>& symbols)
{
	return send_quote_request(tigeropen::push::pb::SocketCommon_Command_SUBSCRIBE, tigeropen::push::pb::SocketCommon_DataType::SocketCommon_DataType_Quote, symbols);
}


bool TIGER_API::PushClientImpl::unsubscribe_quote(const std::vector<std::string>& symbols)
{
	return send_quote_request(tigeropen::push::pb::SocketCommon_Command_UNSUBSCRIBE, tigeropen::push::pb::SocketCommon_DataType::SocketCommon_DataType_Quote, symbols);
}

void TIGER_API::PushClientImpl::set_quote_bbo_changed_callback(const std::function<void(const tigeropen::push::pb::QuoteData&)>& cb)
{
	quote_bbo_changed_ = cb;
}

bool TIGER_API::PushClientImpl::subscribe_quote_bbo(const std::vector<std::string>& symbols)
{
	return send_quote_request(tigeropen::push::pb::SocketCommon_Command_SUBSCRIBE, tigeropen::push::pb::SocketCommon_DataType::SocketCommon_DataType_Quote, symbols);
}

bool TIGER_API::PushClientImpl::unsubscribe_quote_bbo(const std::vector<std::string>& symbols)
{
	return send_quote_request(tigeropen::push::pb::SocketCommon_Command_UNSUBSCRIBE, tigeropen::push::pb::SocketCommon_DataType::SocketCommon_DataType_Quote, symbols);
}

void TIGER_API::PushClientImpl::set_quote_depth_changed_callback(const std::function<void(const tigeropen::push::pb::QuoteDepthData&)>& cb)
{
	quote_depth_changed_ = cb;
}

bool TIGER_API::PushClientImpl::subscribe_quote_depth(const std::vector<std::string>& symbols)
{
	return send_quote_request(tigeropen::push::pb::SocketCommon_Command_SUBSCRIBE, tigeropen::push::pb::SocketCommon_DataType::SocketCommon_DataType_QuoteDepth, symbols);
}	

bool TIGER_API::PushClientImpl::unsubscribe_quote_depth(const std::vector<std::string>& symbols)
{
	return send_quote_request(tigeropen::push::pb::SocketCommon_Command_UNSUBSCRIBE, tigeropen::push::pb::SocketCommon_DataType::SocketCommon_DataType_QuoteDepth, symbols);
}

void TIGER_API::PushClientImpl::set_kline_changed_callback(const std::function<void(const tigeropen::push::pb::KlineData&)>& cb)
{
	kline_changed_ = cb;
}

bool TIGER_API::PushClientImpl::subscribe_kline(const std::vector<std::string>& symbols)
{
	return send_quote_request(tigeropen::push::pb::SocketCommon_Command_SUBSCRIBE, tigeropen::push::pb::SocketCommon_DataType::SocketCommon_DataType_Kline, symbols);
}	

bool TIGER_API::PushClientImpl::unsubscribe_kline(const std::vector<std::string>& symbols)
{
	return send_quote_request(tigeropen::push::pb::SocketCommon_Command_UNSUBSCRIBE, tigeropen::push::pb::SocketCommon_DataType::SocketCommon_DataType_Kline, symbols);
}

void TIGER_API::PushClientImpl::set_full_tick_changed_callback(const std::function<void(const tigeropen::push::pb::TickData&)>& cb)
{
	full_tick_changed_ = cb;
}

bool TIGER_API::PushClientImpl::subscribe_full_tick(const std::vector<std::string>& symbols)
{
	return send_quote_request(tigeropen::push::pb::SocketCommon_Command_SUBSCRIBE, tigeropen::push::pb::SocketCommon_DataType::SocketCommon_DataType_TradeTick, symbols);
}		

bool TIGER_API::PushClientImpl::unsubscribe_full_tick(const std::vector<std::string>& symbols)
{
	return send_quote_request(tigeropen::push::pb::SocketCommon_Command_UNSUBSCRIBE, tigeropen::push::pb::SocketCommon_DataType::SocketCommon_DataType_TradeTick, symbols);
}

void TIGER_API::PushClientImpl::set_tick_changed_callback(const std::function<void(const tigeropen::push::pb::TickData&)>& cb)
{
	tick_changed_ = cb;
}

bool TIGER_API::PushClientImpl::subscribe_tick(const std::vector<std::string>& symbols)
{
	return send_quote_request(tigeropen::push::pb::SocketCommon_Command_SUBSCRIBE, tigeropen::push::pb::SocketCommon_DataType::SocketCommon_DataType_TradeTick, symbols);
}

bool TIGER_API::PushClientImpl::unsubscribe_tick(const std::vector<std::string>& symbols)
{
	return send_quote_request(tigeropen::push::pb::SocketCommon_Command_UNSUBSCRIBE, tigeropen::push::pb::SocketCommon_DataType::SocketCommon_DataType_TradeTick, symbols);
}

bool TIGER_API::PushClientImpl::subscribe_market(const std::string& market)
{
	return send_quote_request(tigeropen::push::pb::SocketCommon_Command_SUBSCRIBE, tigeropen::push::pb::SocketCommon_DataType::SocketCommon_DataType_Quote, {}, market);
}

bool TIGER_API::PushClientImpl::unsubscribe_market(const std::string& market)
{
	return send_quote_request(tigeropen::push::pb::SocketCommon_Command_UNSUBSCRIBE, tigeropen::push::pb::SocketCommon_DataType::SocketCommon_DataType_Quote, {}, market);
}

void TIGER_API::PushClientImpl::set_stock_top_changed_callback(const std::function<void(const tigeropen::push::pb::StockTopData&)>& cb)
{
	stock_top_changed_ = cb;
}

bool TIGER_API::PushClientImpl::subscribe_stock_top(const std::string& market)
{
	return send_quote_request(tigeropen::push::pb::SocketCommon_Command_SUBSCRIBE, tigeropen::push::pb::SocketCommon_DataType::SocketCommon_DataType_StockTop, {}, market);
}

bool TIGER_API::PushClientImpl::unsubscribe_stock_top(const std::string& market)
{
	return send_quote_request(tigeropen::push::pb::SocketCommon_Command_UNSUBSCRIBE, tigeropen::push::pb::SocketCommon_DataType::SocketCommon_DataType_StockTop, {}, market);
}

void TIGER_API::PushClientImpl::set_option_top_changed_callback(const std::function<void(const tigeropen::push::pb::OptionTopData&)>& cb)
{
	option_top_changed_ = cb;
}

bool TIGER_API::PushClientImpl::subscribe_option_top(const std::string& market)
{
	return send_quote_request(tigeropen::push::pb::SocketCommon_Command_SUBSCRIBE, tigeropen::push::pb::SocketCommon_DataType::SocketCommon_DataType_OptionTop, {}, market);
}

bool TIGER_API::PushClientImpl::unsubscribe_option_top(const std::string& market)
{
	return send_quote_request(tigeropen::push::pb::SocketCommon_Command_UNSUBSCRIBE, tigeropen::push::pb::SocketCommon_DataType::SocketCommon_DataType_OptionTop, {}, market);
}

bool TIGER_API::PushClientImpl::send_quote_request(tigeropen::push::pb::SocketCommon_Command command, tigeropen::push::pb::SocketCommon_DataType datatype, std::vector<std::string> symbols, const std::string& market)
{
	if (!socket_)
	{
		return false;
	}
	tigeropen::push::pb::Request request;
	request.set_command(command);
	request.set_id(socket_->get_next_id());

	tigeropen::push::pb::Request_Subscribe* subscribe = request.mutable_subscribe();
	subscribe->set_datatype(datatype);
	
	if (!symbols.empty())
	{
		subscribe->set_symbols(boost::join(symbols, ","));
	}
	if (!market.empty())
	{
		subscribe->set_market(market.c_str());
	}

	send_frame(request);
	return true;
}

bool TIGER_API::PushClientImpl::send_quote_request(tigeropen::push::pb::SocketCommon_Command command, tigeropen::push::pb::SocketCommon_DataType datatype, std::vector<std::string> symbols)
{
	return send_quote_request(command, datatype, symbols, "");
}

bool TIGER_API::PushClientImpl::send_trade_request(tigeropen::push::pb::SocketCommon_Command command, tigeropen::push::pb::SocketCommon_DataType datatype, const std::string& account)
{
	if (account.empty())
	{
		return false;
	}
	tigeropen::push::pb::Request request;
	request.set_command(command);
	request.set_id(socket_->get_next_id());

	tigeropen::push::pb::Request_Subscribe* subscribe = request.mutable_subscribe();
	subscribe->set_datatype(datatype);

	if (!account.empty())
	{
		subscribe->set_account(account.c_str());
	}

	send_frame(request);
	return true;
}

bool TIGER_API::PushClientImpl::send_frame(const tigeropen::push::pb::Request& request)
{
	// serialize pb object to string
	std::string packed_frame = request.SerializeAsString();
	if (packed_frame.empty())
	{
		return false;
	}

	std::string packed_frame_json;
	google::protobuf::util::JsonPrintOptions options;
	MessageToJsonString(request, &packed_frame_json, options).ok();

	LOG(DEBUG) << "send frame:" << packed_frame_json;

	// cross-thread call, need to post a task asynchronouly
	io_service_.post(boost::bind(&PushClientImpl::do_write, this, packed_frame));

	return true;
}

void TIGER_API::PushClientImpl::do_write(const std::string& frame)
{
	socket_->send_message(frame);
}

void TIGER_API::PushClientImpl::do_disconnect()
{
	socket_->disconnect();
}

void TIGER_API::PushClientImpl::on_message(const std::shared_ptr<tigeropen::push::pb::Response>& frame)
{
    try {
        if (frame->code() == ResponseType::GET_SUB_SYMBOLS_END) {
            if (query_subscribed_callback_) {
                query_subscribed_callback_(frame->msg());
            }
        }
        else if (frame->code() == ResponseType::GET_SUBSCRIBE_END) {
            if (subscribe_callback_) {
                subscribe_callback_(frame);
            }
        }
        else if (frame->code() == ResponseType::GET_CANCEL_SUBSCRIBE_END) {
            if (unsubscribe_callback_) {
                unsubscribe_callback_(frame);
            }
        }
        else if (frame->code() == ResponseType::ERROR_END) {
            if (error_callback_) {
                error_callback_(frame);
            }
        }
        else {
            const auto& body = frame->body();
            switch (body.datatype()) {
                case SocketCommon::DataType::Quote: {
                    const auto& quote_data = body.quotedata();
                    switch (quote_data.type()) {
                        case SocketCommon::QuoteType::BASIC:
                            if (quote_changed_) {
                                quote_changed_(quote_data);
                            }
                            break;
                        case SocketCommon::QuoteType::BBO:
                            if (quote_bbo_changed_) {
                                quote_bbo_changed_(quote_data);
                            }
                            break;
                        case SocketCommon::QuoteType::ALL:
                            if (quote_changed_) {
                                auto basic_data = convert_to_basic_data(quote_data);
                                if (basic_data) {
                                    quote_changed_(*basic_data);
                                }
                            }
                            if (quote_bbo_changed_) {
                                auto bbo_data = convert_to_bbo_data(quote_data);
                                if (bbo_data) {
                                    quote_bbo_changed_(*bbo_data);
                                }
                            }
                            break;
                    }
                    break;
                }
                case SocketCommon::DataType::Future:
                case SocketCommon::DataType::Option: {
                    if (quote_changed_) {
                        auto basic_data = convert_to_basic_data(body.quotedata());
                        if (basic_data) {
                            quote_changed_(*basic_data);
                        }
                    }
                    if (quote_bbo_changed_) {
                        auto bbo_data = convert_to_bbo_data(body.quotedata());
                        if (bbo_data) {
                            quote_bbo_changed_(*bbo_data);
                        }
                    }
                    break;
                }
                case SocketCommon::DataType::QuoteDepth:
                    if (quote_depth_changed_) {
                        quote_depth_changed_(body.quotedepthdata());
                    }
                    break;
                case SocketCommon::DataType::TradeTick:
                    if (use_full_tick_) {
                        if (full_tick_changed_) {
                            full_tick_changed_(body.tickdata());
                        }
                    }
                    else {
                        if (tick_changed_) {
                            tick_changed_(convert_tick(body.tradetickdata()));
                        }
                    }
                    break;
                case SocketCommon::DataType::OrderStatus:
                    if (order_changed_) {
                        auto order_status_data = body.orderstatusdata();
                        order_status_data.set_status(
                            get_order_status(order_status_data.status(),
                                           order_status_data.filledquantity()).name());
                        order_changed_(order_status_data);
                    }
                    break;
                case SocketCommon::DataType::OrderTransaction:
                    if (transaction_changed_) {
                        transaction_changed_(body.ordertransactiondata());
                    }
                    break;
                case SocketCommon::DataType::Asset:
                    if (asset_changed_) {
                        asset_changed_(body.assetdata());
                    }
                    break;
                case SocketCommon::DataType::Position:
                    if (position_changed_) {
                        position_changed_(body.positiondata());
                    }
                    break;
                case SocketCommon::DataType::StockTop:
                    if (stock_top_changed_) {
                        stock_top_changed_(body.stocktopdata());
                    }
                    break;
                case SocketCommon::DataType::OptionTop:
                    if (option_top_changed_) {
                        option_top_changed_(body.optiontopdata());
                    }
                    break;
                case SocketCommon::DataType::Kline:
                    if (kline_changed_) {
                        kline_changed_(body.klinedata());
                    }
                    break;
                default:
                    LOG(ERROR) << "unhandled frame: " << frame->DebugString();
                    break;
            }
        }
    }
    catch (const std::exception& e) {
        LOG(ERROR) << "error in on_message: " << e.what();
    }
}



tigeropen::push::pb::QuoteBasicData* TIGER_API::PushClientImpl::convert_to_basic_data(const tigeropen::push::pb::QuoteData& quoteData) {
    // Check for valid input
    if (!quoteData.has_type()) {
        return nullptr;
    }

    QuoteType quoteType = quoteData.type();
    if (quoteType != SocketCommon::QuoteType::ALL && 
        quoteType != SocketCommon::QuoteType::BASIC) {
        return nullptr;
    }

    // Create and initialize builder
    // auto* builder = new QuoteBasicData();
    tigeropen::push::pb::QuoteBasicData builder;
    // Set required fields
    builder.set_symbol(quoteData.symbol);
    builder.set_type(SocketCommon::QuoteType::BASIC);
    builder.set_timestamp(quoteData.timestamp);
    builder.set_latest_price(quoteData.latest_price);
    builder.set_latest_time(quoteData.latesttime);
    builder.set_pre_close(quoteData.pre_close);
    builder.set_volume(quoteData.volume);

    // Set optional fields if they exist
    if (quoteData.servertimestamp) {
        builder.set_servertimestamp(quoteData.servertimestamp);
    }
    
    if (quoteData.avgfillprice) {
        builder.set_avgfillprice(quoteData.avgfillprice);
    }
    
    if (quoteData.latestpricetimestamp) {
        builder.set_latestpricetimestamp(quoteData.latestpricetimestamp);
    }
    
    if (quoteData.amount) {
            builder.set_amount(quoteData.amount);
    }
    
    if (quoteData.open) {
        builder.set_open(quoteData.open);
    }
    
    if (quoteData.high) {
        builder.set_high(quoteData.high);
    }
    
    if (quoteData.low) {
        builder.set_low(quoteData.low);
    }
    
    if (quoteData.hour_trading_tag) {
            builder.set_hour_trading_tag(quoteData.hour_trading_tag);
    }
    
    if (quoteData.market_status) {
        builder.set_market_status(quoteData.market_status);
    }
    
    if (quoteData.identifier) {
        builder.set_identifier(quoteData.identifier);
    }
    
    if (quoteData.open_int) {
        builder.set_open_int(quoteData.open_int);
    }
    
    if (quoteData.trade_time) {
        builder.set_trade_time(quoteData.trade_time);
    }
    
    if (quoteData.presettlement) {
        builder.set_pre_settlement(quoteData.presettlement);
    }
    
    if (quoteData.min_tick) {
        builder.set_min_tick(quoteData.min_tick);
    }
    
    if (quoteData.mi) {
        builder.mutable_mi()->CopyFrom(quoteData.mi);
    }

    return builder;
}

tigeropen::push::pb::QuoteBboData* TIGER_API::PushClientImpl::convert_to_bbo_data(const tigeropen::push::pb::QuoteData& quoteData) {
    if (!quoteData.has_type()) {
        return nullptr;
    }

    QuoteType quoteType = quoteData.type();
    if (quoteType != SocketCommon::QuoteType::BBO) {
        return nullptr;
    }

    // Create and initialize builder
    tigeropen::push::pb::QuoteBboData builder;
    builder.set_symbol(quoteData.symbol);
    builder.set_type(SocketCommon::QuoteType::BBO);
    builder.set_timestamp(quoteData.timestamp);
    builder.set_latestprice(quoteData.latest_price);
    builder.set_latesttime(quoteData.latesttime);
    builder.set_bidprice(quoteData.bid_price);
    builder.set_bidsize(quoteData.bid_size);
    builder.set_askprice(quoteData.ask_price);
    builder.set_asksize(quoteData.ask_size);
    if (quoteData.asktimestamp) {
        builder.set_asktimestamp(quoteData.asktimestamp);
    }
    if (quoteData.bidtimestamp) {
        builder.set_bidtimestamp(quoteData.bidtimestamp);
    }
    return builder;
}


tigeropen::push::pb::TradeTick* TIGER_API::PushClientImpl::convert_tick(const tigeropen::push::pb::TradeTickData& data) {
    // Get basic data
    std::string symbol = data.symbol;
    double price_offset = std::pow(10, data.priceoffset);
    double price_base = data.pricebase;
    int64_t timestamp = data.timestamp;
    int sec_type = data.sectype;
    int quote_level = data.quotelevel;
    int64_t sn = data.sn;

    // Calculate size for vectors
    size_t data_size = data.time_size;  // Assuming all arrays are same size
    
    // Process all parallel arrays
    std::vector<double> prices;
    std::vector<int64_t> times;
    std::vector<int64_t> volumes;
    std::vector<int> tick_types;
    std::vector<std::string> part_codes;
    std::vector<std::string> part_code_names;
    std::vector<std::string> conditions;
    std::vector<int64_t> sn_list;
    
    // Process prices
    prices.reserve(data_size);
    for (const auto& price : data.price()) {
        prices.push_back((static_cast<double>(price) + price_base) / price_offset);
    }

    // Process times (accumulate)
    times.reserve(data_size);
    int64_t time_accum = 0;
    for (const auto& time : data.time()) {
        time_accum += time;
        times.push_back(time_accum);
    }

    // Process volumes
    volumes.reserve(data_size);
    for (const auto& vol : data.volume()) {
        volumes.push_back(static_cast<int64_t>(vol));
    }

    // Process tick types
    tick_types.reserve(data_size);
    if (data.type_size() > 0) {
        for (const auto& type : data.type()) {
            tick_types.push_back(type);
        }
    } else {
        tick_types.resize(data_size, 0);  // Default value
    }

    // Process part codes
    part_codes.reserve(data_size);
    part_code_names.reserve(data_size);
    if (data.part_code_size() > 0) {
        for (const auto& code : data.part_code()) {
            part_codes.push_back(get_part_code(code));
            part_code_names.push_back(get_part_code_name(code));
        }
    } else {
        part_codes.resize(data_size);
        part_code_names.resize(data_size);
    }

    // Process conditions
    conditions.reserve(data_size);
    auto cond_map = get_trade_condition_map(quote_level);
    if (data.cond_size() > 0) {
        for (const auto& cond : data.cond()) {
            conditions.push_back(get_trade_condition(cond, cond_map));
        }
    } else {
        conditions.resize(data_size);
    }

    // Generate sequential SN list
    sn_list.reserve(data_size);
    for (size_t i = 0; i < data_size; ++i) {
        sn_list.push_back(sn + i);
    }

    // Create result container
    tigeropen::push::pb::TradeTick result;
    result.set_symbol(symbol);
    result.set_sectype(sec_type);
    result.set_quotelevel(quote_level);
    result.set_timestamp(timestamp);

    // Process all items
    for (size_t i = 0; i < data_size; ++i) {
        if (i < data.merged_vols_size() && data.merged_vols(i).vol_size() > 0) {
            // Handle merged volumes
            const auto& merged_vol = data.merged_vols(i);
            for (int j = 0; j < merged_vol.vol_size(); ++j) {
                auto* tick = result.add_ticks();
                tick.set_ticktype(tick_types[i]);
                tick.set_price(prices[i]);
                tick.set_volume(merged_vol.vol(j));
                tick.set_partcode(part_codes[i]);
                tick.set_partcodename(part_code_names[i]);
                tick.set_cond(conditions[i]);
                tick.set_time(times[i]);
                tick.set_sn(sn_list[i] * 10 + j);
            }
        } else {
            // Handle regular tick
            auto* tick = result.add_ticks();
            tick.set_ticktype(tick_types[i]);
            tick.set_price(prices[i]);
            tick.set_volume(volumes[i]);
            tick.set_partcode(part_codes[i]);
            tick.set_partcodename(part_code_names[i]);
            tick.set_cond(conditions[i]);
            tick.set_time(times[i]);
            tick.set_sn(sn_list[i]);
        }
    }

    return result;
}
