#include "HWCircuit.h"
#include <algorithm>

#include "HWPacketTest.h"

HWCircuit::HWCircuit(const HWIPEndPoint& ep)
{
	end_point = ep;
	for(S32 i = 0; i < (S32)HWPacketType::PACKET_COUNT; ++i)
	{
		handlers[i] = nullptr;
	}
}

HWCircuit::~HWCircuit()
{
	std::vector<const HWPacket*>::iterator iter = outbox.begin();
	for(; iter != outbox.end(); ++iter)
	{
		delete *iter;
	}

	std::map<U64, HWPacketAssembler*>::iterator iter2 = inbox.begin();
	for(; iter2 != inbox.end(); ++iter2)
	{
		delete (*iter2).second;
	}
}

void HWCircuit::Send(const HWPacket& packet)
{
	outbox.push_back(&packet);
}

void HWCircuit::Recieve(const HWUDPPacket* packet)
{
	HWPacketAssembler* reassembler = inbox[packet->GetPacketID()];
	if(!reassembler)
	{
		reassembler = new HWPacketAssembler();
		inbox[packet->GetPacketID()] = reassembler;
	}
	HWPacket* assembled = reassembler->AddPart(packet);
	if(assembled)
	{
		HWPacketEventHandler handler = handlers[(U32)assembled->GetPacketType()];
		if(handler)
		{
			handler(assembled);
		}
		else
		{
			printf("Packet without handler recieved\n");
		}
		delete reassembler;
	}
}

HWCircuit::HWPacketAssembler::HWPacketAssembler()
{
	sequence_length = 0;
}

HWCircuit::HWPacketAssembler::~HWPacketAssembler()
{
	std::vector<const HWUDPPacket*>::iterator iter = parts.begin();
	for(; iter != parts.end(); ++iter)
	{
		delete *iter;
	}
}

HWPacket* HWCircuit::HWPacketAssembler::AddPart(const HWUDPPacket* part)
{
	parts.push_back(part);
	if(sequence_length == 0)
	{
		if(part->GetSequenceNumber() == 0)
		{
			sequence_length = part->GetSequenceSize();
		}
	}

	if(parts.size() == sequence_length)
	{
		HWSerializer stream = HWSerializer();
		std::vector<const HWUDPPacket*>::iterator iter = parts.begin();
		for(; iter != parts.end(); ++iter)
		{
			const HWUDPPacket* part = *iter;
			stream.Write(part->GetBody(), part->GetBodyLength());
		}

		HWPacketType type = (HWPacketType)stream.ReadVarInt();

		HWPacket* ret = nullptr;

		switch(type)
		{
		case HWPacketType::Test:
			ret = new HWPacketTest();
			break;
		}

		if(ret) ret->Deserialize(stream);
		return ret;
	}

	return nullptr;
}