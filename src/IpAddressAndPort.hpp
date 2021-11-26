#pragma once

#include "IpAddress.hpp"

namespace Soup
{
	struct IpAddressAndPort
	{
		IpAddress addr;
		uint16_t port;

		[[nodiscard]] std::string toString() const noexcept
		{
			std::string str;
			if(addr.isV4())
			{
				str.append(addr.toString4());
			}
			else
			{
				str.push_back('[');
				str.append(addr.toString6());
				str.push_back(']');
			}
			str.push_back(':');
			str.append(std::to_string(port));
			return str;
		}
	};
}
