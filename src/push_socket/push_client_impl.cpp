#include "../include/tigerapi/push_socket/push_client_impl.h"
#include "../include/tigerapi/client_config.h"
#include "../include/tigerapi/push_socket/push_socket.h"
#include "../include/tigerapi/tick_util.h"
#include "../include/openapi_pb/pb_source/PushData.pb.h"
#include "../include/tigerapi/enums.h"
#include "../include/openapi_pb/pb_source/SocketCommon.pb.h"

#include "google/protobuf/util/json_util.h"
#include <vector>
#include <memory>
#include <cmath>
#include <numeric>
#include <boost/algorithm/string.hpp>

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
    client_config_ = client_config;
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

void TIGER_API::PushClientImpl::set_subscribe_callback(const std::function<void(const tigeropen::push::pb::Response&)>& cb)
{
	subscribe_callback_ = cb;
}

void TIGER_API::PushClientImpl::set_unsubscribe_callback(const std::function<void(const tigeropen::push::pb::Response&)>& cb)
{
	unsubscribe_callback_ = cb;
}

void TIGER_API::PushClientImpl::set_error_callback(const std::function<void(const tigeropen::push::pb::Response&)>& cb)
{
	error_callback_ = cb;
}

void TIGER_API::PushClientImpl::query_subscribed_symbols()
{
    tigeropen::push::pb::Request request;
    request.set_command(tigeropen::push::pb::SocketCommon_Command_SEND);
    request.set_id(socket_->get_next_id());
    send_frame(request);
}

void TIGER_API::PushClientImpl::set_query_subscribed_symbols_changed_callback(const std::function<void(const tigeropen::push::pb::Response& query_subscribed_symbols_response)>& cb)
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

void TIGER_API::PushClientImpl::set_transaction_changed_callback(const std::function<void(const tigeropen::push::pb::OrderTransactionData&)>& cb)
{
	transaction_changed_ = cb;
}

bool TIGER_API::PushClientImpl::subscribe_transaction(const std::string& account)
{
	return send_trade_request(tigeropen::push::pb::SocketCommon_Command_SUBSCRIBE, tigeropen::push::pb::SocketCommon_DataType::SocketCommon_DataType_OrderTransaction, account);
}

bool TIGER_API::PushClientImpl::unsubscribe_transaction(const std::string& account)
{
	return send_trade_request(tigeropen::push::pb::SocketCommon_Command_UNSUBSCRIBE, tigeropen::push::pb::SocketCommon_DataType::SocketCommon_DataType_OrderTransaction, account);
}

void TIGER_API::PushClientImpl::set_quote_changed_callback(const std::function<void(const tigeropen::push::pb::QuoteBasicData&)>& cb)
{
	quote_changed_ = cb;
}

bool TIGER_API::PushClientImpl::subscribe_quote(const std::vector<std::string>& symbols)
{
	return send_quote_request(tigeropen::push::pb::SocketCommon_Command_SUBSCRIBE, tigeropen::push::pb::SocketCommon_DataType::SocketCommon_DataType_Quote, symbols, "");
}

bool TIGER_API::PushClientImpl::subscribe_future_quote(const std::vector<std::string>& symbols)
{
	return send_quote_request(tigeropen::push::pb::SocketCommon_Command_SUBSCRIBE, tigeropen::push::pb::SocketCommon_DataType::SocketCommon_DataType_Future, symbols, "");
}

bool TIGER_API::PushClientImpl::subscribe_option_quote(const std::vector<std::string>& symbols)
{
	return send_quote_request(tigeropen::push::pb::SocketCommon_Command_SUBSCRIBE, tigeropen::push::pb::SocketCommon_DataType::SocketCommon_DataType_Option, symbols, "");
}


bool TIGER_API::PushClientImpl::unsubscribe_quote(const std::vector<std::string>& symbols)
{
	return send_quote_request(tigeropen::push::pb::SocketCommon_Command_UNSUBSCRIBE, tigeropen::push::pb::SocketCommon_DataType::SocketCommon_DataType_Quote, symbols, "");
}

void TIGER_API::PushClientImpl::set_quote_bbo_changed_callback(const std::function<void(const tigeropen::push::pb::QuoteBBOData&)>& cb)
{
	quote_bbo_changed_ = cb;
}

bool TIGER_API::PushClientImpl::subscribe_quote_bbo(const std::vector<std::string>& symbols)
{
	return send_quote_request(tigeropen::push::pb::SocketCommon_Command_SUBSCRIBE, tigeropen::push::pb::SocketCommon_DataType::SocketCommon_DataType_Quote, symbols, "");
}

bool TIGER_API::PushClientImpl::unsubscribe_quote_bbo(const std::vector<std::string>& symbols)
{
	return send_quote_request(tigeropen::push::pb::SocketCommon_Command_UNSUBSCRIBE, tigeropen::push::pb::SocketCommon_DataType::SocketCommon_DataType_Quote, symbols, "");
}

