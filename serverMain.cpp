// server_main.cpp
//   This function serves as the "main" function for the server-side
#include "NIM.h"
#include <iostream>
#include <string>
#include <WinSock2.h>

int serverMain(int argc, char *argv[], std::string playerName)
{
	SOCKET s;
	char buffer[MAX_RECV_BUF];
	std::string host;
	std::string port;
	char responseStr[MAX_SEND_BUF];
	
	s = passivesock(NIM_UDPPORT,"udp");

	std::cout << std::endl << "Waiting for a challenge..." << std::endl;
	int len = UDP_recv(s, buffer, MAX_RECV_BUF, (char*)host.c_str() , (char*)port.c_str());
	std::cout << timestamp() << " - Received: " << buffer << std::endl;

	bool finished = false;
	while (!finished) {
		if ( strcmp(buffer,NIM_QUERY) == 0) {
			// Respond to name query
			strcpy_s(responseStr,NIM_NAME);
			strcat_s(responseStr,playerName.c_str());
			UDP_send(s, responseStr, strlen(responseStr)+1, (char*)host.c_str(), (char*)port.c_str());
			std::cout << timestamp() << " - Sending: " << responseStr << std::endl;

		} else if ( strncmp(buffer,NIM_CHALLENGE,strlen(NIM_CHALLENGE)) == 0) {
			// Received a challenge  
			char *startOfName = strstr(buffer,NIM_CHALLENGE);
			if (startOfName != NULL) {
				std::cout << std::endl << "You have been challenged by " << startOfName+strlen(NIM_CHALLENGE) << std::endl;
				
				std::string answerResponse;
				std::cout << "Do you want to challenge " << playerName << "? ";
				std::getline(std::cin, answerResponse);
				if (answerResponse[0] == 'y' || answerResponse[0] == 'Y')
				{
					// Accept challenge request by the client
					std::string reply = "YES";
					UDP_send(s, (char*)reply.c_str(), strlen((char*)reply.c_str()) + 1, (char*)host.c_str(), (char*)port.c_str());
					std::cout << timestamp() << " - Sending: " << (char*)reply.c_str() << std::endl;

					// Wait 2 seconds for client to send "Great!"
					int status3 = wait(s, 2, 0);

					// Attempt to recieve the "Great!" response from the client
					char greatResponse[MAX_RECV_BUF];
					UDP_recv(s, greatResponse, sizeof(greatResponse) - 1, (char*)host.c_str(), (char*)port.c_str());
					std::cout << timestamp() << " - Received: " << greatResponse << std::endl;
					
					// If "Great!" string is recieved from the client
					if (status3 > 0)
					{
						int winner = playNim(s, (char*)playerName.c_str(), (char*)host.c_str(), (char*)port.c_str(), PLAYER2);
						finished = true;
					}
				}
				else
				{
					// Reject challenge request by the client
					std::string reply = "NO";
					UDP_send(s, (char*)reply.c_str(), strlen((char*)reply.c_str()) + 1, (char*)host.c_str(), (char*)port.c_str());
					std::cout << timestamp() << " - Sending: " << (char*)reply.c_str() << std::endl;
				}
			}
		}

		if (!finished) {
			char previousBuffer[MAX_RECV_BUF];		strcpy_s(previousBuffer,buffer);
			std::string previousHost;				previousHost = host;
			std::string previousPort;				previousPort = port;

			// Check for duplicate datagrams (can happen if broadcast enters from multiple ethernet connections)
			bool newDatagram = false;
			int status = wait(s,1,0);	// We'll wait a second to see if we receive another datagram
			while (!newDatagram && status > 0) {
				len = UDP_recv(s, buffer, MAX_RECV_BUF, (char*)host.c_str(), (char*)port.c_str());
				std::cout << timestamp() << " - Received: " << buffer << std::endl;
				if (strcmp(buffer,previousBuffer)==0 &&		// If this datagram is identical to previous one, ignore it.
					host == previousHost && 
					port == previousPort) {
						status = wait(s,1,0);			// Wait another second (still more copies?)
				} else {
					newDatagram = true;		// if not identical to previous one, keep it!
				}
			}

			// If we waited one (or more seconds) and received no new datagrams, wait for one now.
			if (!newDatagram ) {
				len = UDP_recv(s, buffer, MAX_RECV_BUF, (char*)host.c_str(), (char*)port.c_str());
				std::cout << timestamp() << " - Received: " << buffer << std::endl;
			}
		}
	}
	closesocket(s);

	return 0;
}