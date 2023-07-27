#include "mcts.h"
#include <chrono>
#define TIME 



MCTS::MCTS(Game* _board, int _simulationCount, int _playoutCount) {
    board = _board;
    simulationCount = _simulationCount;
    playoutCount = _playoutCount;
}

void showProgressBar(int progress, int total, int barWidth = 70) {
    float progressRatio = static_cast<float>(progress) / total;
    int barLength = static_cast<int>(barWidth * progressRatio);

    std::cout << "\r MCTS: [";
    for (int i = 0; i < barWidth; ++i) {
        if (i < barLength) std::cout << '=';
        else std::cout << ' ';
    }
    std::cout << "] " << static_cast<int>(progressRatio * 100.0) << "%";
    std::cout.flush();
}

int MCTS::getNxtAction() {
    Node* root = new Node();
    expand(root, board);
    auto startTime = std::chrono::high_resolution_clock::now();
    for (int i = 0; i < simulationCount; ++i) {
        search(root);
        #ifdef TIME 
        if ((i + 1) % int(simulationCount / 100) == 0 || i == simulationCount - 1) {
            showProgressBar(i + 1, simulationCount);
            // 計算執行時間並顯示在進度條的右邊
            auto currentTime = std::chrono::high_resolution_clock::now();
            double duration = std::chrono::duration_cast<std::chrono::seconds>(currentTime - startTime).count();
            std::cout << "  time:" << duration << " s";
            std::cout.flush();
        }
        #endif
    }
    cout << endl;
    int nxtAction = -1;
    int maxN = -1;
    for (auto& child : root->children) {
        if (child->n > maxN) {
            maxN = child->n;
            nxtAction = child->action;
        }
    }
    // root->PrintTree(root->action);
    deleteTree(root);
    return nxtAction;
}

void MCTS::search(Node* root) {
    Node* node = root;
    Game* scratch_game = board->clone();
    vector<Node*> path{root};

    while (node->expanded()) {
        auto [nxtNode, nxtAction] = select(node);
        scratch_game->move(nxtAction);
        path.push_back(nxtNode);
        node = nxtNode;
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

    delete scratch_game;
    return;
}

pair<Node*, int> MCTS::select(Node* node) {
    Node* bestNode = node->children[0];
    int bestAction = node->children[0]->action;
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
    return {bestNode, bestAction};
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
        delete scratch_game;
    }
    return value / playoutCount;
}

inline void MCTS::backpropagate(vector<Node*> path, double value) {
    reverse(path.begin(), path.end());
    for (auto& node : path) {
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

    for (auto& child : root->children) {
        deleteTree(child);
    }
    
    delete root;
}