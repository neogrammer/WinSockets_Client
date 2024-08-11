extern "C"
{
#include "lua.h"
#include "lualib.h"
#include "lauxlib.h"
}

#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>

#include <Winsock2.h>
#include <Ws2tcpip.h>
#include <iostream>

sf::RenderWindow gWnd;
float gTime;

int gPort = 55555;

void input();
void update();
void render();

int main()
{
	WSADATA wsaData;
	int wsaerr;
	WORD wVersionRequested = MAKEWORD(2, 2);
	wsaerr = WSAStartup(wVersionRequested, &wsaData);
	if (wsaerr != 0)
	{
		std::cout << "The winsock dll not found!" << std::endl;
		return -1;
	}
	else
	{
		std::cout << "The winsock dll was found!" << "\n" << "The status: " << wsaData.szSystemStatus << std::endl;
	}

	// set up the socket configuration
	SOCKET connSocket = INVALID_SOCKET;
	connSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (connSocket == INVALID_SOCKET)
	{
		std::cout << "Error at socket(); " << WSAGetLastError() << std::endl;
		WSACleanup();
		return - 1;
	}
	else
	{
		std::cout << "socket() is OK!" << std::endl;
	}

	// bind the specifics for the socket with its specific configuration
	sockaddr_in service;
	service.sin_family = AF_INET;
	InetPton(AF_INET, L"127.0.0.1", &service.sin_addr.s_addr);
	service.sin_port = htons(gPort);
	if (connect(connSocket, (SOCKADDR*)&service, sizeof(service)) == SOCKET_ERROR)
	{
		std::cout << "failed to connect to server" << WSAGetLastError() << std::endl;
		closesocket(connSocket);
		WSACleanup();
		return -1;
	}
	else
	{
		std::cout << "Client successfully connected to server!" << std::endl;
		std::cout << "Can start sending data!" << std::endl;
	}

	// setup the game window
	gWnd.create({ 1600U,900U,32U }, "Networking", sf::Style::Default);
	
	
	// GAME LOOP
	sf::Clock frameTimer;
	while (gWnd.isOpen())
	{
		// window event handling
		sf::Event e;
		while (gWnd.pollEvent(e))
		{
			if (e.type == sf::Event::Closed)
			{
				gWnd.close();
			}
		}
		// update flags per input signals indicated in this function
		input();

		// update the whole game world needed for the game portion seen and used
		gTime = frameTimer.restart().asSeconds();
		update();

		// rendering of the window
		gWnd.clear(sf::Color(47, 147, 247, 255));

		render();

		gWnd.display();
	}



	// Clean up program and exit

	//close any open sockets
	if (connSocket != INVALID_SOCKET)
	{
		closesocket(connSocket);
	}

	WSACleanup();

	return 0;
}

void input()
{

}

void update()
{}

void render()
{}