void TIGER_API::PushClientImpl::set_quote_depth_changed_callback(const std::function<void(const tigeropen::push::pb::QuoteDepthData&)>& cb)
{
	quote_depth_changed_ = cb;
}

bool TIGER_API::PushClientImpl::subscribe_quote_depth(const std::vector<std::string>& symbols)
{
	return send_quote_request(tigeropen::push::pb::SocketCommon_Command_SUBSCRIBE, tigeropen::push::pb::SocketCommon_DataType::SocketCommon_DataType_QuoteDepth, symbols, "");
}	

bool TIGER_API::PushClientImpl::unsubscribe_quote_depth(const std::vector<std::string>& symbols)
{
	return send_quote_request(tigeropen::push::pb::SocketCommon_Command_UNSUBSCRIBE, tigeropen::push::pb::SocketCommon_DataType::SocketCommon_DataType_QuoteDepth, symbols, "");
}

void TIGER_API::PushClientImpl::set_kline_changed_callback(const std::function<void(const tigeropen::push::pb::KlineData&)>& cb)
{
	kline_changed_ = cb;
}

bool TIGER_API::PushClientImpl::subscribe_kline(const std::vector<std::string>& symbols)
{
	return send_quote_request(tigeropen::push::pb::SocketCommon_Command_SUBSCRIBE, tigeropen::push::pb::SocketCommon_DataType::SocketCommon_DataType_Kline, symbols, "");
}	

bool TIGER_API::PushClientImpl::unsubscribe_kline(const std::vector<std::string>& symbols)
{
	return send_quote_request(tigeropen::push::pb::SocketCommon_Command_UNSUBSCRIBE, tigeropen::push::pb::SocketCommon_DataType::SocketCommon_DataType_Kline, symbols, "");
}

void TIGER_API::PushClientImpl::set_full_tick_changed_callback(const std::function<void(const tigeropen::push::pb::TickData&)>& cb)
{
	full_tick_changed_ = cb;
}

bool TIGER_API::PushClientImpl::subscribe_full_tick(const std::vector<std::string>& symbols)
{
	return send_quote_request(tigeropen::push::pb::SocketCommon_Command_SUBSCRIBE, tigeropen::push::pb::SocketCommon_DataType::SocketCommon_DataType_TradeTick, symbols, "");
}		

bool TIGER_API::PushClientImpl::unsubscribe_full_tick(const std::vector<std::string>& symbols)
{
	return send_quote_request(tigeropen::push::pb::SocketCommon_Command_UNSUBSCRIBE, tigeropen::push::pb::SocketCommon_DataType::SocketCommon_DataType_TradeTick, symbols, "");
}

void TIGER_API::PushClientImpl::set_tick_changed_callback(const std::function<void(const TradeTick&)>& cb)
{
	tick_changed_ = cb;
}

bool TIGER_API::PushClientImpl::subscribe_tick(const std::vector<std::string>& symbols)
{
	return send_quote_request(tigeropen::push::pb::SocketCommon_Command_SUBSCRIBE, tigeropen::push::pb::SocketCommon_DataType::SocketCommon_DataType_TradeTick, symbols, "");
}

