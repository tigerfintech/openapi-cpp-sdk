#include "tigerapi/push_socket/push_frame_serialize.h"

std::vector<unsigned char> TIGER_API::PushFrameEncoder::encode_frame(const std::string& packed_frame)
{
	if (packed_frame.empty())
	{
		return std::vector<unsigned char>();
	}

	size_t pack_size = packed_frame.size();
	std::vector<unsigned char> pack_array;

	unsigned char bits = pack_size & 0x7F;
	pack_size >>= 7;
	while (pack_size) 
	{
		unsigned char header_byte = 0x80 | bits;
		pack_array.push_back(header_byte);
		bits = pack_size & 0x7F;
		pack_size >>= 7;
	}

	pack_array.push_back(bits);
	pack_array.insert(pack_array.end(), packed_frame.begin(), packed_frame.end());
	return pack_array;
}

bool TIGER_API::PushFrameDecoder::push_byte(unsigned char value)
{
	//逐个字节压入缓冲区
	frame_header_buffer_.push_back(value);
	if (!(value & 0x80))
	{
		//完整读取到包长度
		frame_len_ = decode_varint();
		frame_header_buffer_.clear();
		//frame头部已经读取完毕
		return true;
	}
	//frame头部还有数据，需要继续独取头部数据
	return false;
}

uint32_t TIGER_API::PushFrameDecoder::get_frame_size() const
{
	return frame_len_;
}

uint32_t TIGER_API::PushFrameDecoder::decode_varint()
{
	const uint32_t mask = (1U << 32) - 1;  // 32位掩码
	uint32_t result = 0;
	int shift = 0;

	for (const unsigned char& b : frame_header_buffer_)
	{
		result |= (static_cast<uint32_t>(b & 0x7F) << shift);
		shift += 7;
		if (shift >= 64) 
		{
			throw std::runtime_error("Too many bytes when decoding varint.");
		}
	}

	result &= mask;  // 应用32位掩码
	return result;
}