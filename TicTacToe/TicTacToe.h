#pragma once
#include <vector>
#include <array>
#include <iostream>

class TicTacToe
{
	struct Position
	{
		int8_t row;
		int8_t col;
	};
	std::vector<int8_t> board;
	std::vector<int8_t> openPositions;

	Position prevMove;

	const int8_t numToWin;
	const int8_t rows;
	const int8_t cols;

public:
	const int8_t numPlayers;

	TicTacToe(int8_t numPlayers, int8_t numToWin, int8_t rows, int8_t cols);

	void place(int8_t player, Position p);
	int8_t checkWin();
	int8_t lastMove();
	int8_t lastPlayer();
	int8_t nextPlayer();
	const std::vector<int8_t>& getMoves();

	void printBoard();

	int8_t p2i(Position pos);
	Position i2p(int8_t i);
};

