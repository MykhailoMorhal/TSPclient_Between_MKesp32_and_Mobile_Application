/*----------------------------------------------------------------------
                                Include
*----------------------------------------------------------------------*/
#include <iostream>
#include "stream_get_send.h"
#include <WS2tcpip.h>
#include <string>
#include <time.h>

#pragma comment(lib, "ws2_32.lib")

/*----------------------------------------------------------------------
                                Defines
*----------------------------------------------------------------------*/
#define PORT 							23
#define IP_ADDRES 						"xxxxxxxxxxxxxx"
#define BUFF_SIZE						4096u

static int bytesReceived;
static SOCKET sock_client;
static char buff[BUFF_SIZE];

/*----------------------------------------------------------------------
                                Forvard declaration
*----------------------------------------------------------------------*/
void get_data_from_esp_1();
void send_data_to_application(	const std::string level1, const std::string level2, 
								const std::string level3, const std::string level4);
void delay(unsigned timeout);
void check_recive_error();
void check_send_error(const int sock);

static void socket_init()
{
	//Initialze winsock
	WSADATA wsDATA;
	WORD ver = MAKEWORD(2, 2);
	int wSock = WSAStartup(ver, &wsDATA);
	char host[NI_MAXHOST];  	// Client's remote name
	char service[NI_MAXSERV];  	// Service (i.e. port) the client is connect on

	if ( 0 != wSock) 
	{
		std::cerr << "Can't initialize winsock! Quitting" << std::endl;
	}
	else 
	{
		std::cout << "Initialize wSock is OK" << std::endl;
	}
	//Create socket
	SOCKET sock_listening = socket(AF_INET, SOCK_STREAM, 0);

	if (sock_listening == INVALID_SOCKET) 
	{
		std::cerr << "CAN'T CREATE socket listening! Quitting" << std::endl;
	}
	else 
	{
		std::cout << "Create sock_listening" << std::endl;
	}

	//Bind the socket to ip address and port
	sockaddr_in hint;
	hint.sin_family = AF_INET;
	hint.sin_port = htons(PORT);
	hint.sin_addr.S_un.S_addr = INADDR_ANY;

	bind(sock_listening, (sockaddr*)&hint, sizeof(hint));
	//Tell winsock the socket is for listening
	listen(sock_listening, SOMAXCONN);
	//Wait for connection
	sockaddr_in client;
	int clientSize = sizeof(client);
	sock_client = accept(sock_listening, (sockaddr*)&client, &clientSize);

	if (sock_client == INVALID_SOCKET) 
	{
		std::cerr << "CAN'T CREATE socket client! Quitting" << std::endl;
	}
	else 
	{
		std::cout << "Create sock_client" << std::endl;
	}

	ZeroMemory(host, NI_MAXHOST);
	ZeroMemory(service, NI_MAXSERV);

	if (getnameinfo((sockaddr*)&client, sizeof(client), host, NI_MAXHOST, service, NI_MAXSERV, 0) == 0) 
	{
		std::cout << host << " CONNECT on port " << service << std::endl;
	}
	else 
	{
		inet_ntop(AF_INET, &client.sin_addr, host, NI_MAXHOST);
		std::cout << host << " CONNECT on port " <<
			ntohs(client.sin_port) << std::endl;
	}

	//Close listening socket
	closesocket(sock_listening);
}

void run_socket_server()
{
	while (true)
	{
		// init server
		socket_init();

		bool check_device = true;

		// Loop for get/set data from server
		while (check_device) {

			ZeroMemory(buff, 4096);
			bytesReceived = recv(sock_client, buff, 1, 0);
			check_recive_error();
			int check_device_input = atoi(buff);

			switch (check_device_input)
			{
			case 1:
				std::cout << "Take data from ESP " << std::endl;
				get_data_from_esp_1(getPath());
				get_data_from_esp_1(getPath());
				get_data_from_esp_1(getPath());
				get_data_from_esp_1(getPath());
				check_device = false;
				break;
			case 2:
				std::cout << "Send data to aplication" << std::endl;
				send_data_to_application(valueSensor1_level1, valueSensor1_level2, 
										valueSensor1_level3, valueSensor1_level4);
				break;
			default:
				std::cout << "Unknown device" << std::endl;
				delay(1000);
				check_device = false;
				break;
			}
		}
		//Close the socket
		closesocket(sock_client);
		//Clean winsock
		WSACleanup();
	}
	
}

static void get_data_from_esp_1(const std::string path) 
{

	ZeroMemory(buff, 4096);
	bytesReceived = recv(sock_client, buff, 5, 0);
	check_recive_error();
	write_data_to_file(buff, path);
}

static void send_data_to_application(	const std::string level1, const std::string level2, 
								const std::string level3, const std::string level4) 
{
	check_send_error(send(sock_client, get_data_from_file(level1).c_str(), 7, 0));
	check_send_error(send(sock_client, get_data_from_file(level2).c_str(), 7, 0));
	check_send_error(send(sock_client, get_data_from_file(level3).c_str(), 7, 0));
	check_send_error(send(sock_client, get_data_from_file(level4).c_str(), 7, 0));
}

static void delay(unsigned timeout)
{
	timeout += clock();
	while (clock() < timeout) continue;
}

static void check_recive_error() 
{
	if (bytesReceived == SOCKET_ERROR) 
	{
		std::cerr << "Error in RECIVE" << std::endl;
	}

	if (bytesReceived == 0) 
	{
		std::cout << "client DISCONNECTED" << std::endl;
	}
	delay(3000);
}

void check_send_error(const int sock) 
{
	if (sock < 0)
		std::cout << "Error in send data to application" << std::endl;
}