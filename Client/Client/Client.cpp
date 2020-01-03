#include "pch.h"
#include <winsock2.h>
#include <ws2tcpip.h>
#include "iostream"
#include <tchar.h>

using namespace std;

class ClientSetUp {
public:
	SOCKET clientSocket;
	int port = 55555;
	int max = 2;


	int winSockCheck() {
		WSADATA wsaData;
		int winSockPresent;
		WORD wVersionRequested = MAKEWORD(2, 2);
		bool err =1;
		

		winSockPresent = WSAStartup(wVersionRequested, &wsaData);
		try
		{
			if (winSockPresent != 0) throw err;
			cout << "The Winsock dll found!" << endl;
			cout << "The status: " << wsaData.szSystemStatus << endl;
		}
		catch (const exception& err) {
			cerr << "The Winsock dll not found!" << endl;
			return 0;
		}
	}

	int socketCheck() {
		bool err = 1;
		clientSocket = INVALID_SOCKET;
		clientSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
		sockaddr_in clientService;
		clientService.sin_family = AF_INET;
		InetPton(AF_INET, _T("127.0.0.1"), &clientService.sin_addr.s_addr);
		clientService.sin_port = htons(port);
		int more = max;
		
		if (clientSocket == INVALID_SOCKET) {
			cerr << "Error at socket(): " << WSAGetLastError() << endl;
			cout << " Re attempting "<<more<<" more times.";
			for (int i = 0; i < max; i++)
			{
				socketCheck();
				more--;
				if (max ==0)
				{
					cout << "Exiting program failed to find socket";
					WSACleanup();
					return 0;
				}
			}
		}
		else {
			cout << "socket() is OK!" << endl;
		}

		try
		{
			if (connect(clientSocket, (SOCKADDR*)&clientService, sizeof(clientService)) == SOCKET_ERROR) throw err;
			cout << "Client: connect() is OK." << endl;
			cout << "Client: Can start sending and receiving data..." << endl;
		}
		catch (const exception& err)
		{
			cerr << "Client: connect() - Failed to connect." << endl;
			cout << " Closing Program please wait.";
			WSACleanup();
			return 0;
		}
	}

	int bufferFlow() {
		int byteCount = SOCKET_ERROR;
		char buffer[200];
		char receiveBuffer[200] = "";

		while (1) {
			cout << "Enter your message ";
			cin.getline(buffer, 200);
			byteCount = send(clientSocket, buffer, 200, 0);
			if (byteCount == SOCKET_ERROR) {
				cout << "Client: send() error " << WSAGetLastError() << endl;
			}
			else {
				cout << "Please Wait for a response form the server. " << endl;
				if (strcmp(buffer, "QUIT") == 0) {
					//closesocket(clientSocket);
					try
					{
						terminate();
					}
					catch (const exception &e) {
						cout << e.what() << endl;
					}
					break;
				}
			}
			byteCount = recv(clientSocket, receiveBuffer, 200, 0);
			if (byteCount == 0 || byteCount == WSAECONNRESET) cout << "Client: Connection Closed." << endl;
			if (byteCount < 0) cout << "Client: error " << WSAGetLastError() << endl;
			else cout << "From Server :  " << receiveBuffer << endl;
		}
	};
};

int _tmain(int argc, _TCHAR* argv[]) {
	ClientSetUp client;
	client.winSockCheck();
	client.socketCheck();
	client.bufferFlow();
	system("pause");
	WSACleanup();
	return 0;

}
/*
	// Exercise 6
	Data data;
	int byteCount = SOCKET_ERROR;
	while (1) {
		byteCount = recv(clientSocket, (char *)&data, sizeof(Data), 0);
		if (byteCount == 0 || byteCount == WSAECONNRESET) {
			cout << "Client: Connection Closed." << endl;
			break;
		}
		if (byteCount < 0) {
			cout << "Client: error " << WSAGetLastError() << endl;
			break;
		}
		else {
			cout << "Name :  " << data.name << endl;
			cout << "Health : " << data.health << endl;

		}
	}*/