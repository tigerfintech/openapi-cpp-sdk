#ifndef PUSH_FRAME_SERIALIZE_H
#define PUSH_FRAME_SERIALIZE_H
#include <vector>
#include <string>

namespace TIGER_API
{
	class PushFrameEncoder
	{
	public:
		static std::vector<unsigned char> encode_frame(const std::string& packed_frame);
	};

	class PushFrameDecoder 
	{
	public:
		PushFrameDecoder() = default;
		~PushFrameDecoder() = default;

		bool push_byte(unsigned char value);
		uint32_t get_frame_size() const;

	private:
		uint32_t decode_varint();
		std::vector<unsigned char> frame_header_buffer_;
		uint32_t frame_len_ = 0;
	};

}
#endif //PUSH_FRAME_SERIALIZE_H
