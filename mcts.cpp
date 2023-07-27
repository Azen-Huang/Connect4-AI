#include "mcts.h"

MCTS::MCTS(Game* _board, int _simulationCount, int _playoutCount) {
    board = _board;
    simulationCount = _simulationCount;
    playoutCount = _playoutCount;
}

int MCTS::getNxtAction() {
    Node* root = new Node();
    expand(root, board);

    for (int i = 0; i < simulationCount; ++i) {
        if (i % int(simulationCount / 10) == 0) {
            cout << "Simulation: " << i << " / " << simulationCount << endl;
        }
        search(root);
    }

    int nxtAction = -1;
    int MaxN = -1;
    for (auto child : root->children) {
        if (child->n > MaxN) {
            MaxN = child->n;
            nxtAction = child->action;
        }
    }
    // root->PrintTree(-1, "", true);
    deleteTree(root);
    return nxtAction;
}

void MCTS::search(Node* root) {
    Node* node = root;
    Game* scratch_game = board->clone();
    vector<Node*> path{root};
    
    int action = -1;
    while (node->expanded()) {
        node = select(node, action);
        scratch_game->move(action);
        path.push_back(node);
    }
    double value;
    int score = scratch_game->score();
    if (score != -1) {
        value = score;
    }
    else {
        expand(node, scratch_game);
        value = playout(scratch_game);
    }
    backpropagate(path, value);
    return;
}

Node* MCTS::select(Node* node, int& action) {
    Node* bestNode;
    int bestAction;
    double bestScore = -1;
    //node->PrintTree(node->action, "", true);
    for (auto& child : node->children) {
        double uct_score = child->n == 0 ? 10000 : child->reward / (child->n) + c * sqrt(log(node->n) / (child->n));
        if (uct_score > bestScore) {
            bestScore = uct_score;
            bestNode = child;
            bestAction = child->action;
        }
    }
    action = bestAction;
    return bestNode;
}

void MCTS::expand(Node* node, Game* _board) {
    vector<int> ValidAction = _board->getValidAction();
    for (auto& action : ValidAction) {
        Node* nxtNode = new Node(action);
        node->children.push_back(nxtNode);
    }
    return;
}

inline const double MCTS:: onePlayout(Game* _board) {
    int current_player = _board->turn;
    double score = _board->score();
    while (score == -1) {
        vector<int> validAction = _board->getValidAction();

        random_device rd;  // 使用真實的硬體來產生種子
        mt19937 gen(rd()); // 使用 Mersenne Twister 引擎
        uniform_int_distribution<> distrib(0, validAction.size() - 1); // 定義範圍
        int randomIndex = distrib(gen); // 從 0 到 vector 大小減 1 間隨機取一個 index
        int nxtAction = validAction[randomIndex]; // 取得隨機數字

        _board->move(nxtAction);
        score = _board->score();
    }

    if (score == 0) {
        return 0;
    }
    int win_player = _board->turn == 1 ? 2 : 1;
    return current_player == win_player ? 1 : -1;
}

double MCTS::playout(Game* _board) {
    double value = 0;
    for (int i = 0; i < playoutCount; ++i) {
        Game* scratch_game = _board->clone();
        value += onePlayout(scratch_game);
    }
    return value / playoutCount;
}

inline void MCTS::backpropagate(vector<Node*> path, double value) {
    reverse(path.begin(), path.end());
    for (auto node : path) {
        node->reward += value;
        node->n += 1;
        value *= -1;
    }
    return;
}

void MCTS::deleteTree(Node* root) {
    if (root == nullptr) {
        return;
    }

    // 釋放所有子節點的記憶體
    for (auto child : root->children) {
        deleteTree(child);
    }
    
    // 釋放根節點的記憶體
    delete root;
}