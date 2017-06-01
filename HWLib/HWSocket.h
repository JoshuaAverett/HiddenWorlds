#pragma once

#include "Master.h"
#include "HWIPEndPoint.h"

#include <WinSock2.h>
#pragma comment(lib, "Ws2_32.lib")

class HWSocket
{
public:
	friend class HWIPEndPoint;

	HWSocket();
	HWSocket(const HWSocket& copy);
	~HWSocket();

	bool Bind(U16 port);
	S64 Send(const HWIPEndPoint& ep, const U8* data, S64 length) const;
	S64 RecvFrom(HWIPEndPoint& sender, U8* buffer, S64 length) const;

private:
	SOCKET socket_id;
	sockaddr* socket_data;
	static S32 socket_count;

#ifdef HW_OS_WINDOWS
	static WSADATA wsa_data;
	static bool initialized;

	static void InitWSA();
#endif
};
