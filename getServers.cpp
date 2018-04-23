#include "NIM.h"
#include <WinSock2.h>
#include <iostream>
#include <string>

using namespace std;

// getServers.cpp
//  Given a socket, a broadcast address and a port number, this function returns the number
//  of NIM servers as well as an array of structs.  Each struct contains the name, IP_Address and 
//  port number of a remote TicTacToe server.
//  port number of a remote NIM server.


//	IN parameters:
//	SOCKET s				= Allocated UDP socket handle that can be used for communcations
//	char *broadcastAddress	= The broadcast address that should be used for the current LAN
//	char *broadcastPort		= The port that should be used for the broadcast

//	OUT parameters:
//	ServerStruct serverArray[]	= An array that contains the structs.  The members of each struct
//							  contain the name, IP Address and port number of a remote NIM server.
//	numServers				= Number of elements in serverArray[]

//	Function return value	= Number of remote NIM servers (size of serverArray[])

int getServers(SOCKET s, char *broadcastAddress, char *broadcastPort, ServerStruct serverArray[])
{
	// This function returns the number of NIM servers that may be found within the current Broadcast Domain.
	// The last parameter, serverArray[], will contain all of the servers' names, IP addresses and port numbers.

	int numServers = 0;

	//send 'who?' to any address receiving
	int numBytesSent = UDP_send(s, NIM_QUERY, strlen(NIM_QUERY) + 1, broadcastAddress, NIM_UDPPORT);


	
	int status = wait(s,2,0);
	if (status > 0) 
	{
		int len = 1;
		char recvBuffer[MAX_RECV_BUF + 1];
		char host[v4AddressSize];
		char port[portNumberSize];

		//receive a broadcast message	
		int numBytesRecv = UDP_recv(s, recvBuffer, sizeof(recvBuffer) - 1, host, port);

		int status2 = wait(s, 15, 0);

		char recvBuffer2[MAX_RECV_BUF + 1];

		numBytesRecv = UDP_recv(s, recvBuffer2, sizeof(recvBuffer2) - 1, host, port);

		if (strcmp(recvBuffer2, "Yes") == 0) 
		{
			numBytesSent = UDP_send(s, NIM_ACCEPT, strlen(NIM_ACCEPT) + 1, host, NIM_UDPPORT);

			string name;
			while (status2 > 0 && len > 0)
			{
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
