#pragma once

#include "HWCircuit.h"
#include "HWSocket.h"
#include <vector>

class HWNetworkManager
{
public:
	static HWNetworkManager* GetInstance();

	void Update();
	HWCircuit* CreateCircuit(const HWIPEndPoint& ep);
	void Bind(U16 port);

private:
	std::vector<HWCircuit*> circuits;
	static const S32 buffer_length = 65536;
	U8 buffer[buffer_length];
	HWSocket* socket;
	S32 packet_count;
	static HWNetworkManager* instance;

	HWNetworkManager();
	~HWNetworkManager();
};
