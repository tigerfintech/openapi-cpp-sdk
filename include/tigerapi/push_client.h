#ifndef PUSH_CLIENT_H
#define PUSH_CLIENT_H
#include <memory>
#include <functional>
#include "tigerapi/win32.h"
#include "openapi_pb/pb_source/AssetData.pb.h"

namespace TIGER_API
{
	class ClientConfig;
	class OPENAPI_EXPORT IPushClient
	{
	public:
		static std::shared_ptr<IPushClient> create_push_client(const TIGER_API::ClientConfig& client_config);
		virtual void connect() = 0;
		virtual void disconnect() = 0;

		virtual void set_connected_callback(const std::function<void()>& cb) = 0;
		virtual void set_disconnected_callback(const std::function<void()>& cb) = 0;
		virtual void set_inner_error_callback(const std::function<void(std::string)>& cb) = 0;
		
		virtual void set_asset_changed_callback(const std::function<void(const tigeropen::push::pb::AssetData&)>& cb) = 0;
		virtual bool subscribe_asset(const std::string& account) = 0;
		virtual bool unsubscribe_asset(const std::string& account) = 0;
		
		//TODO:other
	};
}

#endif // PUSH_CLIENT_H
