// clientMain.cpp
//   This function serves as the "main" function for the client-side
#include "NIM.h"
#include <iostream>
#include <string>
#include <WinSock2.h>

int clientMain(int argc, char *argv[], std::string playerName)
{
	std::string host;
	std::string port;
	ServerStruct serverArray[MAX_SERVERS];

	SOCKET s = connectsock("","","udp");	// Create a socket  (Don't need to designate a host or port for UDP)

	// Find all NIM servers on our subnet
	std::cout << std::endl << "Looking for NIM servers ... " << std::endl;

	char broadcastAddress[v4AddressSize];
	char myIPAddress[v4AddressSize];
	int status = getIPAddressInfo(myIPAddress, broadcastAddress);
	if (status != 0) {
		std::cerr << "No interface found that supports broadcasting." << std::endl;
		return -1;
	}
	int numServers = getServers(s, broadcastAddress, NIM_UDPPORT, serverArray);

	if (numServers == 0) {
		std::cout << std::endl << "Sorry.  No NIM servers were found.  Try again later." << std::endl << std::endl;
	} 
	else 
	{
		// Display the list of servers found
		std::cout << std::endl << "Found NIM server";
		if (numServers == 1) {
			std::cout << ":" << "  " << serverArray[0].name << std::endl;
		} else {
			std::cout << "s:" << std::endl;
			for (int i=0; i<numServers; i++) {
				std::cout << "  " << i+1 << " - " << serverArray[i].name << std::endl;
			}
			std::cout << std::endl << "  " << numServers+1 << " - QUIT" << std::endl;
		}
		std::cout << std::endl;

		// Allow user to select someone to challenge
		int answer = 0;
		std::string answerStr;
		if (numServers == 1) {
			std::cout << "Do you want to challenge " << serverArray[0].name << "? ";
			std::getline(std::cin, answerStr);
			if (answerStr[0] == 'y' || answerStr[0] == 'Y') answer = 1;
		} else if (numServers > 1) {
			std::cout << "Who would you like to challenge (1-" << numServers+1 << ")? ";
			std::getline(std::cin,answerStr);
			answer = atoi(answerStr.c_str());
			if (answer > numServers) answer = 0;
		}
			
		if (answer >= 1 && answer <= numServers) 
		{
			// Extract the opponent's info from the server[] array
			std::string serverName;
			serverName = serverArray[answer-1].name;		// Adjust for 0-based array
			host = serverArray[answer-1].host;
			port = serverArray[answer-1].port;

			// Append playerName to the NIM_CHALLENGE string & send a challenge to host:port
			char buffer[MAX_SEND_BUF];
			strcpy_s(buffer,NIM_CHALLENGE);
			strcat_s(buffer,playerName.c_str());
			int len = UDP_send(s, buffer, strlen(buffer)+1,(char*)host.c_str(), (char*)port.c_str());

			// Wait 15 sec for response to challenge
			int status2 = wait(s, 15, 0);

			// Recieve "yes" or "no" response to challenge request
			char recvBuffer2[MAX_RECV_BUF + 1];
			int numBytesRecv = UDP_recv(s, recvBuffer2, sizeof(recvBuffer2) - 1, (char*)host.c_str(), (char*)port.c_str());

			if (recvBuffer2[0] == 'y' || recvBuffer2[0] == 'Y' && status2 > 0)
			{
				// Send "Great!" to other player if they accept the challenge request
				UDP_send(s, NIM_ACCEPT, strlen(NIM_ACCEPT) + 1, (char*)host.c_str(), (char*)port.c_str());

				// Play the game.  You are the first player
				int winner = playNim(s, serverName, host, port, PLAYER1);
			}
			else
			{
				/*
				char choiceStr[80], newline;
				int choice = 0;
				const int QUIT2 = 2;
				do {
					std::cout << std::endl << std::endl << std::endl;
					std::cout << "Choose an option:" << std::endl;
					std::cout << "   1 - Challenge someone else" << std::endl;
					std::cout << "   2 - Quit" << std::endl;
					std::cout << "Enter 1 or 2: ";
					std::cin >> choiceStr; std::cin.get(newline);
					choice = atoi(choiceStr);

					switch (choice) {
					case 1: clientMain(argc, argv, playerName); break;	// Call the client main function
					case QUIT2: QUIT = 3; break;
					default: std::cout << std::endl << "Please enter either a digit of 1 or 2." << std::endl;
					}
				} while (choice != QUIT2);
				*/
			}
			
		}
	}

	closesocket(s);
	return 0;
}