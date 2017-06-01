#pragma once

#include "HWPacket.h"
#include "HWCurve25519.h"

class HWPacketCreateCircuit : public HWPacket
{
public:
	HWPacketCreateCircuit()
	{
		packet_type = HWPacketType::CreateCircuit;
		flags = (U8)HWPacketFlag::Reliable;
	}

	void Serialize(HWSerializer& stream) const
	{
		HWPacket::SerializeHeader(stream);
		stream.Write(encryption_key, HWCurve25519::KeyLength);
	}

	void Deserialize(HWSerializer& stream)
	{
		HWPacket::DeserializeHeader(stream);
		encryption_key
	}

private:
	U8 encryption_key[HWCurve25519::KeyLength];
};
