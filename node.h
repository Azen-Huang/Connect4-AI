#include "game.h"
#ifndef NODE_H 
#define NODE_H 
class Node {
public:
    int action;
    double reward;
    double n;
    vector<Node*> children;
    

    Node();
    Node(int action);
    bool expanded();
    void PrintTree(int pos = -1, string pre_output = "", bool isLast = true);
};
#endif