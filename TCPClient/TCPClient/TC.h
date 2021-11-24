#pragma once

#pragma warning( disable : 4996)
#define WIN32_LEAN_AND_MEAN

#include <string>
#include <windows.h>
#include <winsock2.h>
#include <ws2tcpip.h>
#include <stdlib.h>
#include <stdio.h>
#include <thread>
#include <vector>
#include <functional>

// Need to link with Ws2_32.lib, Mswsock.lib, and Advapi32.lib
#pragma comment (lib, "Ws2_32.lib")
#pragma comment (lib, "Mswsock.lib")
#pragma comment (lib, "AdvApi32.lib")

class TC
{

public:

	void init();
	bool connect_server(std::string ipaddress, std::string port); 
	void disconnect();
	void send_buffer(const char* buffer, size_t size);
	bool is_connected() const;
	
private:
	void handle_read();
	SOCKET ConnectSocket;
	bool isRunning_;
	bool is_connected_;
	std::vector<std::shared_ptr<std::thread>> thread_pool_;

};

