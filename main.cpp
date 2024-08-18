#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>

#include <Winsock2.h>
#include <Ws2tcpip.h>
#include <iostream>
#include <core/Cfg.h>
#include <core/globals.h>
#include <level/LayeredBackground.h>
#include <memory>
#include <core/clientspecific.h>
#include <core/interrelated.h>
#include <sstream>
#include <CidNet64.h>


const int INPUT_BFR_SIZE = 9;
SOCKADDR_IN ThisSenderInfo;

sf::Texture* playerTex{};
std::map<std::string, std::vector<sf::IntRect> > animMap{};
std::string currentAnim{"idle"};
int index = 0;
sf::Sprite playerSpr{};

sf::Texture* player2Tex{};
std::map<std::string, std::vector<sf::IntRect> > animMap2{};
std::string currentAnim2{ "idle" };
int index2 = 0;
sf::Sprite player2Spr{};

float gElapsed;

int gPort = 55555;

int gClientID = 10;

//sf::Font gFont;

SOCKET connSocket;

std::unique_ptr<LayeredBackground> bg2;


std::string input();
void update();
void render();

wchar_t* stringToWChar(const std::string& str);
void setupAnims();

int main()
{

	Cfg::Initialize();
	std::string result;
	std::cout << "Type in ip address of server: " << std::endl;
	std::cin >> result;

	wchar_t* wstr = stringToWChar(result);

	if (!cid::net::initialize())
	{
		return 1;
	}
	else
	{
		std::cout << "Success on initialization of winsock!" << std::endl;
	}


	// set up the socket configuration
	cid::Socket connSocket{ cid::IPVersion::IPv4, INVALID_SOCKET };
	{
		auto result = connSocket.Create();
		if (result != cid::CResult::C_Success)
		{
			std::cout << "Error at socket creation for main connection listener" << std::endl;
			cid::net::shutdown();
			return 0;
		}
		else
		{
			std::cout << "socket connector created" << std::endl;
		}
	}


	if (connSocket.Connect(cid::IPEndpoint(result.c_str(), 55555)) == cid::CResult::C_Success)
	{
		std::cout << "connected to server" << std::endl;
	}
	else
	{
		std::cout << "unable to connect to server" << std::endl;
	}


	std::cout << "received input data from client1" << std::endl;

	{
		char buffer[2];
		cid::CResult result = cid::CResult::C_Success;
		result = connSocket.RecvAll(buffer, 2);
		if (result != cid::CResult::C_Success)
		{
			connSocket.Close();
			cid::net::shutdown();
			return 1;
		}
		std::cout << "received clientID from server" << buffer << std::endl;
		gClientID = atoi(buffer);
	}


	
	Cfg::Initialize();

	wndw::CreateSFMLWindow("Networking Adventure", 1600U, 900U);

	setupAnims();

	bg2 = std::make_unique<LayeredBackground>(Cfg::textures.get((int)Cfg::Textures::BG_Par1_1), Cfg::textures.get((int)Cfg::Textures::BG_Par1_2), Cfg::textures.get((int)Cfg::Textures::BG_Par1_3), Cfg::textures.get((int)Cfg::Textures::BG_Par1_4), Cfg::textures.get((int)Cfg::Textures::BG_Par1_5), .25f, .5f, .75f, 1.f, 1.f);

	
	// GAME LOOP
	sf::Clock frameTimer;
	gElapsed = 0.f;
	while (gWnd.isOpen())
	{
		std::string tmp = input();
		char mystr[9];
		tmp.copy(mystr, 8);
		mystr[8] = '\0';

		char sendbuf[9];
		strcpy_s(sendbuf, mystr);

		std::cout << "about to send to server the input data" << std::endl;

		{
			cid::CResult result = cid::CResult::C_Success;

				result = connSocket.SendAll(sendbuf, 9);
				if (result != cid::CResult::C_Success)
				{
					connSocket.Close();
					cid::net::shutdown();
					return 1;
				}
				std::cout << "Attempting to send chunk of data..." << std::endl;
			
		}
		std::cout << "sent to server the input data" << std::endl;






		std::cout << "waiting to receive world data from server" << std::endl;

			{

				char recvbuff[15];
				cid::CResult result = cid::CResult::C_Success;

					result = connSocket.RecvAll(recvbuff, 15);
					if (result != cid::CResult::C_Success)
					{
						connSocket.Close();
						cid::net::shutdown();
						return 1;
					}
			
				recvbuff[14] = '\0';


				
				std::cout << "Got message from server: " << recvbuff << std::endl;

				std::string p1X{ "0000" }, p1Y{ "000" }, p2X{ "0000" }, p2Y{ "000" };
				for (int i = 0; i < 4; i++)
				{
					p1X[i] = recvbuff[i];
				}
				for (int i = 0; i < 3; i++)
				{
					p1Y[i] = recvbuff[i + 4];
				}
				for (int i = 0; i < 4; i++)
				{
					p2X[i] = recvbuff[i + 7];
				}
				for (int i = 0; i < 3; i++)
				{
					p2Y[i] = recvbuff[i + 11];
				}

				if (gClientID == 1)
				{
					playerSpr.setPosition({ (float)stoi(p1X), (float)stoi(p1Y) });
					player2Spr.setPosition({ (float)stoi(p2X), (float)stoi(p2Y) });
				}
				else
				{
					playerSpr.setPosition({ (float)stoi(p2X), (float)stoi(p2Y) });
					player2Spr.setPosition({ (float)stoi(p1X), (float)stoi(p1Y) });
				}
			}
			std::cout << "received world data from the server" << std::endl;

		
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
		

		// update the whole game world needed for the game portion seen and used
		gTime = frameTimer.restart().asSeconds();
	
		// rendering of the window
		gWnd.clear(sf::Color(47, 147, 247, 255));

		render();

		gWnd.display();

	
	}





	//close any open sockets
	if (connSocket.GetHandle() != INVALID_SOCKET)
	{
		cid::CResult result = connSocket.Close();
		if (result != cid::CResult::C_Success)
		{
			std::cout << "Trouble with closing the socket with the server" << std::endl;
			cid::net::shutdown();
			return 1;
		}
	}

	cid::net::shutdown();

	return 0;
}

