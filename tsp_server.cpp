#include <iostream>
#include "stream_get_send.h"
#include <WS2tcpip.h>
#include <string>
#include <time.h>

#pragma comment(lib, "ws2_32.lib")

using namespace std;

#define PORT 23
#define IP_ADDRES "31.182.164.199"

int bytesReceived;
SOCKET sock_client;
char buff[4096];
/*
string value_sensor1_level1 = "C:\\my_projects\\Cpp_projects\\ServerTcp\\ServerTcp\\Sensor_1\\valueLevel1.txt";
string value_sensor1_level2 = "C:\\my_projects\\Cpp_projects\\ServerTcp\\ServerTcp\\Sensor_1\\valueLevel2.txt";
string value_sensor1_level3 = "C:\\my_projects\\Cpp_projects\\ServerTcp\\ServerTcp\\Sensor_1\\valueLevel3.txt";
string value_sensor1_level4 = "C:\\my_projects\\Cpp_projects\\ServerTcp\\ServerTcp\\Sensor_1\\valueLevel4.txt";

string value_sensor2_level1 = "C:\\my_projects\\Cpp_projects\\ServerTcp\\ServerTcp\\Sensor_2\\valueLevel1.txt";
string value_sensor2_level2 = "C:\\my_projects\\Cpp_projects\\ServerTcp\\ServerTcp\\Sensor_2\\valueLevel2.txt";
string value_sensor2_level3 = "C:\\my_projects\\Cpp_projects\\ServerTcp\\ServerTcp\\Sensor_2\\valueLevel3.txt";
string value_sensor2_level4 = "C:\\my_projects\\Cpp_projects\\ServerTcp\\ServerTcp\\Sensor_2\\valueLevel4.txt";

string value_sensor3_level1 = "C:\\my_projects\\Cpp_projects\\ServerTcp\\ServerTcp\\Sensor_3\\valueLevel1.txt";
string value_sensor3_level2 = "C:\\my_projects\\Cpp_projects\\ServerTcp\\ServerTcp\\Sensor_3\\valueLevel2.txt";
string value_sensor3_level3 = "C:\\my_projects\\Cpp_projects\\ServerTcp\\ServerTcp\\Sensor_3\\valueLevel3.txt";
string value_sensor3_level4 = "C:\\my_projects\\Cpp_projects\\ServerTcp\\ServerTcp\\Sensor_3\\valueLevel4.txt";
D:\My project\Project_of_C++\ServerTcp\ServerTcp
*/

string value_sensor1_level1 = "D:\\My project\\Project_of_C++\\ServerTcp\\ServerTcp\\Sensor_1\\valueLevel1.txt";
string value_sensor1_level2 = "D:\\My project\\Project_of_C++\\ServerTcp\\ServerTcp\\Sensor_1\\valueLevel2.txt";
string value_sensor1_level3 = "D:\\My project\\Project_of_C++\\ServerTcp\\ServerTcp\\Sensor_1\\valueLevel3.txt";
string value_sensor1_level4 = "D:\\My project\\Project_of_C++\\ServerTcp\\ServerTcp\\Sensor_1\\valueLevel4.txt";

string value_sensor2_level1 = "D:\\My project\\Project_of_C++\\ServerTcp\\ServerTcp\\Sensor_2\\valueLevel1.txt";
string value_sensor2_level2 = "D:\\My project\\Project_of_C++\\ServerTcp\\ServerTcp\\Sensor_2\\valueLevel2.txt";
string value_sensor2_level3 = "D:\\My project\\Project_of_C++\\ServerTcp\\ServerTcp\\Sensor_2\\valueLevel3.txt";
string value_sensor2_level4 = "D:\\My project\\Project_of_C++\\ServerTcp\\ServerTcp\\Sensor_2\\valueLevel4.txt";

