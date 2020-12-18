#include "TicTacToe.h"

TicTacToe::TicTacToe(int numPlayers, int numToWin, int rows, int cols)
	:board(rows * cols), openPositions{}, numPlayers{ numPlayers }, numToWin{ numToWin }, rows{ rows }, cols{ cols }
{
	for (int i = 0; i < rows * cols; i++)
	{
		openPositions.push_back(i);
	}
	prevMove = { -1, -1 };
}

void TicTacToe::place(int player, Position pos)
{
	board[p2i(pos)] = player;
	openPositions.erase(std::remove(openPositions.begin(), openPositions.end(), p2i(pos)), openPositions.end());
	prevMove = pos;
}

int TicTacToe::checkWin()
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
			newPos = { newPos.row + dir.row, newPos.col + dir.col };
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
			newPos = { newPos.row - dir.row, newPos.col - dir.col };
		}
	}
	return 0;
}

int TicTacToe::lastMove()
{
	return p2i(prevMove);
}

int TicTacToe::lastPlayer()
{
	if (prevMove.row == -1 && prevMove.col == -1)
		return 0;

	return board[p2i(prevMove)];
}

int TicTacToe::nextPlayer()
{
	return lastPlayer() % numPlayers + 1;
}

std::vector<int> TicTacToe::getMoves()
{
	if (checkWin() != 0) // If someone has won, no possible moves
		return {};

	return openPositions;
}

void TicTacToe::printBoard()
{
	for (int row = 0; row < rows; row++)
	{
		for (int col = 0; col < cols; col++)
		{
			std::cout << board[p2i({ row, col })] << " ";
		}
		std::cout << std::endl;
	}
	std::cout << std::endl;
}

int TicTacToe::p2i(Position pos)
{
	return pos.row * cols + pos.col;
}

TicTacToe::Position TicTacToe::i2p(int i)
{
	return {i / cols, i % cols};
}
