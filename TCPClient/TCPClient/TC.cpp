#include "TC.h"

void TC::init()
{
	WSADATA wsaData;
	ConnectSocket = INVALID_SOCKET;
	int iResult;
	// Initialize Winsock
	iResult = WSAStartup(MAKEWORD(2, 2), &wsaData);
	if (iResult != 0)
	{
		printf("WSAStartup failed with error: %d\n", iResult);
	}
	isRunning_ = false;
	is_connected_ = false;;
}

bool TC::connect_server(std::string ipaddress, std::string port)
{
	int iResult;

	struct sockaddr_in dest_addr;
	dest_addr.sin_family = AF_INET;          // host byte order
	dest_addr.sin_port = htons(std::atoi(port.c_str()));   // short, network byte order
	dest_addr.sin_addr.s_addr = inet_addr(ipaddress.c_str()); //destination IP
	 
		// Create a SOCKET for connecting to server
	ConnectSocket = socket(AF_INET, SOCK_STREAM, 0);
	if (ConnectSocket == INVALID_SOCKET) {
		printf("socket failed with error: %ld\n", WSAGetLastError());
		WSACleanup();
		return false;
	}


	//error check the connect()
	if (connect(ConnectSocket, (struct sockaddr*) & dest_addr, sizeof(struct sockaddr)) == -1) {
		closesocket(ConnectSocket);
		ConnectSocket = INVALID_SOCKET;
		printf("Socket connection failed.\n");
		return false;
	}


	if (ConnectSocket == INVALID_SOCKET) {
		printf("Unable to connect to server!\n");
		WSACleanup();
		return false;
	}
	isRunning_ = true;
	is_connected_ = true;;
	std::shared_ptr<std::thread> t1 = std::make_shared<std::thread>(std::bind(&TC::handle_read, this));
	thread_pool_.push_back(t1);

	return is_connected_;
}


void TC::disconnect()
{
	isRunning_ = false;
	is_connected_ = false;

	closesocket(ConnectSocket);
	//WSACleanup();
	for (int i = 0; i < thread_pool_.size(); i++)
		if (thread_pool_[i]->joinable())
			thread_pool_[i]->join();
}

void TC::send_buffer(const char* buffer, size_t size)
{
	if (is_connected_)
	{
		int iResult = send(ConnectSocket, buffer, size, 0);
		if (iResult == SOCKET_ERROR) {
			printf("send failed with error: %d\n", WSAGetLastError());
			disconnect();
		}
	}
}

bool TC::is_connected() const
{
	return is_connected_;
}

void TC::handle_read()
{
	char recvbuf[2048];
	int  iResult;
	int  recvbuflen = 2048;
	while (isRunning_ && is_connected_)
	{
		iResult = recv(ConnectSocket, recvbuf, recvbuflen, 0);
		if (iResult > 0)
		{
			printf("Bytes received: %d\n", iResult);
			send_buffer(recvbuf, iResult);
		}
		else if (iResult == 0)
		{
			printf("Connection closed\n");
			is_connected_ = false;
			isRunning_ = false;
		}
		else
			printf("recv failed with error: %d\n", WSAGetLastError());

	}
}
