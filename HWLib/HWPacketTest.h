#pragma once

#include "HWPacket.h"
#include <string>

class HWPacketTest : public HWPacket
{
public:
	HWPacketTest()
	{
		packet_type = HWPacketType::Test;
		flags = 0;
	}

	HWPacketTest(const std::string& string)
	{
		packet_type = HWPacketType::Test;
		flags = 0;
		SetData(string);
	}

	~HWPacketTest()
	{
	}

	void Serialize(HWSerializer& stream) const
	{
		HWPacket::SerializeHeader(stream);
		stream.Write(data);
	}

	void Deserialize(HWSerializer& stream)
	{
		data = stream.ReadString();
	}

	void SetData(const std::string& string)
	{
		data = string;
	}

	std::string GetData()
	{
		return data;
	}

private:
	std::string data;
};
