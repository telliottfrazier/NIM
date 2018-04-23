// playNIM.cpp
// This set of functions are used to actually play the game.
// Play starts with the function: playNIM() which is defined below

#include "NIM.h"
#include <WinSock2.h>
#include <iostream>
#include <string>
#include <random>

using std::cout;
using std::cin;
using std::endl;
using std::random_device;
using std::default_random_engine;
using std::uniform_int_distribution;

void initializeBoard(int board[19], int localPlayer)
{
	random_device rd;
	default_random_engine engine(rd());
	uniform_int_distribution<int> piles(3, 9);
	uniform_int_distribution<int> rocks(1, 20);

	if (localPlayer == PLAYER1)
	{
		//Get board from opponent.
	}
	else
	{
		//Create and send a board.
		//3 to 9 piles with 1 to 20 rocks per pile.
		board[0] = piles(engine);

		for (int i = 1; i <= board[0]; ++i)
		{
			board[i] = rocks(engine);
		}
	}
}


int board[10][20];
void initializeBoard(int board[][20])
{
	cout << "NIM board" << endl;
	cout << "-------------------------------------------------------------------------------" << endl;
	for (int i = 0; i < board[i][0]; i++) {
		cout << "Rock Pile #" << i << " ->";

	}

}
//void initializeBoard( char board[10] )
//{
//
//}

//void updateBoard( char board[10], int move, int Player)
//{
//	if (Player == PLAYER1) {
//		board[move] = 'X';
//	} else if (Player == PLAYER2) {
//		board[move] = 'O';
//	} else
//		std::cout << "Problem with updateBoard function!" << std::endl;
//}

void displayBoard( char board[10] )
{
	std::cout << std::endl;
	std::cout << board[7] << " | " << board[8] << " | " << board[9] << std::endl;
	std::cout << "__+___+__" << std::endl;
	std::cout << board[4] << " | " << board[5] << " | " << board[6] << std::endl;
	std::cout << "__+___+__" << std::endl;
	std::cout << board[1] << " | " << board[2] << " | " << board[3] << std::endl;
	std::cout << std::endl;
}

//
//int check4Win(char board[10])
//{
//	int winner = noWinner;
//
//	// Check for vertical winners
//	int i = 1;
//	while (winner == noWinner && i < 4) {
//		if (board[i] == board[i+3] && board[i] == board[i+6]) {
//			winner = (board[i] == 'X') ? xWinner : oWinner;
//		}
//		i++;
//	}
//
//	// Check for horizontal winners
//	i = 1;
//	while (winner == noWinner && i < 8) {
//		if (board[i] == board[i+1] && board[i] == board[i+2]) {
//			winner = (board[i] == 'X') ? xWinner : oWinner;
//		}
//		i  += 3;
//	}
//
//	// Check for diagonal winners
//	if (winner == noWinner) {
//		if ( (board[1] == board[5] && board[1] == board[9]) ||
//			 (board[3] == board[5] && board[3] == board[7]) ) {
//			winner = (board[5] == 'X') ? xWinner : oWinner;
//		}
//	}
//
//	// Check for tie
//	i = 1;
//	int numMoves = 0;
//	while ( i < 10) {
//		if ( (board[i] == 'X' || board[i] == 'O') ) {
//			numMoves++;
//		}
//		i++;
//	}
//	if (winner == noWinner && numMoves == 9)
//		winner = TIE;
//	
//
//	return winner;
//}
//
//int getMove(char board[10], int Player)
//{
//	int move;
//	char move_str[80];
//
//	std::cout << "Where do you want to place your ";
//	char mark = (Player == PLAYER1) ? 'X' : 'O';
//	std::cout << mark << "? " << std::endl;
//
//	do {
//		std::cout << "Your move? ";
//		std::cin  >> move_str;
//		move = atoi(move_str);
//		if (board[move] == 'X' || board[move] == 'O') move = 0;
//	} while (move < 1 || move > 9);
//
//	return move;
//}
//
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

