// playNIM.cpp
// This set of functions are used to actually play the game.
// Play starts with the function: playNIM() which is defined below

#include "NIM.h"
#include <WinSock2.h>
#include <iostream>
#include <string>
#include <random>
#include <iomanip>

using std::cout;
using std::cin;
using std::endl;
using std::random_device;
using std::default_random_engine;
using std::uniform_int_distribution;

using namespace std;

struct Move
{
	int pile;
	int rocks;
};


void parseBoard(char* board, int correctBoard[])
{
	int size = strlen(board);
	int k = 1;
	correctBoard[0] = board[0] - '0';
	for (int i = 1; i < size; i += 2)
	{
		int tens;
		int ones;
		tens = (board[i] - '0') * 10;
		ones = board[i+1] - '0';
		correctBoard[k] = tens + ones;
		k++;
	}
}

// opponentBuff holds "mnn" where m is a single digit ('1' thru '9') that represents a pile number, and nn are 2
// digits("01" through "20") that represent the number of “rocks” to remove from pile m.
void parseMove(char* opponentBuff, Move &opponentMove)
{
	string opponentStr(opponentBuff);
	opponentMove.pile = stoi(opponentStr.substr(0, 1));
	opponentMove.rocks = stoi(opponentStr.substr(1, 2));
}

void encodeBoard(const int board[], char* sendableBoard)
{
	int tens;
	int ones;
	char temp[3];

	_itoa_s(board[0], temp, sizeof(temp), 10);
	sendableBoard[0] = temp[0];

	int k = 1;

	for (int i = 1; k <= board[0]; i += 2)
	{
		tens = board[k] / 10;
		ones = board[k] % 10;

		_itoa_s(tens, temp, sizeof(temp), 10);
		sendableBoard[i] = temp[0];
		_itoa_s(ones, temp, sizeof(temp), 10);
		sendableBoard[i + 1] = temp[0];
		k++;
	}

	sendableBoard[(board[0] * 2) + 1] = '\0';
}

void encodeMove(const Move move, char* sendableBoard)
{
	char temp[3];

	_itoa_s(move.pile, temp, sizeof(temp), 10);
	sendableBoard[0] = temp[0];

	int tens = move.rocks / 10;
	int ones = move.rocks % 10;

	_itoa_s(tens, temp, sizeof(temp), 10);
	sendableBoard[1] = temp[0];
	_itoa_s(ones, temp, sizeof(temp), 10);
	sendableBoard[2] = temp[0];

	sendableBoard[3] = NULL;
}

void initializeBoard(int board[19])
{
	random_device rd;
	default_random_engine engine(rd());
	uniform_int_distribution<int> piles(3, 9);
	uniform_int_distribution<int> rocks(1, 20);

	//Create and send a board.
	//3 to 9 piles with 1 to 20 rocks per pile.
	board[0] = piles(engine);

	for (int i = 1; i <= board[0]; ++i)
	{
		board[i] = rocks(engine);
	}
}

void displayBoard(int *setup) {
	int numOfPiles = setup[0];


	cout << "NIM board:" << endl;
	cout << "--------------------------------------------------------------------------------" << endl;

	for (int i = 0; i < numOfPiles; i++) {
		std::string row = "";

		row += "Rock Pile #";
		row += std::to_string(i + 1);
		row += " ->";
		for (int j = 0; j < setup[i + 1]; j++) {
			row += " *";
		}

		cout << std::setw(60) << std::left << row << "(" << std::setw(2) << setup[i + 1] << ") <- Rock Pile #" << (i + 1) << endl;
	}
	cout << "--------------------------------------------------------------------------------" << endl << endl;
}

bool updateBoard(int board[], Move move)
{
	bool validMove = true;

	if (move.pile > board[0] || move.pile <= 0)
		validMove = false;
	else if (move.rocks > board[move.pile])
		validMove = false;
	else
		board[move.pile] -= move.rocks;

	return validMove;
}

//Checks to see if there are no rocks left on the board.
//If there are no rocks remaining then it returns false,
//	the game is over, and the last person who moved loses.
int checkEndgame(int board[19])
{
	int rocksRemain = -1;

	for (int i = 0; i < board[0] && rocksRemain != 1; ++i)
	{
		if (board[i] > 0)
			rocksRemain = 1;
	}

	
	return rocksRemain;
}

