#pragma once
#include <vector>
#include <array>
#include <iostream>

class TicTacToe
{
	struct Position
	{
		int row;
		int col;
	};
	std::vector<int> board;
	std::vector<int> openPositions;

	Position prevMove;

	const int numToWin;
	const int rows;
	const int cols;

public:
	const int numPlayers;

	TicTacToe(int numPlayers, int numToWin, int rows, int cols);

	void place(int player, Position p);
	int checkWin();
	int lastMove();
	int lastPlayer();
	int nextPlayer();
	std::vector<int> getMoves();

	void printBoard();

	int p2i(Position pos);
	Position i2p(int i);
};