int check4Win(char board[10])
{
	int winner = noWinner;

	// Check for vertical winners
	int i = 1;
	while (winner == noWinner && i < 4) {
		if (board[i] == board[i+3] && board[i] == board[i+6]) {
			winner = (board[i] == 'X') ? xWinner : oWinner;
		}
		i++;
	}

	// Check for horizontal winners
	i = 1;
	while (winner == noWinner && i < 8) {
		if (board[i] == board[i+1] && board[i] == board[i+2]) {
			winner = (board[i] == 'X') ? xWinner : oWinner;
		}
		i  += 3;
	}

	// Check for diagonal winners
	if (winner == noWinner) {
		if ( (board[1] == board[5] && board[1] == board[9]) ||
			 (board[3] == board[5] && board[3] == board[7]) ) {
			winner = (board[5] == 'X') ? xWinner : oWinner;
		}
	}

	// Check for tie
	i = 1;
	int numMoves = 0;
	while ( i < 10) {
		if ( (board[i] == 'X' || board[i] == 'O') ) {
			numMoves++;
		}
		i++;
	}
	if (winner == noWinner && numMoves == 9)
		winner = TIE;
	

	return winner;
}

int getMove(char board[10], int Player)
{
	int move;
	char move_str[80];

	std::cout << "Where do you want to place your ";
	char mark = (Player == PLAYER1) ? 'X' : 'O';
	std::cout << mark << "? " << std::endl;

	do {
		std::cout << "Your move? ";
		std::cin  >> move_str;
		move = atoi(move_str);
		if (board[move] == 'X' || board[move] == 'O') move = 0;
	} while (move < 1 || move > 9);

	return move;
}

int playNim(SOCKET s, std::string serverName, std::string remoteIP, std::string remotePort, int localPlayer)
{
	// This function plays the game and returns the value: winner.  This value 
	// will be one of the following values: noWinner, xWinner, oWinner, TIE, ABORT
	int winner = noWinner;
	char board[20];
	int opponent;
	int move;
	bool myMove;

	if (localPlayer == PLAYER1) {
		std::cout << "Playing first" << std::endl;
		opponent = PLAYER2;
		myMove = true;
	} else {
		std::cout << "Playing second" << std::endl;
		opponent = PLAYER1;
		myMove = false;
	}

	initializeBoard(board, localPlayer);
	displayBoard(board);

	while (winner == noWinner) {
		if (myMove) {
			// Get my move & display board
			move = getMove(board, localPlayer);
			std::cout << "Board after your move:" << std::endl;
			updateBoard(board,move,localPlayer);
			displayBoard(board);

			// Send move to opponent
/****			
	Task 1: "move" is an integer that was assigned a value (from 1 to 9) in the previous code segment.
	         Add code here to convert "move" to a null-terminated C-string and send it to your opponent at remoteIP using remotePort.
****/
			char moveString[MAX_SEND_BUF - 1];
			//sprintf_s(moveString, "%d\0", move);
			_itoa_s(move, moveString, MAX_SEND_BUF - 1, 10);
			UDP_send(s, moveString, strlen(moveString) , (char*)remoteIP.c_str(), (char*)remotePort.c_str());


		} else {
			std::cout << "Waiting for your opponent's move..." << std::endl << std::endl;
			//Get opponent's move & display board
			int status = wait(s,WAIT_TIME,0);
			if (status > 0) {
/****			
Task 2: (i) Insert code inside this IF statement that will accept a null-terminated C-string from your
		opponent that represents their move.  Convert that string to an integer and then
		(ii) call a function that will update the game board (see above) using your opponent's move, and
		(iii) call a function that will display the updated board on your screen.
****/
				char opponentBuff[MAX_RECV_BUF -1];


				UDP_recv(s, opponentBuff, MAX_RECV_BUF-1, (char*)remoteIP.c_str(), (char*)remotePort.c_str());

				int opponentMove = atoi(opponentBuff);
				updateBoard(board, opponentMove, opponent);
				displayBoard(board);


			} else {
				winner = ABORT;
			}
		}
		myMove = !myMove;

		if (winner == ABORT) {
			std::cout << timestamp() << " - No response from opponent.  Aborting the game..." << std::endl;
		} else {
			winner = check4Win(board);
		}
		
		if (winner == localPlayer)
			std::cout << "You WIN!" << std::endl;
		else if (winner == TIE)
			std::cout << "It's a tie." << std::endl;
		else if (winner == opponent)
			std::cout << "I'm sorry.  You lost" << std::endl;
	}

	return winner;
}
