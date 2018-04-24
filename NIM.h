// nim.h    Header file for Nim project
#include <winsock2.h>
#include <string>

#define NIM_QUERY	"Who?"
#define NIM_NAME	"Name="
#define NIM_CHALLENGE	"Player="
#define NIM_ACCEPT	"Great!"


static char NIM_UDPPORT[] = "29333";

const int v4AddressSize = 16;
const int portNumberSize = 10;
const int WAIT_TIME = 120;
const int MAX_SEND_BUF = 2048;
const int MAX_RECV_BUF = 2048;
const int MAX_SERVERS = 100;
const int MAX_NAME = 100;
const int PLAYER1 = 1;
const int PLAYER2 = 2;
const int ABORT = -1;

struct ServerStruct {
	std::string name;
	std::string host;
	std::string port;
};

SOCKET connectsock(const char*, const char*, const char*);
SOCKET passivesock(const char*, const char*);
int UDP_recv(SOCKET, char*, int, char*, char*);
int UDP_send(SOCKET, char*, int, const char*, const char*);
int wait(SOCKET, int, int);
char* timestamp();
int getServers(SOCKET, char*, char*, ServerStruct[]);
int playNim(SOCKET, std::string, std::string, std::string, int);
int serverMain(int, char *argv[], std::string);
int clientMain(int, char *argv[], std::string);
int getIPAddressInfo(char*, char*);