// TCPClient.cpp : This file contains the 'main' function. Program execution begins and ends there.
//
#pragma warning( disable : 4996)

#include <iostream>
#include <thread>

#include "TC.h"

int main()
{ 
	std::string ip = "172.17.1.11";
	std::string port = "5656";
	std::shared_ptr<TC> tcp_client = std::make_shared<TC>();
	tcp_client->init();
	tcp_client->connect_server(ip, port);

	while (true)
	{
		if (!tcp_client->is_connected())
		{
			tcp_client->connect_server(ip, port);
		}
		std::this_thread::sleep_for(std::chrono::seconds(3));
	}
} 
 