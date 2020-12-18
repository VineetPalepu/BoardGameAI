#include "MCTS.h"

int MCTS::getBestMove(TicTacToe game)
{
    Node root(game, nullptr);

    for (int i = 0; i < 10000; i++)
    {
        Node* toExpand = selectNode(&root);
        Node* toSim = nullptr;
        int winner;
        if (toExpand->isTerminal)
        {
            toSim = toExpand;
            winner = toSim->currentState.checkWin();
        }
        else
        {
            toSim = expandNode(toExpand);
            winner = simulate(toSim->currentState);
        }
        backprop(toSim, winner);
    }

    Node* bestMove = *std::max_element(root.children.begin(), root.children.end(), 
        [](Node* n1, Node* n2) -> bool { return n1->sims < n2->sims; });

    return bestMove->currentState.lastMove();
}

MCTS::Node* MCTS::selectNode(Node* root)
{
    Node* node = root;
    while (node->isFullyExpanded() && !node->isLeaf())
    {
        node = *std::max_element(node->children.begin(), node->children.end(), nodeRank);
    }

    if (!node->childrenCreated)
    {
        for (auto move : node->currentState.getMoves())
        {
            TicTacToe childState = node->currentState;
            childState.place(node->currentState.nextPlayer(), node->currentState.i2p(move));
            Node* child = new Node(childState, node);
            if (childState.checkWin() != 0)
                child->isTerminal = true;
            node->children.push_back(child);
        }
        node->childrenCreated = true;
    }

    return node;
}

MCTS::Node* MCTS::expandNode(Node* node)
{
    static std::random_device dev;
    static std::mt19937 gen(dev());
    
    std::vector<Node*> unexpanded;
    for (auto child : node->children)
    {
        if (child->sims == 0)
            unexpanded.push_back(child);
    }
    std::uniform_int_distribution<int> range(0, unexpanded.size() - 1);

    return unexpanded[range(gen)];
}

int MCTS::simulate(TicTacToe state)
{
    static std::random_device dev;
    static std::mt19937 gen(dev());
    
    while (state.checkWin() == 0)
    {
        auto moves = state.getMoves();
        std::uniform_int_distribution<int> range(0, moves.size() - 1);
        state.place(state.nextPlayer(), state.i2p(moves[range(gen)]));
    }

    return state.checkWin();
}

void MCTS::backprop(Node* node, int winner)
{
    node->sims++;
    if (winner == -1)
        for (int i = 0; i < node->wins.size(); i++)
            node->wins[i] += .5;
    else
        node->wins[winner - 1]++;

    if (node->parent != nullptr)
        backprop(node->parent, winner);
}

double MCTS::UCB1(Node* node)
{
    return node->wins[node->currentState.lastPlayer() - 1] / node->sims + sqrt(2) * sqrt(log(node->parent->sims) / node->sims);
}

bool MCTS::Node::isLeaf()
{
    return children.size() == 0;
}

bool MCTS::Node::isFullyExpanded()
{    
    for (auto child : children)
    {
        if (child->sims == 0)
            return false;
    }
    return true;
}