bool TIGER_API::PushClientImpl::unsubscribe_tick(const std::vector<std::string>& symbols)
{
	return send_quote_request(tigeropen::push::pb::SocketCommon_Command_UNSUBSCRIBE, tigeropen::push::pb::SocketCommon_DataType::SocketCommon_DataType_TradeTick, symbols, "");
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
        if (frame->code() == static_cast<int>(ResponseType::GET_SUB_SYMBOLS_END)) {
            if (query_subscribed_symbols_changed_) {
                query_subscribed_symbols_changed_(*frame);
            }
        }
        else if (frame->code() == static_cast<int>(ResponseType::GET_SUBSCRIBE_END)) {
            if (subscribe_callback_) {
                subscribe_callback_(*frame);
            }
        }
        else if (frame->code() == static_cast<int>(ResponseType::GET_CANCEL_SUBSCRIBE_END)) {
            if (unsubscribe_callback_) {
                unsubscribe_callback_(*frame);
            }
        }
        else if (frame->code() == static_cast<int>(ResponseType::ERROR_END)) {
            if (error_callback_) {
                error_callback_(*frame);
            }
        }
        else {
            const auto& body = frame->body();
            switch (body.datatype()) {
                case tigeropen::push::pb::SocketCommon_DataType_Quote: {
                    const auto& quote_data = body.quotedata();
                    switch (quote_data.type()) {
                        case tigeropen::push::pb::SocketCommon_QuoteType_BASIC:
                            if (quote_changed_) {
                                quote_changed_(*convert_to_basic_data(quote_data));
                            }
                            break;
                        case tigeropen::push::pb::SocketCommon_QuoteType_BBO:
                            if (quote_bbo_changed_) {
                                quote_bbo_changed_(*convert_to_bbo_data(quote_data));
                            }
                            break;
                        case tigeropen::push::pb::SocketCommon_QuoteType_ALL:
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
                case tigeropen::push::pb::SocketCommon_DataType_Future:
                case tigeropen::push::pb::SocketCommon_DataType_Option: {
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
                case tigeropen::push::pb::SocketCommon_DataType_QuoteDepth:
                    if (quote_depth_changed_) {
                        quote_depth_changed_(body.quotedepthdata());
                    }
                    break;
                case tigeropen::push::pb::SocketCommon_DataType_TradeTick:
                    if (client_config_.use_full_tick) {
                        if (full_tick_changed_) {
                            full_tick_changed_(body.tickdata());
                        }
                    }
                    else {
                        if (tick_changed_) {
                            tick_changed_(*convert_tick(body.tradetickdata()));
                        }
                    }
                    break;
                case tigeropen::push::pb::SocketCommon_DataType_OrderStatus:
                    if (order_changed_) {
                        auto order_status_data = body.orderstatusdata();
                        // order_status_data.set_status(
                        //     get_order_status(order_status_data.status(),
                        //                    order_status_data.filledquantity()).name());
                        order_changed_(order_status_data);
                    }
                    break;
                case tigeropen::push::pb::SocketCommon_DataType_OrderTransaction:
                    if (transaction_changed_) {
                        transaction_changed_(body.ordertransactiondata());
                    }
                    break;
                case tigeropen::push::pb::SocketCommon_DataType_Asset:
                    if (asset_changed_) {
                        asset_changed_(body.assetdata());
                    }
                    break;
                case tigeropen::push::pb::SocketCommon_DataType_Position:
                    if (position_changed_) {
                        position_changed_(body.positiondata());
                    }
                    break;
                case tigeropen::push::pb::SocketCommon_DataType_StockTop:
                    if (stock_top_changed_) {
                        stock_top_changed_(body.stocktopdata());
                    }
                    break;
                case tigeropen::push::pb::SocketCommon_DataType_OptionTop:
                    if (option_top_changed_) {
                        option_top_changed_(body.optiontopdata());
                    }
                    break;
                case tigeropen::push::pb::SocketCommon_DataType_Kline:
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



std::shared_ptr<tigeropen::push::pb::QuoteBasicData> TIGER_API::PushClientImpl::convert_to_basic_data(const tigeropen::push::pb::QuoteData& quote_data) {
    // Check for valid input
    if (!quote_data.type()) {
        return nullptr;
    }

    tigeropen::push::pb::SocketCommon_QuoteType quoteType = quote_data.type();
    if (quoteType != tigeropen::push::pb::SocketCommon::QuoteType::SocketCommon_QuoteType_ALL &&
        quoteType != tigeropen::push::pb::SocketCommon::QuoteType::SocketCommon_QuoteType_BASIC) {
        return nullptr;
    }

    // Create and initialize builder
    // auto* builder = new QuoteBasicData();
    auto builder = std::make_shared<tigeropen::push::pb::QuoteBasicData>();
    // Set required fields
    builder->set_symbol(quote_data.symbol());
    builder->set_type(tigeropen::push::pb::SocketCommon::QuoteType::SocketCommon_QuoteType_BASIC);
    builder->set_timestamp(quote_data.timestamp());
    builder->set_latestprice(quote_data.latestprice());
    builder->set_latesttime(quote_data.latesttime());
    builder->set_preclose(quote_data.preclose());
    builder->set_volume(quote_data.volume());

    // Set optional fields if they exist
    if (quote_data.servertimestamp()) {
        builder->set_servertimestamp(quote_data.servertimestamp());
    }
    
    if (quote_data.avgprice() != 0) {
        builder->set_avgprice(quote_data.avgprice());
    }
    
    if (quote_data.latestpricetimestamp()) {
        builder->set_latestpricetimestamp(quote_data.latestpricetimestamp());
    }
    
    if (quote_data.amount() != 0) {
            builder->set_amount(quote_data.amount());
    }
    
    if (quote_data.open() != 0) {
        builder->set_open(quote_data.open());
    }
    
    if (quote_data.high() != 0) {
        builder->set_high(quote_data.high());
    }
    
    if (quote_data.low() != 0) {
        builder->set_low(quote_data.low());
    }
    
    if (!quote_data.hourtradingtag().empty()) {
            builder->set_hourtradingtag(quote_data.hourtradingtag());
    }
    
    if (!quote_data.marketstatus().empty()) {
        builder->set_marketstatus(quote_data.marketstatus());
    }
    
    if (!quote_data.identifier().empty()) {
        builder->set_identifier(quote_data.identifier());
    }
    
    if (quote_data.openint() != 0) {
        builder->set_openint(quote_data.openint());
    }
    
    if (quote_data.tradetime()) {
        builder->set_tradetime(quote_data.tradetime());
    }
    
    if (quote_data.presettlement() != 0) {
        builder->set_presettlement(quote_data.presettlement());
    }
    
    if (quote_data.mintick() != 0) {
        builder->set_mintick(quote_data.mintick());
    }
    
    builder->mutable_mi()->CopyFrom(quote_data.mi());


    return builder;
}

std::shared_ptr<tigeropen::push::pb::QuoteBBOData> TIGER_API::PushClientImpl::convert_to_bbo_data(const tigeropen::push::pb::QuoteData& quote_data) {
    if (!quote_data.type()) {
        return nullptr;
    }

    tigeropen::push::pb::SocketCommon_QuoteType quote_type = quote_data.type();
    if (quote_type != tigeropen::push::pb::SocketCommon::QuoteType::SocketCommon_QuoteType_BBO) {
        return nullptr;
    }

    // Create and initialize builder
    auto builder = std::make_shared<tigeropen::push::pb::QuoteBBOData>();
    builder->set_symbol(quote_data.symbol());
    builder->set_type(tigeropen::push::pb::SocketCommon::QuoteType::SocketCommon_QuoteType_BBO);
    builder->set_timestamp(quote_data.timestamp());
    builder->set_bidprice(quote_data.bidprice());
    builder->set_bidsize(quote_data.bidsize());
    builder->set_askprice(quote_data.askprice());
    builder->set_asksize(quote_data.asksize());
    builder->set_asktimestamp(quote_data.asktimestamp());
    builder->set_bidtimestamp(quote_data.bidtimestamp());
    return builder;
}


shared_ptr<TIGER_API::TradeTick> TIGER_API::PushClientImpl::convert_tick(const tigeropen::push::pb::TradeTickData& data) {
    // Get basic data
    std::string symbol = data.symbol();
    double price_offset = std::pow(10, data.priceoffset());
    double price_base = data.pricebase();
    int64_t timestamp = data.timestamp();
    std::string sec_type = data.sectype();
    std::string quote_level = data.quotelevel();
    int64_t sn = data.sn();

    // Calculate size for vectors
    size_t data_size = data.time_size();  // Assuming all arrays are same size
    
    // Process all parallel arrays
    std::vector<double> prices;
    std::vector<int64_t> times;
    std::vector<long> volumes;
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
    if (data.type().size() > 0) {
        for (const auto& type : data.type()) {
            tick_types.push_back(type);
        }
    } else {
        tick_types.resize(data_size, 0);  // Default value
    }

    // Process part codes
    part_codes.reserve(data_size);
    part_code_names.reserve(data_size);
    if (data.partcode_size() > 0) {
        for (const auto& code : data.partcode()) {
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
    if (data.cond().size() > 0) {
        for (const auto& cond : data.cond()) {
            conditions.push_back(get_trade_condition(std::string(1, cond), cond_map));
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
    auto result = std::make_shared<TIGER_API::TradeTick>();
    result->set_symbol(symbol);
    result->set_sectype(sec_type);
    result->set_quotelevel(quote_level);
    result->set_timestamp(std::to_string(timestamp));

    // Process all items
    for (size_t i = 0; i < data_size; ++i) {
        if (i < data.mergedvols_size() && data.mergedvols(i).vol_size() > 0) {
            // Handle merged volumes
            const auto& merged_vol = data.mergedvols(i);
            for (int j = 0; j < merged_vol.vol_size(); ++j) {
                TIGER_API::TradeTickItem tick;
                tick.set_ticktype(std::to_string(tick_types[i]));
                tick.set_price(prices[i]);
                tick.set_volume(merged_vol.vol(j));
                tick.set_partcode(part_codes[i]);
                tick.set_partcodename(part_code_names[i]);
                tick.set_cond(conditions[i]);
                tick.set_time(std::to_string(times[i]));
                tick.set_sn(std::to_string(sn_list[i] * 10 + j));
                result->addTick(tick);
            }
        } else {
            // Handle regular tick
            TIGER_API::TradeTickItem tick;
            tick.set_ticktype(std::to_string(tick_types[i]));
            tick.set_price(prices[i]);
            tick.set_volume(volumes[i]);
            tick.set_partcode(part_codes[i]);
            tick.set_partcodename(part_code_names[i]);
            tick.set_cond(conditions[i]);
            tick.set_time(std::to_string(times[i]));
            tick.set_sn(std::to_string(sn_list[i]));
            result->addTick(tick);
        }
    }

    return result;
}
