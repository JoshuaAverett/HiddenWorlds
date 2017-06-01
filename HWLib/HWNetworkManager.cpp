#include "HWNetworkManager.h"
#include <algorithm>

HWNetworkManager* HWNetworkManager::instance;

HWNetworkManager::HWNetworkManager()
{
	socket = new HWSocket();
	packet_count = 0;
}


HWNetworkManager::~HWNetworkManager()
{
	std::vector<HWCircuit*>::iterator iter = circuits.begin();
	for(; iter != circuits.end(); ++iter)
	{
		HWCircuit* circ = *iter;
		delete circ;
	}
	delete socket;
}

HWNetworkManager* HWNetworkManager::GetInstance()
{
	if(instance == nullptr)
	{
		instance = new HWNetworkManager();
	}
	return instance;
}

void HWNetworkManager::Update()
{
	std::vector<HWCircuit*>::iterator iter = circuits.begin();
	for(; iter != circuits.end(); ++iter)
	{
		HWCircuit* circ = *iter;
		HWSerializer stream = HWSerializer();
		std::vector<const HWPacket*>::iterator iter2 = circ->outbox.begin();
		for(; iter2 != circ->outbox.end();)
		{
			const HWPacket* packet = *iter2;
			// Append packet data onto the stream
			packet->Serialize(stream);
			if(!packet->HasFlag(HWPacketFlag::Reliable)) iter2 = circ->outbox.erase(iter2);
			else ++iter2;
		}

		// Fix this to do splitting
		HWUDPPacket packet = HWUDPPacket(packet_count++, 0, 1, stream);
		stream.Clear();
		packet.Serialize(stream);

		U8* data = new U8[stream.GetDataLength()];
		S64 length = stream.GetData(&data);

		socket->Send(circ->end_point, data, length);
	}

	S64 length = 0;
	HWIPEndPoint sender = HWIPEndPoint();
	while((length = socket->RecvFrom(sender, buffer, buffer_length)) > 0)
	{
		std::vector<HWCircuit*>::iterator iter = std::find_if(circuits.begin(), circuits.end(),
			[&sender](const HWCircuit* circ) -> bool { return circ->end_point.GetIPAddress() == sender.GetIPAddress(); });
		HWCircuit* circ = *iter;

		HWSerializer stream = HWSerializer();
		stream.Write(buffer, length);

		HWUDPPacket* pack = new HWUDPPacket(stream);
		circ->Recieve(pack);
	}

	if(length == SOCKET_ERROR)
	{
		S32 error = WSAGetLastError();
		switch(error)
		{
		case WSAEWOULDBLOCK:
			// This is expected at the end of the loop
			break;
		default:
			printf("Socket error on recieveing: %i", error);
			break;
		}
	}
}

HWCircuit* HWNetworkManager::CreateCircuit(const HWIPEndPoint& ep)
{
	HWCircuit* circ = new HWCircuit(ep);
	circuits.push_back(circ);
	//TODO: circ->Send(handshake);
	return circ;
}

void HWNetworkManager::Bind(U16 port)
{
	socket->Bind(port);
}
