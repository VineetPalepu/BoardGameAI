#include <iostream>
#include <chrono>

#include "TicTacToe.h"
#include "MCTS.h"

using namespace std;
using namespace std::chrono;

int main()
{	
	int players = 2;
	int numToWin = 4;
	int sizeX = 7;
	int sizeY = 7;

	high_resolution_clock::time_point start = high_resolution_clock::now();
	
	TicTacToe game(players, numToWin, sizeX, sizeY);
	int winner = 0;
	while (true)
	{
		// CPU P1
		game.printBoard();
		int move = MCTS::getBestMove(game, 2);
		game.place(1, game.i2p(move));
		winner = game.checkWin();
		if (winner != 0)
			break;

		// CPU P2
		game.printBoard();
		move = MCTS::getBestMove(game, 2);
		game.place(2, game.i2p(move));
		winner = game.checkWin();
		if (winner != 0)
			break;
	}
	game.printBoard();
	cout << "Player " << winner << " won." << endl;

	cout << "Elapsed: " << duration_cast<duration<double>>(high_resolution_clock::now() - start).count() << endl;
	
	system("pause");
}