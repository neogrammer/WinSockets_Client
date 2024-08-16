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

//struct PlayerData
//{
//	int xpos{};
//	int ypos{};
//};
//
//struct BallData
//{
//	int xpos{};
//	int ypos{};
//};
//
//struct FrameData
//{
//	int ypos{};
//};
//
//struct WorldData
//{
//	int yourYPos{};
//	int otherYPos{};
//	int ballXPos{};
//	int ballYPos{};
//};

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
std::unique_ptr<LayeredBackground> bg1;

//// paddle and ball textures
//sf::Texture gPaddleTex;
//sf::Texture gBallTex;
//
//
//
//PlayerData myData{};
//PlayerData otherPlayerData{};
//BallData ballData{};
//
//FrameData frameData{};
//WorldData worldData{};

//bool gUpPressed{ false };
//bool gDownPressed{ false };

void input(ClientFrameInput& data_);
void update();
void render();

wchar_t* stringToWChar(const std::string& str);

int main()
{
	
	Cfg::Initialize();
	std::string result;
	std::cout << "Type in ip address of server: " << std::endl;
	std::cin >> result;

	wchar_t* wstr = stringToWChar(result);


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
	connSocket = INVALID_SOCKET;
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
	InetPton(AF_INET, wstr, &service.sin_addr.s_addr);
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

	{
		//wait for server to send initial data such as what this clients ID will be for this game session
		char buffer[2];
		int byteCount = recv(connSocket, buffer, 2, 0);
		if (byteCount < 0)
		{
			std::cout << "Unable to receive data from server supplying the Client ID for this machine" << std::endl;
			closesocket(connSocket);
			WSACleanup();
			return 0;
		}
		else
		{
			std::cout << "Data received successfully from server supplying the players Client ID for this machine" << std::endl;

			gClientID = atoi(buffer);
		}
	}
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

		animMap["idle10"].push_back({ {i * 96, 96*9},{96,96} });
	}
	animMap.emplace(std::pair("idle11", std::vector<sf::IntRect>{}));
	animMap["idle11"].clear();
	for (int i = 0; i < 11; i++)
	{

		animMap["idle11"].push_back({ {i * 96, 96*10},{96,96} });
	}
	playerSpr.setTexture(Cfg::textures.get((int)Cfg::Textures::PlayerSheet));
	playerSpr.setTextureRect(animMap[currentAnim].at(index));
	if (gClientID == 1)
	{
		playerSpr.setPosition({30.f,700.f});
	}
	else
	{
		playerSpr.setPosition({ 150.f,700.f });

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
		player2Spr.setPosition({ 30.f,700.f });
	}
	else
	{
		player2Spr.setPosition({ 150.f,700.f });

	}

	// int send ( SOCKET, const char* buf //MESSAGE//, int len //Length of Message Array(individual characters)//, int flags //DEFAULT OK- pass 0//); 
    //  returns number of bytes sent, non-zero is ideal here

