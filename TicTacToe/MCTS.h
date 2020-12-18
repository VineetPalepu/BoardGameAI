#pragma once

#include <vector>
#include <random>
#include <memory>

#include "TicTacToe.h"

class MCTS
{
	struct Node
	{
		TicTacToe currentState;

		std::vector<float> wins;
		int sims;
		bool isTerminal;
		bool childrenCreated;
		Node* parent;
		std::vector<Node*> children;
		
		Node(TicTacToe state, Node* parent)
			:currentState(state), wins(state.numPlayers), sims(), isTerminal(false), childrenCreated(false), parent(parent), children()
		{
		}

		bool isLeaf();
		bool isFullyExpanded();
	};

public:

	static int getBestMove(TicTacToe game);

	static Node* selectNode(Node* root);
	static Node* expandNode(Node* node);
	static int simulate(TicTacToe state);
	static void backprop(Node* node, int winner);

	static double UCB1(Node* node);
	

	static bool nodeRank(Node* n1, Node* n2) { return UCB1(n1) < UCB1(n2); };
};

