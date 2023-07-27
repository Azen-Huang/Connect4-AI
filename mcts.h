#include "node.h"
#include "game.h"
#include <cmath>
#include <random>
#ifndef MCTS_H 
#define MCTS_H 
class MCTS{
public:
    MCTS(Game* _board, int _simulationCount, int _playoutCount);
    int getNxtAction();
private:
    void search(Node* root);
    // Node* select(Node* node, int& action);
    pair<Node*, int> select(Node* node);
    void expand(Node* node, Game* _board);
    double playout(Game* _board);
    void backpropagate(vector<Node*> path, double value);
    const double onePlayout(Game* _board);
    void deleteTree(Node* node);

    Game* board;
    double playoutCount;
    double simulationCount;
    double c = sqrt(2);
    double EPS = 1e-8;
};
#endif