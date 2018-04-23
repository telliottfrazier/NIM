// NIM.cpp

// This program plays NIM with other computers across the network using the UDP protocol.

#include "NIM.h"
#include <iostream>
#include <string>
#include <WinSock2.h>
#pragma comment (lib, "ws2_32.lib")

int main (int argc, char *argv[])
{
	WSADATA wsaData;
	int iResult = WSAStartup (MAKEWORD(2,2), &wsaData);
	if (iResult != 0) {
		std::cout << "Unable to initialize Windows Socket library." << std::endl;
		return 0;
	}
	
	std::string playerName;
	std::cout << "What is your name? ";
	std::getline(std::cin, playerName);

	char choiceStr[80], newline;
	int choice = 0;
	const int QUIT = 3;
	do {
		std::cout << std::endl << std::endl << std::endl;
		std::cout << "Choose an option:" << std::endl;
		std::cout << "   1 - Host a game of NIM" << std::endl;
		std::cout << "   2 - Challenge someone else" << std::endl;
		std::cout << "   3 - Quit" << std::endl;
		std::cout << "Enter 1, 2 or 3: ";
		std::cin >> choiceStr; std::cin.get(newline);
		choice = atoi(choiceStr);

		switch (choice) {
		case 1:	serverMain(argc,argv,playerName); break;	// Call the server main function
		case 2: clientMain(argc,argv,playerName); break;	// Call the client main function
		case QUIT: break;
		default: std::cout << std::endl << "Please enter a digit between 1 and 3." << std::endl;
		}
	} while (choice != QUIT);

	WSACleanup();
}