Move getMove(int board[19])
{
	Move move;
	int movePile;
	int moveRock;
	bool validMove = false;

	//Get which pile the user wants to move.
	while (!validMove)
	{
		cout << "Which pile do you want to take from? ";
		cin >> movePile;

		if (movePile >= 1 && movePile <= board[0])
			validMove = true;
		else
		{
			cout << "That is not a valid pile number. " << endl;
			cin.clear();
		}
	}
	validMove = false;
	
	//Get how many rocks the user wants to take.
	while (!validMove)
	{
		cout << "How many rocks do you want to take? ";
		cin >> moveRock;

		if (moveRock >= 1 && moveRock <= board[movePile])
			validMove = true;
		else
			cout << "That is not a valid rock number. " << endl;
	}

	move.pile = movePile;
	move.rocks = moveRock;
	
	return move;
}
//void moveOptions(char decision, Move move, int board[]) {
//	if (decision >= '1' && decision <= '9') {
//		move = getMove(board);
//		updateBoard(board, move);
//	}
//	else if (decision == 'c' || decision == 'C') {
//		std::cout << "chat" << endl;
//	}
//
//	else if (decision == 'f' || decision == 'F') {
//	std:cout << "forfeit" << endl;
//	}
//}

//int playTicTacToe(SOCKET s, std::string serverName, std::string remoteIP, std::string remotePort, int localPlayer)
//{
//	// This function plays the game and returns the value: winner.  This value 
//	// will be one of the following values: noWinner, xWinner, oWinner, TIE, ABORT
//	int winner = noWinner;
//	char board[10];
//	int opponent;
//	int move;
//	bool myMove;
//
//	if (localPlayer == PLAYER1) {
//		std::cout << "Playing as X" << std::endl;
//		opponent = PLAYER2;
//		myMove = true;
//	} else {
//		std::cout << "Playing as O" << std::endl;
//		opponent = PLAYER1;
//		myMove = false;
//	}
//
//	initializeBoard(board);
//	displayBoard(board);
//
//	while (winner == noWinner) {
//		if (myMove) {
//			// Get my move & display board
//			move = getMove(board, localPlayer);
//			std::cout << "Board after your move:" << std::endl;
//			updateBoard(board,move,localPlayer);
//			displayBoard(board);
//
//			// Send move to opponent
///****			
//	Task 1: "move" is an integer that was assigned a value (from 1 to 9) in the previous code segment.
//	         Add code here to convert "move" to a null-terminated C-string and send it to your opponent at remoteIP using remotePort.
//****/
//			char moveString[MAX_SEND_BUF - 1];
//			//sprintf_s(moveString, "%d\0", move);
//			_itoa_s(move, moveString, MAX_SEND_BUF - 1, 10);
//			UDP_send(s, moveString, strlen(moveString) , (char*)remoteIP.c_str(), (char*)remotePort.c_str());
//
//
//		} else {
//			std::cout << "Waiting for your opponent's move..." << std::endl << std::endl;
//			//Get opponent's move & display board
//			int status = wait(s,WAIT_TIME,0);
//			if (status > 0) {
///****			
//Task 2: (i) Insert code inside this IF statement that will accept a null-terminated C-string from your
//		opponent that represents their move.  Convert that string to an integer and then
//		(ii) call a function that will update the game board (see above) using your opponent's move, and
//		(iii) call a function that will display the updated board on your screen.
//****/
//				char opponentBuff[MAX_RECV_BUF -1];
//
//
//				UDP_recv(s, opponentBuff, MAX_RECV_BUF-1, (char*)remoteIP.c_str(), (char*)remotePort.c_str());
//
//				int opponentMove = atoi(opponentBuff);
//				updateBoard(board, opponentMove, opponent);
//				displayBoard(board);
//
//
//			} else {
//				winner = ABORT;
//			}
//		}
//		myMove = !myMove;
//
//		if (winner == ABORT) {
//			std::cout << timestamp() << " - No response from opponent.  Aborting the game..." << std::endl;
//		} else {
//			winner = check4Win(board);
//		}
//		
//		if (winner == localPlayer)
//			std::cout << "You WIN!" << std::endl;
//		else if (winner == TIE)
//			std::cout << "It's a tie." << std::endl;
//		else if (winner == opponent)
//			std::cout << "I'm sorry.  You lost" << std::endl;
//	}
//
//	return winner;
//}


