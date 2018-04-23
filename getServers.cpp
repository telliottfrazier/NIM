#include "NIM.h"
#include <WinSock2.h>
#include <iostream>
#include <string>

using namespace std;

// getServers.cpp
//  Given a socket, a broadcast address and a port number, this function returns the number
//  of TicTacToe servers as well as an array of structs.  Each struct contains the name, IP_Address and 
//  port number of a remote TicTacToe server.

//	IN parameters:
//	SOCKET s				= Allocated UDP socket handle that can be used for communcations
//	char *broadcastAddress	= The broadcast address that should be used for the current LAN
//	char *broadcastPort		= The port that should be used for the broadcast

//	OUT parameters:
//	ServerStruct serverArray[]	= An array that contains the structs.  The members of each struct
//							  contain the name, IP Address and port number of a remote TicTacToe server.
//	numServers				= Number of elements in serverArray[]

//	Function return value	= Number of remote TicTacToe servers (size of serverArray[])

int getServers(SOCKET s, char *broadcastAddress, char *broadcastPort, ServerStruct serverArray[])
{
	// This function returns the number of TicTacToe servers that may be found within the current Broadcast Domain.
	// The last parameter, serverArray[], will contain all of the servers' names, IP addresses and port numbers.

	int numServers = 0;

	//send 'who?' to any address receiving
	int numBytesSent = UDP_send(s, NIM_QUERY, strlen(NIM_QUERY) + 1, broadcastAddress, NIM_UDPPORT);


	// Receive incoming UDP datagrams (with a maximum of 2 second wait before each UDP_recv() function call
	// As you read datagrams, if they start with the prefix: NIM_NAME, parse out the server's name
	// and add the name, host address and port number to serverArray[].  Don't forget to increment numServers.
	int status = wait(s,2,0);
	if (status > 0) 
	{
		int len = 1;
		char recvBuffer[MAX_RECV_BUF + 1];
		char host[v4AddressSize];
		char port[portNumberSize];

		//receive a broadcast message	
		int numBytesRecv = UDP_recv(s, recvBuffer, sizeof(recvBuffer) - 1, host, port);

		char challengeName[MAX_RECV_BUF - 1] = "";
		std::cout << "What is your name?";
		std::cin >> challengeName;
		char *challenge;
		strcat(NIM_CHALLENGE, challengeName);
		int numBytesSent = UDP_send(s, challenge, strlen(challenge) + 1, broadcastAddress, NIM_UDPPORT);

		int status2 = wait(s, 15, 0);

		int numBytesRecv = UDP_recv(s, recvBuffer, sizeof(recvBuffer) - 1, host, port);

		if (strcmp(recvBuffer, "Yes") != 0) 
		{

			string name;
			while (status > 0 && len > 0)
			{
				/****
				Task 4b: Inside this while loop, extract a response, which should be a C-string that looks like "Name=some server's name".
				If the response doesn't begin with the characters, "Name=", ignore it.
				If it does begin with the characters, "Name=", parse the actual name that follows and
				(i) assign that name to the array of structs, serverArray[numServers].name
				(ii) assign the IP Address from which the response originated to serverArray[numServers].host
				(iii) assign the server's port number to serverArray[numServers].port
				(iv) increment numServers
				****/

				string recvStr(recvBuffer);
				if (recvStr.substr(0, 5) == "Name=")
				{
					name = recvStr.substr(5);
					serverArray[numServers].name = name;
					serverArray[numServers].host = host;
					serverArray[numServers].port = port;
					numServers++;
				}
			}
		}
		else
		{
			// Now, we'll see if there is another response.
			status = wait(s, 2, 0);
			if (status > 0)
				len = UDP_recv(s, recvBuffer, MAX_RECV_BUF, host, port);
		}
	}
	return numServers;
}
