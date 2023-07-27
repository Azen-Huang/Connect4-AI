#include "node.h"

Node::Node() {
    action = -1;
    reward = 0.0;
    n = 0.0;
    //children = {};
}

Node::Node(int _action) {
    action = _action;
    reward = 0.0;
    n = 0.0;
    //children = {};
}

bool Node::expanded() {
    return children.size() > 0;
}

void Node::PrintTree(int pos, string pre_output, bool isLast){
    auto node = this;
    // Condition when node is None
    if (node == NULL)
        return;
    
    cout << pre_output;
    int count = 2;
    float Qvalue;
    if (isLast) {
        cout << "└── " << "action:" << pos; 
        cout << ", n:" << node->n << ", reward:" << node->reward << ", child size:" << node->children.size() << ", node addr: " << node;
        cout << endl;
    }
    else {
        cout << "├── " << "action:" << pos; 
        cout << ", n:" << node->n << ", reward:" << node->reward << ", child size:" << node->children.size() << ", node addr: " << node;
        cout << endl;
    }

    int it = 0;
    int cnt = node->children.size();
    for(auto child: node->children){
        child->PrintTree(child->action, pre_output+"   ", it == (cnt - 1));
        it++;
    }
}