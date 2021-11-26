#include <iostream>
#include <thread>

#include "../src/Server.hpp"

int main()
{
	try
	{
		Soup::Server srv{ 1337 };
		std::cout << "Listening on *:1337..." << std::endl;
		while(true)
		{
			auto t = std::thread([/* Soup::Client */ client = srv.accept() ]
			{
				auto name = client.peer.toString();
				try
				{
					std::cout << name << " + connected" << std::endl;

					while(true)
					{
						constexpr auto bufsize = 1024;
						std::string buf(bufsize, 0);
						int read = recv(client.fd, &buf.at(0), bufsize, 0);
						if(read <= 0) // -1 = error, 0 = closed
						{
							break;
						}
						buf.resize(read);
						std::cout << name << " > " << buf << std::endl;
					}

					std::cout << name << " - disconnected" << std::endl;
				}
				catch(const std::exception& e)
				{
					std::cout << name << " ! " << e.what() << std::endl;
				}
			});
			t.detach();
		}
	}
	catch(const std::exception& e)
	{
		std::cout << e.what();
	}
}
