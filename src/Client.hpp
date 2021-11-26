#pragma once

#include "Socket.hpp"

#include "IpAddressAndPort.hpp"

namespace Soup
{
	struct Client : public Socket
	{
		IpAddressAndPort peer;
	};
}