std::string input()
{
	std::string s = "00000000";

	if (sf::Keyboard::isKeyPressed(sf::Keyboard::W))
	{
		s[0] = '1';
	}
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::S))
	{
		s[1] = '1';
	}
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::A))
	{
		s[2] = '1';
	}
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::D))
	{
		s[3] = '1';
	}
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Space))
	{
		s[4] = '1';
	}
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::C))
	{
		s[5] = '1';
	}
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Enter))
	{
		s[6] = '1';
	}
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Escape))
	{
		s[7] = '1';
	}
	return s;
}

void update()
{
}


void render()
{
	bg2->render();

	gWnd.draw(playerSpr);
	gWnd.draw(player2Spr);

	bg2->renderForeground();
	

}

wchar_t* stringToWChar(const std::string& str) {
	int size_needed = MultiByteToWideChar(CP_UTF8, 0, str.c_str(), (int)str.size(), NULL, 0);
	wchar_t* wstr = new wchar_t[size_needed + 1];
	MultiByteToWideChar(CP_UTF8, 0, str.c_str(), (int)str.size(), wstr, size_needed);
	wstr[size_needed] = L'\0'; // Null-terminate the wide string
	return wstr;
}

void setupAnims()
{
	animMap.clear();

	animMap.emplace(std::pair("idle", std::vector<sf::IntRect>{}));
	animMap["idle"].clear();
	for (int i = 0; i < 11; i++)
	{

		animMap["idle"].push_back({ {i * 96, 0},{96,96} });
	}
	animMap.emplace(std::pair("idle2", std::vector<sf::IntRect>{}));
	animMap["idle2"].clear();
	for (int i = 0; i < 11; i++)
	{

		animMap["idle2"].push_back({ {i * 96, 96 * 1},{96,96} });
	}
	animMap.emplace(std::pair("idle3", std::vector<sf::IntRect>{}));
	animMap["idle3"].clear();
	for (int i = 0; i < 11; i++)
	{

		animMap["idle3"].push_back({ {i * 96, 96 * 2},{96,96} });
	}
	animMap.emplace(std::pair("idle4", std::vector<sf::IntRect>{}));
	animMap["idle4"].clear();
	for (int i = 0; i < 11; i++)
	{

		animMap["idle4"].push_back({ {i * 96, 96 * 3},{96,96} });
	}
	animMap.emplace(std::pair("idle5", std::vector<sf::IntRect>{}));
	animMap["idle5"].clear();
	for (int i = 0; i < 11; i++)
	{

		animMap["idle5"].push_back({ {i * 96, 96 * 4},{96,96} });
	}
	animMap.emplace(std::pair("idle6", std::vector<sf::IntRect>{}));
	animMap["idle6"].clear();
	for (int i = 0; i < 11; i++)
	{

		animMap["idle6"].push_back({ {i * 96, 96 * 5},{96,96} });
	}
	animMap.emplace(std::pair("idle7", std::vector<sf::IntRect>{}));
	animMap["idle7"].clear();
	for (int i = 0; i < 11; i++)
	{

		animMap["idle7"].push_back({ {i * 96, 96 * 6},{96,96} });
	}
	animMap.emplace(std::pair("idle8", std::vector<sf::IntRect>{}));
	animMap["idle8"].clear();
	for (int i = 0; i < 11; i++)
	{

		animMap["idle8"].push_back({ {i * 96, 96 * 7},{96,96} });
	}
	animMap.emplace(std::pair("idle9", std::vector<sf::IntRect>{}));
	animMap["idle9"].clear();
	for (int i = 0; i < 11; i++)
	{

		animMap["idle9"].push_back({ {i * 96, 96 * 8},{96,96} });
	}

	animMap.emplace(std::pair("idle10", std::vector<sf::IntRect>{}));
	animMap["idle10"].clear();
	for (int i = 0; i < 11; i++)
	{

		animMap["idle10"].push_back({ {i * 96, 96 * 9},{96,96} });
	}
	animMap.emplace(std::pair("idle11", std::vector<sf::IntRect>{}));
	animMap["idle11"].clear();
	for (int i = 0; i < 11; i++)
	{

		animMap["idle11"].push_back({ {i * 96, 96 * 10},{96,96} });
	}
	playerSpr.setTexture(Cfg::textures.get((int)Cfg::Textures::PlayerSheet));
	playerSpr.setTextureRect(animMap[currentAnim].at(index));
	if (gClientID == 1)
	{
		playerSpr.setPosition({ 50.f,600.f });
	}
	else
	{
		playerSpr.setPosition({ 350.f,600.f });

	}


	animMap2.emplace(std::pair("idle", std::vector<sf::IntRect>{}));
	animMap2["idle"].clear();
	for (int i = 0; i < 11; i++)
	{

		animMap2["idle"].push_back({ {i * 96, 0},{96,96} });
	}
	animMap2.emplace(std::pair("idle2", std::vector<sf::IntRect>{}));
	animMap2["idle2"].clear();
	for (int i = 0; i < 11; i++)
	{

		animMap2["idle2"].push_back({ {i * 96, 96 * 1},{96,96} });
	}
	animMap2.emplace(std::pair("idle3", std::vector<sf::IntRect>{}));
	animMap2["idle3"].clear();
	for (int i = 0; i < 11; i++)
	{

		animMap2["idle3"].push_back({ {i * 96, 96 * 2},{96,96} });
	}
	animMap2.emplace(std::pair("idle4", std::vector<sf::IntRect>{}));
	animMap2["idle4"].clear();
	for (int i = 0; i < 11; i++)
	{

		animMap2["idle4"].push_back({ {i * 96, 96 * 3},{96,96} });
	}
	animMap2.emplace(std::pair("idle5", std::vector<sf::IntRect>{}));
	animMap2["idle5"].clear();
	for (int i = 0; i < 11; i++)
	{

		animMap2["idle5"].push_back({ {i * 96, 96 * 4},{96,96} });
	}
	animMap2.emplace(std::pair("idle6", std::vector<sf::IntRect>{}));
	animMap2["idle6"].clear();
	for (int i = 0; i < 11; i++)
	{

		animMap2["idle6"].push_back({ {i * 96, 96 * 5},{96,96} });
	}
	animMap2.emplace(std::pair("idle7", std::vector<sf::IntRect>{}));
	animMap2["idle7"].clear();
	for (int i = 0; i < 11; i++)
	{

		animMap2["idle7"].push_back({ {i * 96, 96 * 6},{96,96} });
	}
	animMap2.emplace(std::pair("idle8", std::vector<sf::IntRect>{}));
	animMap2["idle8"].clear();
	for (int i = 0; i < 11; i++)
	{

		animMap2["idle8"].push_back({ {i * 96, 96 * 7},{96,96} });
	}
	animMap2.emplace(std::pair("idle9", std::vector<sf::IntRect>{}));
	animMap2["idle9"].clear();
	for (int i = 0; i < 11; i++)
	{

		animMap2["idle9"].push_back({ {i * 96, 96 * 8},{96,96} });
	}

	animMap2.emplace(std::pair("idle10", std::vector<sf::IntRect>{}));
	animMap2["idle10"].clear();
	for (int i = 0; i < 11; i++)
	{

		animMap2["idle10"].push_back({ {i * 96, 96 * 9},{96,96} });
	}
	animMap2.emplace(std::pair("idle11", std::vector<sf::IntRect>{}));
	animMap2["idle11"].clear();
	for (int i = 0; i < 11; i++)
	{

		animMap2["idle11"].push_back({ {i * 96, 96 * 10},{96,96} });
	}
	player2Spr.setTexture(Cfg::textures.get((int)Cfg::Textures::PlayerSheet));
	player2Spr.setTextureRect(animMap2[currentAnim2].at(index2));
	if (gClientID == 2)
	{
		player2Spr.setPosition({ 50.f,600.f });
	}
	else
	{
		player2Spr.setPosition({ 350.f,600.f });

	}
}
