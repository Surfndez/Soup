#pragma once

#include <cstdint>
#include <cstring> // memcpy
#include <stdexcept>
#include <string>

#include "Client.hpp"
#include "Platform.hpp"

#if SOUP_PLATFORM_WINDOWS
#pragma comment(lib, "Ws2_32.lib")
#include <Winsock2.h>
#include <Ws2tcpip.h>
#else
#include <sys/socket.h>
#include <netinet/in.h>
#endif

namespace Soup
{
	struct Server : public Socket
	{
#if SOUP_PLATFORM_WINDOWS
		inline static size_t wsa_consumers = 0;
#endif

		explicit Server(const uint16_t port)
		{
#if SOUP_PLATFORM_WINDOWS
			if(wsa_consumers++ == 0)
			{
				WSADATA wsaData;
				WORD wVersionRequested = MAKEWORD(2, 2);
				WSAStartup(wVersionRequested, &wsaData);
			}
#endif
			fd = socket(AF_INET6, SOCK_STREAM, 0);
			if(fd == -1)
			{
				_throw("socket");
			}
			sockaddr_in6 addr{}; // bind to any because all zeroes by construction 🙂
			addr.sin6_family = AF_INET6;
			addr.sin6_port = htons(port);
			if(bind(fd, (sockaddr*)&addr, sizeof(addr)) == -1)
			{
				_throw("bind");
			}
			if(listen(fd, 100) == -1)
			{
				_throw("listen");
			}
		}

		~Server()
		{
#if SOUP_PLATFORM_WINDOWS
			if(--wsa_consumers == 0)
			{
				WSACleanup();
			}
#endif
		}

		[[nodiscard]] Client accept()
		{
			Client res;
			sockaddr_in6 addr;
#if SOUP_PLATFORM_WINDOWS
			using socklen_t = int;
#endif
			socklen_t addrlen = sizeof(addr); // Windows takes int* instead of unsigned int*
			res.fd = ::accept(fd, (sockaddr*)&addr, &addrlen);
			if(res.fd == -1)
			{
				_throw("accept");
			}
			memcpy(&res.peer.addr, &addr.sin6_addr, sizeof(addr.sin6_addr));
			res.peer.port = addr.sin6_port;
			return res;
		}
	};
}
