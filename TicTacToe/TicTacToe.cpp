#include "TicTacToe.h"

TicTacToe::TicTacToe(int8_t numPlayers, int8_t numToWin, int8_t rows, int8_t cols)
	:board(rows * cols), openPositions{}, numPlayers{ numPlayers }, numToWin{ numToWin }, rows{ rows }, cols{ cols }
{
	for (int i = 0; i < rows * cols; i++)
	{
		openPositions.push_back(i);
	}
	prevMove = { -1, -1 };
}

void TicTacToe::place(int8_t player, const Position& pos)
{
	board[p2i(pos)] = player;
	openPositions.erase(std::remove(openPositions.begin(), openPositions.end(), p2i(pos)), openPositions.end());
	prevMove = pos;
}

int8_t TicTacToe::checkWin()
{
	if (prevMove.row == -1 && prevMove.col == -1)
		return 0;

	if (openPositions.size() == 0)
		return -1;

	static const std::array<const Position, 4> directions =
	{{
		{-1, -1},
		{-1, 0},
		{-1, 1},
		{0, 1}
	}};

	int player = board[p2i(prevMove)];
	for (auto& dir : directions)
	{
		int consecutive = 0;
		Position newPos = prevMove;
		while (newPos.row >= 0 && newPos.row < rows
			&& newPos.col >= 0 && newPos.col < cols
			&& board[p2i(newPos)] == player)
		{
			consecutive++;
			if (consecutive >= numToWin)
				return player;
			newPos.col += dir.col;
			newPos.row += dir.row;
		}

		consecutive--;
		newPos = prevMove;

		while (newPos.row >= 0 && newPos.row < rows
			&& newPos.col >= 0 && newPos.col < cols
			&& board[p2i(newPos)] == player)
		{
			consecutive++;
			if (consecutive >= numToWin)
				return player;
			newPos.col -= dir.col;
			newPos.row -= dir.row;
		}
	}
	return 0;
}

int8_t TicTacToe::lastMove()
{
	return p2i(prevMove);
}

int8_t TicTacToe::lastPlayer()
{
	if (prevMove.row == -1 && prevMove.col == -1)
		return 0;

	return board[p2i(prevMove)];
}

int8_t TicTacToe::nextPlayer()
{
	return lastPlayer() % numPlayers + 1;
}

const std::vector<int8_t>& TicTacToe::getMoves()
{
	if (checkWin() != 0) // If someone has won, no possible moves
		openPositions.clear();

	return openPositions;
}

void TicTacToe::printBoard()
{
	for (int8_t row = 0; row < rows; row++)
	{
		for (int8_t col = 0; col < cols; col++)
		{
			std::cout << (int)board[p2i({ row, col })] << " ";
		}
		std::cout << std::endl;
	}
	std::cout << std::endl;
}

int8_t TicTacToe::p2i(const Position& pos)
{
	return pos.row * cols + pos.col;
}

TicTacToe::Position TicTacToe::i2p(int8_t i)
{
	return {i / cols, i % cols};
}