int playNim(SOCKET s, std::string serverName, std::string remoteIP, std::string remotePort, int localPlayer) {
	int board[19];
	int winner = -1;
	int opponent;
	Move move;
	bool myMove;
	int firstMove = true;
	char decision = 'c';
	char* comment = new char[MAX_SEND_BUF - 1];

	if (localPlayer == PLAYER1) {
		char opponentBuff[MAX_RECV_BUF - 1];
		UDP_recv(s, opponentBuff, MAX_RECV_BUF - 1, (char*)remoteIP.c_str(), (char*)remotePort.c_str());
		parseBoard(opponentBuff, board);
		displayBoard(board);
		opponent = PLAYER2;
		myMove = true;
	}
	else {
		initializeBoard(board);
		displayBoard(board);
		char sendBoard[MAX_SEND_BUF - 1];
		encodeBoard(board, sendBoard);
		UDP_send(s, sendBoard, strlen(sendBoard) + 1, (char*)remoteIP.c_str(), (char*)remotePort.c_str());
		std::cout << "You go second. " << std::endl;
		opponent = PLAYER1;
		myMove = false;
	}

	

	while (winner == -1) {
		if (myMove) {
			// Get my move & display board

			//move = getMove(board);
			//updateBoard(board, move);
			//displayBoard(board);
			std::cout << "Your turn. " << std::endl;
			while (decision == 'c' || decision == 'C') {
				std::cout << "Enter first letter of one of the following commands (C or F);";
				std::cout << " or enter a number to make a move." << endl;
				std::cout << "Command (Chat, Forfeit, #)?";

				cin >> decision;

				//moveOptions(decision, move, board);
				if (decision >= '1' && decision <= '9') {
					move = getMove(board);
					updateBoard(board, move);
				}
				else if (decision == 'c' || decision == 'C') {
					string input;
					string commentStr = "C";

					std::cout << "What is your message? ";
					cin.ignore();
					getline(cin, input);

					commentStr.append(input);
					strcpy(comment, commentStr.c_str());

					UDP_send(s, comment, strlen(comment) + 1, (char*)remoteIP.c_str(), (char*)remotePort.c_str());
				}
				else if (decision == 'f' || decision == 'F') {
				std:cout << "forfeit" << endl;
				}
			}
			
			myMove = false;
			firstMove = false;



			//Send move to opponent

			char moveString[MAX_SEND_BUF - 1];
			//TODO: encode new move to be sent as a char array in UDP_SEND
			encodeMove(move, moveString);

		
			//sprintf_s(moveString, "%d\0", move);
			//_itoa_s(move, moveString, MAX_SEND_BUF - 1, 10);
			UDP_send(s, moveString, strlen(moveString) + 1, (char*)remoteIP.c_str(), (char*)remotePort.c_str());


		}
		else {
			std::cout << "Waiting for your opponent's move..." << std::endl << std::endl;
			//Get opponent's move & display board
			int status = wait(s, WAIT_TIME, 0);
			if (status > 0) {
				/****
				Task 2: (i) Insert code inside this IF statement that will accept a null-terminated C-string from your
						opponent that represents their move.  Convert that string to an integer and then
						(ii) call a function that will update the game board (see above) using your opponent's move, and
						(iii) call a function that will display the updated board on your screen.
				****/
				char opponentBuff[MAX_RECV_BUF - 1];

				UDP_recv(s, opponentBuff, MAX_RECV_BUF - 1, (char*)remoteIP.c_str(), (char*)remotePort.c_str());

				if (opponentBuff[0] == 'C')
				{
					cout << "(CHAT MESSAGE from " << serverName << "): " << endl << "  " << opponentBuff << endl;
				}
				else if (opponentBuff[0] == 'F')
				{
					winner = ABORT;
					std::cout << "YOU WON! Opponent has forfeited." << endl;
				}
				else if(opponentBuff[0] < '1' || opponentBuff[0] > '9')
				{
					winner = ABORT;
					std::cout << "YOU WON! Opponent has made in invalid move." << endl;
				}
				else
				{
					parseMove(opponentBuff, move);						// Accept and parse opponent move
				}
					

			
			
				if (!updateBoard(board, move))		
				{
					winner = ABORT;
					std::cout << "YOU WON! Opponent has made an invalid move." << endl;			// opponents move is invalid based on game layout
				}

				if (winner == ABORT) {
					std::cout << timestamp() << " - No response from opponent.  Aborting the game..." << std::endl;
				}
				else {
					winner = checkEndgame(board);
					if (winner != -1)
					{
						//Whoever just moved loses.
						if (myMove)
							winner = opponent;
						else
							winner = localPlayer;
					}
				}
				myMove = !myMove;

				if (winner == localPlayer)
					std::cout << "you win!" << std::endl;
				else if (winner == opponent)
					std::cout << "i'm sorry.  you lost" << std::endl;
				
				}

				//return winner;

				return true;
			}
		}
	}
