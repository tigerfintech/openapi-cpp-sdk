#ifndef PUSH_SOCKET_H
#define PUSH_SOCKET_H

#include <memory>
#include <string>
#include "boost/asio.hpp"
#include "boost/asio/ssl.hpp"
#include "boost/bind.hpp"
#include "boost/pool/pool.hpp"
#include "boost/optional.hpp"
#include "cpprest/details/basic_types.h"
#include "tigerapi/client_config.h"
#include "push_frame_serialize.h"

#include "openapi_pb/pb_source/Request.pb.h"
#include "openapi_pb/pb_source/Response.pb.h"

namespace TIGER_API
{
	enum class SocketState
	{
		CONNECTING,			//正在连接
		CONNECTED,			//已连接
		DISCONNECTED		//已断开
	};
	
	class PushSocket : public std::enable_shared_from_this<PushSocket>
	{
	public:
		static std::shared_ptr<PushSocket> create_push_socket(boost::asio::io_service* io_service,
			const TIGER_API::ClientConfig& client_config);
		
		~PushSocket();
		PushSocket(const PushSocket&) = delete;
		PushSocket& operator=(const PushSocket&) = delete;

	private:
		PushSocket() = delete;
		PushSocket(boost::asio::io_service* io_service,
			const TIGER_API::ClientConfig& client_config);
	public:
		void set_connected_callback(const std::function<void()>& cb);
		void set_disconnected_callback(const std::function<void()>& cb);
		void set_on_message_callback(const std::function<void(const std::shared_ptr<tigeropen::push::pb::Response>& response_pb_object)>& cb);
		void set_inner_error_callback(const std::function<void(std::string)>& cb);

		void connect();
		void disconnect();
		bool send_message(const std::string& msg);
		uint32_t get_next_id();
	private:
		void init_socket();
		bool verify_certificate(bool preverified,
			boost::asio::ssl::verify_context& ctx);
		void close_session();
		void send_authentication();
		void start_keep_alive();
		void send_heart_beat();
		void auto_reconnect();

		void handle_connect(const boost::system::error_code& error, 
			boost::asio::ip::tcp::resolver::iterator endpoint_iterator);
		void handle_handshake(const boost::system::error_code& error);
		void handle_write(const boost::system::error_code& error,
			size_t bytes_transferred,
			unsigned int frame_len);
		void handle_read_head(const boost::system::error_code& error,
			size_t bytes_transferred);
		void handle_read_body(const boost::system::error_code& error,
			size_t bytes_transferred,
			char* recv_buff,
			int page_num,
			unsigned int frame_len);
		void handle_timer(const boost::system::error_code& error);

		void read_head();
		void read_body(size_t frame_len);

		void dispatch_connected_callback();
		void dispatch_disconnected_callback();
		void dispatch_inner_error_callback(const std::string& error);

		void message_filter(const std::shared_ptr<tigeropen::push::pb::Response>& response_pb_object);
	private:
		std::function<void()> connected_callback_;
		std::function<void()> disconnected_callback_;
		std::function<void(const std::shared_ptr<tigeropen::push::pb::Response>& response_pb_object)> on_message_callback_;
		std::function<void(std::string)> on_inner_error_callback_;
	private:
		TIGER_API::ClientConfig client_config_;
		
		boost::asio::io_service* io_service_ = nullptr;
		boost::optional<boost::asio::ssl::stream<boost::asio::ip::tcp::socket>> socket_;
		std::shared_ptr<boost::asio::deadline_timer> keep_alive_timer_;
		std::shared_ptr<boost::asio::deadline_timer> reconnect_timer_;
		std::atomic<SocketState> socket_state_ = SocketState::DISCONNECTED;

		std::atomic<uint32_t> id_counter_ = 0;

		char head_buff_[1024];
		boost::shared_ptr<boost::pool<>> recv_buff_pool_;
		TIGER_API::PushFrameDecoder frame_decoder_;
	private:
		std::time_t last_send_heart_beat_time_ = 0;
		std::time_t last_io_time_ = 0;
		int reconnect_interval_ = 10 * 1000;	//单位：ms
		int send_interval_ = 10 * 1000;			//单位：ms
		int recv_interval_ = 10 * 1000;			//单位：ms
	};
}
#endif // PUSH_SOCKET_H