string value_sensor3_level1 = "D:\\My project\\Project_of_C++\\ServerTcp\\ServerTcp\\Sensor_3\\valueLevel1.txt";
string value_sensor3_level2 = "D:\\My project\\Project_of_C++\\ServerTcp\\ServerTcp\\Sensor_3\\valueLevel2.txt";
string value_sensor3_level3 = "D:\\My project\\Project_of_C++\\ServerTcp\\ServerTcp\\Sensor_3\\valueLevel3.txt";
string value_sensor3_level4 = "D:\\My project\\Project_of_C++\\ServerTcp\\ServerTcp\\Sensor_3\\valueLevel4.txt";

void get_data_from_esp_1();
void get_data_from_esp_2();
void get_data_from_esp_3();
void send_data_to_application(string level1, string level2, string level3, string level4);
void delay(unsigned timeout);
void check_recive_error();
void check_send_error(int sock);

void socket_server_ini()
{
	while (true)
	{
		//Initialze winsock
		WSADATA wsDATA;
		WORD ver = MAKEWORD(2, 2);
		int wSock = WSAStartup(ver, &wsDATA);

		if (wSock != 0) {
			cerr << "Can't initialize winsock! Quitting" << endl;
			//return ;
		}
		else {
			cout << "Initialize wSock is OK" << endl;
		}

		//Create socket
		SOCKET sock_listening = socket(AF_INET, SOCK_STREAM, 0);

		if (sock_listening == INVALID_SOCKET) {
			cerr << "CAN'T CREATE socket listening! Quitting" << endl;
			//return;
		}
		else {
			cout << "Create sock_listening" << endl;
		}

		//Bind the socket to ip address and port
		sockaddr_in hint;
		hint.sin_family = AF_INET;
		hint.sin_port = htons(PORT);
		hint.sin_addr.S_un.S_addr = INADDR_ANY;// говорит о том, что сервер будет доступен с любой пользовательской машины. 

		bind(sock_listening, (sockaddr*)&hint, sizeof(hint));

		//Tell winsock the socket is for listening
		listen(sock_listening, SOMAXCONN);

		//Wait for connection
		sockaddr_in client;
		int clientSize = sizeof(client);



		sock_client = accept(sock_listening, (sockaddr*)&client, &clientSize);
		//cout << "Waiting for connection" << endl;

		if (sock_client == INVALID_SOCKET) {
			cerr << "CAN'T CREATE socket client! Quitting" << endl;
		}
		else {
			cout << "Create sock_client" << endl;
		}


		char host[NI_MAXHOST];  // Client's remote name
		char service[NI_MAXSERV];  // Service (i.e. port) the client is connect on

		ZeroMemory(host, NI_MAXHOST);
		ZeroMemory(service, NI_MAXSERV);

		if (getnameinfo((sockaddr*)&client, sizeof(client), host, NI_MAXHOST, service, NI_MAXSERV, 0) == 0) {
			cout << host << " CONNECT on port " << service << endl;
		}
		else {
			inet_ntop(AF_INET, &client.sin_addr, host, NI_MAXHOST);
			cout << host << " CONNECT on port " <<
				ntohs(client.sin_port) << endl;
		}

		//Close listening socket
		closesocket(sock_listening);

		//While loop: accept and echo message back to client
		char arg[] = { "\n" };
		bool check_device = true;

		while (check_device) {

			ZeroMemory(buff, 4096);
			bytesReceived = recv(sock_client, buff, 1, 0);
			check_recive_error();
			int check_device_input = atoi(buff);

			switch (check_device_input)
			{
			case 1:
				cout << "Begin take data from ESP first place" << endl;
				get_data_from_esp_1();
				check_device = false;
				break;
			case 2:
				cout << "Begin take data from ESP second place" << endl;
				get_data_from_esp_2();
				check_device = false;
				break;
			case 3:
				cout << "Begin take data from ESP third place" << endl;
				get_data_from_esp_3();
				check_device = false;
				break;
			case 4:
				cout << "Send data to aplication" << endl;
				send_data_to_application(value_sensor1_level1, value_sensor1_level2, 
					value_sensor1_level3, value_sensor1_level4);
				delay(1000);

				send_data_to_application(value_sensor2_level1, value_sensor2_level2,
					value_sensor2_level3, value_sensor2_level4);
				delay(1000);

				send_data_to_application(value_sensor3_level1, value_sensor3_level2,
					value_sensor3_level3, value_sensor3_level4);
				delay(1000);
				break;
			default:
				cout << "Unknown device" << endl;
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

void get_data_from_esp_1() {

	ZeroMemory(buff, 4096);
	bytesReceived = recv(sock_client, buff, 5, 0);
	check_recive_error();
	write_data_to_txt(buff, value_sensor1_level1);
	cout << buff << endl;


	bytesReceived = recv(sock_client, buff, 5, 0);
	check_recive_error();
	write_data_to_txt(buff, value_sensor1_level2);
	cout << buff << endl;

	bytesReceived = recv(sock_client, buff, 5, 0);
	check_recive_error();
	write_data_to_txt(buff, value_sensor1_level3);
	cout << buff << endl;

	bytesReceived = recv(sock_client, buff, 5, 0);
	check_recive_error();
	write_data_to_txt(buff, value_sensor1_level4);
	cout << buff << endl;
}

void get_data_from_esp_2() {

	ZeroMemory(buff, 4096);
	bytesReceived = recv(sock_client, buff, 5, 0);
	check_recive_error();
	write_data_to_txt(buff, value_sensor2_level1);
	cout << buff << endl;


	bytesReceived = recv(sock_client, buff, 5, 0);
	check_recive_error();
	write_data_to_txt(buff, value_sensor2_level2);
	cout << buff << endl;

	bytesReceived = recv(sock_client, buff, 5, 0);
	check_recive_error();
	write_data_to_txt(buff, value_sensor2_level3);
	cout << buff << endl;

	bytesReceived = recv(sock_client, buff, 5, 0);
	check_recive_error();
	write_data_to_txt(buff, value_sensor2_level4);
	cout << buff << endl;
}

void get_data_from_esp_3() {

	ZeroMemory(buff, 4096);
	bytesReceived = recv(sock_client, buff, 5, 0);
	check_recive_error();
	write_data_to_txt(buff, value_sensor3_level1);
	cout << buff << endl;


	bytesReceived = recv(sock_client, buff, 5, 0);
	check_recive_error();
	write_data_to_txt(buff, value_sensor3_level2);
	cout << buff << endl;

	bytesReceived = recv(sock_client, buff, 5, 0);
	check_recive_error();
	write_data_to_txt(buff, value_sensor3_level3);
	cout << buff << endl;

	bytesReceived = recv(sock_client, buff, 5, 0);
	check_recive_error();
	write_data_to_txt(buff, value_sensor3_level4);
	cout << buff << endl;
}


void send_data_to_application(string level1, string level2, string level3, string level4) {

	string arg1 = get_data_from_txt(level1) + "\n";
	string arg2 = get_data_from_txt(level2) + "\n";
	string arg3 = get_data_from_txt(level3) + "\n";
	string arg4 = get_data_from_txt(level4) + "\n";

	const char* get_from_level_1 = arg1.c_str();
	const char* get_from_level_2 = arg2.c_str();
	const char* get_from_level_3 = arg3.c_str();
	const char* get_from_level_4 = arg4.c_str();


	cout << get_from_level_1 << endl;
	check_send_error(send(sock_client, get_from_level_1, 7, 0));
	delay(1000);
	cout << get_from_level_2 << endl;
	check_send_error(send(sock_client, get_from_level_2, 7, 0));
	delay(1000);
	cout << get_from_level_3 << endl;
	check_send_error(send(sock_client, get_from_level_3, 7, 0));
	delay(1000);
	cout << get_from_level_4 << endl;
	check_send_error(send(sock_client, get_from_level_4, 7, 0));
	delay(1000);
	
}

void delay(unsigned timeout)
{
	timeout += clock();
	while (clock() < timeout) continue;
}
int i = 0;
void check_recive_error() {
	if (bytesReceived == SOCKET_ERROR) {
		cerr << "Error in RECIVE" << endl;
	}

	if (bytesReceived == 0) {
		cout << "client DISCONNECTED" << endl;
	}
	delay(3000);
}

void check_send_error(int sock) {
	if (sock < 0)
		cout << "Error in send data to application" << endl;
}