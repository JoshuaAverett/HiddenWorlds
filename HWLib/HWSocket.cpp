#include "HWSocket.h"
#include <stdio.h>

#ifdef HW_OS_WINDOWS
WSADATA HWSocket::wsa_data;
bool HWSocket::initialized = false;

void HWSocket::InitWSA()
{
	if(!initialized)
	{
		initialized = true;
		if(WSAStartup(MAKEWORD(2, 2), &wsa_data) != NO_ERROR)
		{
			exit(1);
		}
	}
}
#endif

HWSocket::HWSocket()
{
#ifdef HW_OS_WINDOWS
	InitWSA();
#endif
	socket_id = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
	if(socket_id == INVALID_SOCKET)
	{
		exit(1);
	}
	++socket_count;
	
	static U32 non_blocking = TRUE;
	ioctlsocket(socket_id, FIONBIO, &non_blocking);
}

HWSocket::~HWSocket()
{
	--socket_count;
	closesocket(socket_id);
#ifdef HW_OS_WINDOWS
	if(socket_count == 0)
	{
		WSACleanup();
	}
#endif
}

bool HWSocket::Bind(U16 port)
{
	sockaddr_in sin;
	memset(&sin, 0, sizeof sin);

	sin.sin_family = AF_INET;
	sin.sin_port = htons(port);
	sin.sin_addr.s_addr = htonl(INADDR_ANY);

	if(bind(socket_id, (SOCKADDR*)&sin, sizeof sin) == SOCKET_ERROR)
	{
		return false;
	}

	socket_data = (sockaddr*)&sin;

	return true;
}

S64 HWSocket::Send(const HWIPEndPoint& ep, const U8* data, S64 length) const
{
	sockaddr dest_addr = ep.GetSocketAddress();
	S64 len = sendto(socket_id, (char*)data, (int)length, 0, &dest_addr, sizeof dest_addr);
	return len;
}

S64 HWSocket::RecvFrom(HWIPEndPoint& sender, U8* buffer, S64 length) const
{
	sockaddr temp;
	S64 temp_length = sizeof temp;
	S64 len = recvfrom(socket_id, (char*)buffer, (int)length, 0, &temp, (int*)&temp_length);
	sender = HWIPEndPoint(temp);
	return len;
}

S32 HWSocket::socket_count = 0;