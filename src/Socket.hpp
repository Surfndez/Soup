#pragma once

#include "Platform.hpp"

#if SOUP_PLATFORM_WINDOWS
#include <Winsock2.h> // closesocket, WSAGetLastError
#else
#include <string.h> // strerror
#include <unistd.h> // close
#endif

namespace Soup
{
	struct Socket
	{
		int fd = -1;

		constexpr Socket() noexcept = default;

		Socket(const Socket&) = delete;

		Socket(Socket&& b) noexcept
			: fd(b.fd)
		{
			b.fd = -1;
		}

		void operator =(const Socket&) = delete;

		void operator =(Socket&& b) noexcept
		{
			fd = b.fd;
			b.fd = -1;
		}

		~Socket() noexcept
		{
			if(fd != -1)
			{
#if SOUP_PLATFORM_WINDOWS
				closesocket(fd);
#else
				close(fd);
#endif
			}
		}

		[[nodiscard]] static int _errno()
		{
#if SOUP_PLATFORM_WINDOWS
			return WSAGetLastError();
#else
			return errno;
#endif
		}

		[[noreturn]] static void _throw(const char* const func)
		{
			std::string msg = func;
#if SOUP_PLATFORM_WINDOWS
			msg.append(" failed with error code ");
			msg.append(std::to_string(_errno()));
#else
			msg.append(" failed: ");
			msg.append(strerror(_errno()));
			msg.append(" (Code ");
			msg.append(std::to_string(_errno()));
			msg.push_back(')');
#endif
			throw std::runtime_error(msg);
		}
	};
}
