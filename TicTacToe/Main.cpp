#include <iostream>

#include "TicTacToe.h"
#include "MCTS.h"

using namespace std;

int main()
{
	TicTacToe game(3, 3, 4, 4);
	int winner = 0;
	while (true)
	{
		// Human P1
		game.printBoard();
		int x, y;
		cin >> x;
		cin >> y;
		cout << endl;
		game.place(1, { x - 1, y - 1 });
		winner = game.checkWin();
		if (winner != 0)
			break;

		// CPU P2
		int move = MCTS::getBestMove(game);
		game.place(2, game.i2p(move));
		winner = game.checkWin();
		if (winner != 0)
			break;

		// CPU P3
		move = MCTS::getBestMove(game);
		game.place(3, game.i2p(move));
		winner = game.checkWin();
		if (winner != 0)
			break;
	}
	game.printBoard();
	cout << "Player " << winner << " won." << endl;

	system("pause");
}