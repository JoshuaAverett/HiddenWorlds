#pragma once

#include "HWIPEndPoint.h"
#include "HWPacket.h"
#include <map>
#include <vector>

typedef void (*HWPacketEventHandler)(const HWPacket* packet);

class HWCircuit
{
public:
	friend class HWNetworkManager;

	HWCircuit(const HWIPEndPoint& ep);
	HWCircuit(const HWCircuit& copy);
	~HWCircuit();

	void Send(const HWPacket& packet);
	void Recieve(const HWUDPPacket* packet);
	void Register(HWPacketType type, HWPacketEventHandler handler);

private:
	class HWPacketAssembler
	{
	public:
		HWPacketAssembler();
		HWPacketAssembler(const HWPacketAssembler&);
		~HWPacketAssembler();

		HWPacket* AddPart(const HWUDPPacket* part);

	private:
		std::vector<const HWUDPPacket*> parts;
		U64 sequence_length;
	};

	HWIPEndPoint end_point;
	bool connected;
	std::vector<const HWPacket*> outbox;
	std::map<U64, HWPacketAssembler*> inbox;
	HWPacketEventHandler handlers[HWPacketType::PACKET_COUNT];
};
