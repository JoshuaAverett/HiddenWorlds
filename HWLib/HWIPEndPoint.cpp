#include "HWIPEndPoint.h"
#include "HWNetworkManager.h"

HWIPEndPoint::HWIPEndPoint()
{
}

HWIPEndPoint::HWIPEndPoint(const char* host_name, U16 port)
{
#ifdef HW_OS_WINDOWS
	HWSocket::InitWSA();
#endif
	hostent* hosts = gethostbyname(host_name);
	if(!hosts) return;

	sockaddr_in temp;
	memcpy(&temp.sin_addr, hosts->h_addr_list[0], hosts->h_length);
	temp.sin_family = AF_INET;
	temp.sin_port = htons(port);

	memcpy(&value, &temp, sizeof temp);
}

HWIPEndPoint::HWIPEndPoint(const sockaddr& sa)
{
	memcpy(&value, &sa, sizeof sa);
}

HWIPEndPoint::~HWIPEndPoint()
{
}

sockaddr HWIPEndPoint::GetSocketAddress() const
{
	return value;
}

U16 HWIPEndPoint::GetPort() const
{
	return (*(sockaddr_in*)&value).sin_port;
}

U32 HWIPEndPoint::GetIPAddress() const
{
	return (*(sockaddr_in*)&value).sin_addr.s_addr;
}