/*	if (gClientID == 1)
		myData.xpos = 40;
	else
		myData.xpos = 1510;

	myData.ypos = 380;

	if (gClientID == 1)
		otherPlayerData.xpos = 1510;
	else
		otherPlayerData.xpos = 40;

	otherPlayerData.ypos = 380;

	ballData.xpos = 785;
	ballData.ypos = 435;

	worldData.yourYPos = 380;
	worldData.ballXPos = 785;
	worldData.ballYPos = 435;
	worldData.otherYPos = 380;

	frameData.ypos = 380;

	*/// Invariants of game setup are now configured to begin play
	// example of sending and receiving the objects
	//  SENDING
	{
		ClientFrameInput frameInputData = {};
		input(frameInputData);

		unsigned long long byteCount = 0Ui64;
		while (byteCount < sizeof(frameInputData))
			byteCount = send(connSocket, (char*)&frameInputData, sizeof(frameInputData), 0);
	}
	//sent
	
	// frameData.ypos = myData.ypos;
	// byteCount = send(connSocket, (char*)&frameData, sizeof(frameData), 0);
	// RECEIVING;
	// byteCount = recv(connSocket, (char *) &worldData, sizeof(WorldData), 0);
	// otherPlayerData.ypos = worldData.otherYPos;
	// ballData.xpos = worldData.ballXPos;
	// ballData.ypos = worldData.ballYPos;
	// myData.ypos = worldData.yourYPos;
	/////////////////////////////////////////

	// strcpy_s(buffer, "string");  overwrites a char* like char buffer[2];
	// if (strcmp(buffer, "STRING TO MATCH") == 0 ) { // strings are equal // }  can be < 0 or > 0
	
	Cfg::Initialize();

	wndw::CreateSFMLWindow("Networking Adventure", 1600U, 900U);
	// setup the game window
	//gWnd.create({ 1600U,900U,32U }, "Networking", sf::Style::Default);
	//
	//// set assets
	//gFont.loadFromFile("assets/fonts/font1.ttf");
	//gPaddleTex.loadFromFile("assets/textures/paddle.png");
	//gBallTex.loadFromFile("assets/textures/ball.png");
	bg1 = std::make_unique<LayeredBackground>(Cfg::textures.get((int)Cfg::Textures::BG1_1), Cfg::textures.get((int)Cfg::Textures::BG1_2), Cfg::textures.get((int)Cfg::Textures::BG1_3), Cfg::textures.get((int)Cfg::Textures::BG1_4), .25f, .5f, .75f, 1.f );

	
	// GAME LOOP
	sf::Clock frameTimer;
	gElapsed = 0.f;
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
		

		// update the whole game world needed for the game portion seen and used
		gTime = frameTimer.restart().asSeconds();

	
		{

			struct combo
			{
				sf::Vector2f tmp1;
				sf::Vector2f tmp2;
			} data;
			//wait for server to send initial data such as what this clients ID will be for this game session
			char buffer[2];
			sf::Vector2f tmp1;

			int byteCount = recv(connSocket, (char*)&data, sizeof(data), 0);
			if (byteCount < 0)
			{
				std::cout << "Unable to receive data from server supplying the Client ID for this machine" << std::endl;
				closesocket(connSocket);
				WSACleanup();
				return 0;
			}
			else
			{
				std::cout << "Data received successfully from server supplying the players Client ID for this machine" << std::endl;

				gClientID = atoi(buffer);
			}
			playerSpr.setPosition({ data.tmp1.x, data.tmp1.y });
			player2Spr.setPosition({ data.tmp2.x, data.tmp2.y });

		}
	
		// rendering of the window
		gWnd.clear(sf::Color(47, 147, 247, 255));

		render();

		gWnd.display();

		{
			ClientFrameInput frameInputData = {};
			input(frameInputData);

			unsigned long long byteCount = 0Ui64;
			while (byteCount < sizeof(frameInputData))
				byteCount = send(connSocket, (char*)&frameInputData, sizeof(frameInputData), 0);
		}
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

void input(ClientFrameInput& data_)
{

	if (sf::Keyboard::isKeyPressed(sf::Keyboard::W))
	{
		data_.up = true;
	}
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::A))
	{
		data_.left = true;
	}
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::S))
	{
		data_.down = true;
	}
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::D))
	{
		data_.right = true;
	}
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Space))
	{
		data_.attack = true;
	}
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Enter))
	{
		data_.start = true;
	}
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Escape))
	{
		gWnd.close();
	}
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::C))
	{
		data_.run = true;
	}

	/*if (gClientID == 1)
	{
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::W))
		{
			gUpPressed = true;
		}
		else
		{
			gUpPressed = false;
		}
		
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::D))
		{
			gDownPressed = true;
		}
		else
		{
			gDownPressed = false;
		}
	}

	if (gClientID == 2)
	{
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up))
		{
			gUpPressed = true;
		}
		else
		{
			gUpPressed = false;
		}
		
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down))
		{
			gDownPressed = true;
		}
		else
		{
			gDownPressed = false;
		}
	}*/
}

