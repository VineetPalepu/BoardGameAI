#pragma once

#include <vector>
#include <random>
#include <memory>
#include <chrono>

#include "TicTacToe.h"

class MCTS
{
	struct Node
	{
		float UCB;
		int sims;
		int winner = 0;
		std::vector<float> wins;
		bool isTerminal;
		bool childrenCreated;
		Node* parent;
		std::vector<Node*> children;

		TicTacToe currentState;
		
		Node(TicTacToe state, Node* parent)
			:currentState(state), wins(state.numPlayers), sims(), isTerminal(false), childrenCreated(false), UCB(), parent(parent), children()
		{
		}

		~Node()
		{
			for (auto child : children)
			{
				delete child;
			}
		}

		bool isLeaf();
		bool isFullyExpanded();
	};

public:

	static int getBestMove(TicTacToe game, double seconds);

	static Node* selectNode(Node* root);
	static Node* expandNode(Node* node);
	static int simulate(TicTacToe state);
	static void backprop(Node* node, int winner);

	static double UCB1(Node* node);

	static bool nodeRank(Node* n1, Node* n2) { return n1->UCB < n2->UCB; };
};

