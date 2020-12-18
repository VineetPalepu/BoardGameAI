#include "MCTS.h"

int MCTS::getBestMove(TicTacToe game, double seconds)
{
    Node root(game, nullptr);
    std::chrono::high_resolution_clock::time_point start = std::chrono::high_resolution_clock::now();

    int iters = 0;
    //for (int i = 0; i < 400000; i++)
    while ( std::chrono::duration_cast<std::chrono::duration<double>>(std::chrono::high_resolution_clock::now() - start).count() < seconds)
    {
        iters++;
        Node* toExpand = selectNode(&root);
        Node* toSim = nullptr;
        int winner;
        if (toExpand->isTerminal)
        {
            toSim = toExpand;
            winner = toSim->winner;
        }
        else
        {
            toSim = expandNode(toExpand);
            winner = simulate(toSim->currentState);
        }
        backprop(toSim, winner);
    }

    std::cout << iters << " iterations complete." << std::endl;

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
            int winningPlayer = childState.checkWin();
            
            if (winningPlayer != 0)
            {
                child->isTerminal = true;
                child->winner = winningPlayer;
            }
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
    unexpanded.reserve(10);
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
        const auto& moves = state.getMoves();
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
    {
        backprop(node->parent, winner);
        node->UCB = UCB1(node);
    }

}

double MCTS::UCB1(Node* node)
{
    float t1 = node->wins[node->currentState.lastPlayer() - 1];
    float t2 = node->sims;
    float t3 = sqrtf(2);
    float t4 = logf(node->parent->sims + 1); // Add 1 here since we're caching the UCB value during backprop,
    float t5 = sqrtf(t4 / t2);               // and the parent hasn't been updated yet
    return t1 / t2 + t3 * t5;
    //return node->wins[node->currentState.lastPlayer() - 1] / node->sims + sqrt(2) * sqrt(log(node->parent->sims) / node->sims);
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
