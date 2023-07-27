#include "node.h"
#include "game.h"
#include <cmath>
#include <unordered_map>
#include <random>

#ifndef MCTS_H 
#define MCTS_H 
class MCTS{
public:
    MCTS(Game* _game, int _simulationCount, int _playoutCount);
    int getNxtAction();
private:
    void search(Node* root, mt19937& gen);

    pair<Node*, int> select(Node* node) const;
    void expand(Node* node, Game* _game) const;
    double playout(Game* _game, mt19937& gen);
    void backpropagate(vector<Node*> path, double value) const;
    
    void deleteTree(Node* node) const;

    Game* game;
    double playoutCount;
    double simulationCount;
    static constexpr double c = 1.414; //sqrt(2)
    unordered_map<string, double> table;
};
#endif