void update()
{
	//// wait for time to update before doing so
	//if (gElapsed >= (1.f / 60.f))
	//{
	//	gElapsed = 0.f;
	//
	//	if (gUpPressed && myData.ypos >= 20)
	//	{
	//		myData.ypos -= 20;
	//	}

	//	if (gDownPressed && myData.ypos <= 740)
	//	{
	//		myData.ypos += 20;
	//	}

	//	//send frame's new updated data
	//	int sentBytes = 0;
	//	char writeBuffer[4] = { '0','0','0','\0' };
	//	std::string yPOS = (std::to_string(myData.ypos));
	//	char strBfr[4];
	//	_itoa_s(myData.ypos, strBfr, _countof(strBfr), 10);
	//	size_t len = strnlen(strBfr, _countof(strBfr));
	//	size_t diff = len - yPOS.length();
	//	for (size_t i = 0; i < yPOS.length(); i++)
	//	{
	//		strBfr[i + diff] = yPOS.at(i);
	//	}
	//	writeBuffer[0] = strBfr[0];
	//	writeBuffer[1] = strBfr[1];
	//	writeBuffer[2] = strBfr[2];
	//	/*if (len < 3)
	//	{
	//		if (len < 2)
	//		{
	//			if (len < 1)
	//			{
	//				writeBuffer[0] = '0';
	//				writeBuffer[1] = '0';
	//				writeBuffer[2] = '0';
	//			}
	//			else
	//			{
	//				writeBuffer[0] = '0';
	//				writeBuffer[1] = '0';
	//				writeBuffer[2] = strBfr[0];
	//			}
	//		}
	//		else
	//		{
	//			writeBuffer[0] = '0';
	//			writeBuffer[1] = strBfr[0];
	//			writeBuffer[2] = strBfr[1];
	//		}		
	//	}
	//	else
	//	{
	//		writeBuffer[0] = strBfr[0];
	//		writeBuffer[1] = strBfr[1];
	//		writeBuffer[2] = strBfr[2];
	//	}*/

	//	sentBytes = send(connSocket, writeBuffer, 4, 0);
	//	if (sentBytes == SOCKET_ERROR)
	//	{
	//		//error
	//	}

	//	int readBytes = 0;
	//	char readBuffer[14];
	//	readBytes = recv(connSocket, readBuffer, 14, 0);
	//	if (readBytes == SOCKET_ERROR)
	//	{
	//		//error
	//	}
	//	else
	//	{
	//		std::string tmp = "000";
	//		tmp.at(0) = readBuffer[0];
	//		tmp.at(1) = readBuffer[1];
	//		tmp.at(2) = readBuffer[2];

	//		int newYPos = stoi(tmp);
	//		myData.ypos = newYPos;

	//		std::string tmp2 = "000";
	//		tmp2.at(0) = readBuffer[3];
	//		tmp2.at(1) = readBuffer[4];
	//		tmp2.at(2) = readBuffer[5];

	//		int newOtherYPos = stoi(tmp2);
	//		otherPlayerData.ypos = newOtherYPos;


	//		std::string tmpBallx = "0000";
	//		tmpBallx.at(0) = readBuffer[6];
	//		tmpBallx.at(1) = readBuffer[7];
	//		tmpBallx.at(2) = readBuffer[8];
	//		tmpBallx.at(3) = readBuffer[9];

	//		int newBallXPos = stoi(tmpBallx);
	//		ballData.xpos = newBallXPos;

	//		std::string tmpBally = "000";
	//		tmpBally.at(0) = readBuffer[10];
	//		tmpBally.at(1) = readBuffer[11];
	//		tmpBally.at(2) = readBuffer[12];

	//		int newBallYPos = stoi(tmpBally);
	//		ballData.ypos = newBallYPos;
	//	}

	//	// data vals are updated, and can now be drawn 
	//}
}

void render()
{
	bg1->render();

	gWnd.draw(playerSpr);
	gWnd.draw(player2Spr);


	//sf::Sprite playerSpr;
	//playerSpr = sf::Sprite{};
	//playerSpr.setPosition({(float)myData.xpos,(float)myData.ypos});
	//playerSpr.setTexture(gPaddleTex);
	//sf::Sprite otherPlayerSpr;
	//otherPlayerSpr = sf::Sprite{};
	//otherPlayerSpr.setPosition({ (float)otherPlayerData.xpos,(float)otherPlayerData.ypos });
	//otherPlayerSpr.setTexture(gPaddleTex);
	//sf::Sprite ballSpr;
	//ballSpr = sf::Sprite{};
	//ballSpr.setPosition({ (float)ballData.xpos,(float)ballData.ypos });
	//ballSpr.setTexture(gBallTex);

	//gWnd.draw(playerSpr);
	//gWnd.draw(otherPlayerSpr);
	//gWnd.draw(ballSpr);


	//// HUD
	//// player data displayed
	//sf::Text playerNumTxt;
	//playerNumTxt.setFont(gFont);
	//playerNumTxt.setCharacterSize(32);
	//playerNumTxt.setFillColor(sf::Color::White);
	//std::string playerNumberStr = "Player ";
	//playerNumberStr.append(std::to_string(gClientID));
	//playerNumTxt.setString(playerNumberStr);

	//gWnd.draw(playerNumTxt);

}

wchar_t* stringToWChar(const std::string& str) {
	int size_needed = MultiByteToWideChar(CP_UTF8, 0, str.c_str(), (int)str.size(), NULL, 0);
	wchar_t* wstr = new wchar_t[size_needed + 1];
	MultiByteToWideChar(CP_UTF8, 0, str.c_str(), (int)str.size(), wstr, size_needed);
	wstr[size_needed] = L'\0'; // Null-terminate the wide string
	return wstr;
}