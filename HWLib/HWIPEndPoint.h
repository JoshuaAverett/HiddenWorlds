#pragma once

#include "Master.h"
#include <WinSock2.h>
#pragma comment(lib, "Ws2_32.lib")

class HWIPEndPoint
{
public:
	HWIPEndPoint();
	HWIPEndPoint(const HWIPEndPoint& copy);
	HWIPEndPoint(const char* host_name, U16 port);
	HWIPEndPoint(const sockaddr& sa);
	~HWIPEndPoint();

	sockaddr GetSocketAddress() const;
	U16 GetPort() const;
	U32 GetIPAddress() const;

private:
	sockaddr value;
};

