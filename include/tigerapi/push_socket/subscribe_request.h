#ifndef TIGERAPI_PUSH_SUBSCRIBE_REQUEST_H
#define TIGERAPI_PUSH_SUBSCRIBE_REQUEST_H

#include <string>
#include "openapi_pb/pb_source/Request.pb.h"
#include "openapi_pb/pb_source/SocketCommon.pb.h"

namespace TIGER_API
{
	// Builds the SUBSCRIBE / UNSUBSCRIBE frame for the account-level subjects
	// (asset / position / order / transaction).
	//
	// Kept free-standing and socket-independent so it can be unit-tested on its own.
	//
	// asset_quote_type is the AssetQuoteType enum name ("ETH" / "RTH" /
	// "OVERNIGHT") -- the server maps the name, so it must not be translated to a
	// code here. An empty string leaves field 5 unset, which is exactly what
	// every call site did before the field existed; the server then values the
	// pushed assets with ETH.
	inline tigeropen::push::pb::Request build_trade_request(
		tigeropen::push::pb::SocketCommon_Command command,
		tigeropen::push::pb::SocketCommon_DataType datatype,
		const std::string& account,
		const std::string& asset_quote_type,
		unsigned int id)
	{
		tigeropen::push::pb::Request request;
		request.set_command(command);
		request.set_id(id);

		tigeropen::push::pb::Request_Subscribe* subscribe = request.mutable_subscribe();
		subscribe->set_datatype(datatype);

		if (!account.empty())
		{
			subscribe->set_account(account.c_str());
		}

		if (!asset_quote_type.empty())
		{
			subscribe->set_assetquotetype(asset_quote_type.c_str());
		}

		return request;
	}
}

#endif // TIGERAPI_PUSH_SUBSCRIBE_REQUEST_H
