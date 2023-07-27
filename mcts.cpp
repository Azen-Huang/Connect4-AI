#include "mcts.h"

#define TIME 
#define TABLE

#ifdef TIME
#include <chrono>
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
#endif

MCTS::MCTS(Game* _game, int _simulationCount, int _playoutCount)
    : game(_game), simulationCount(_simulationCount), playoutCount(_playoutCount) {}

int MCTS::getNxtAction() {
    random_device rd;
    mt19937 gen(rd());

    Node* root = new Node();
    expand(root, game);
    auto startTime = std::chrono::high_resolution_clock::now();
    for (int i = 0; i < simulationCount; ++i) {
        search(root, gen);
        #ifdef TIME 
        if ((i + 1) % int(simulationCount / 100) == 0 || i == simulationCount - 1) {
            showProgressBar(i + 1, simulationCount);
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
    for (const auto& child : root->children) {
        if (child->n > maxN) {
            maxN = child->n;
            nxtAction = child->action;
        }
    }
    // root->PrintTree(root->action);
    deleteTree(root);
    return nxtAction;
}

inline void MCTS::search(Node* root, mt19937& gen) {
    Node* node = root;
    Game* scratch_game = game->clone();
    vector<Node*> path{root};

    while (node->expanded()) {
        auto [nxtNode, nxtAction] = select(node);
        scratch_game->move(nxtAction);
        path.emplace_back(nxtNode);
        node = nxtNode;
    }
    double value;
    int score = scratch_game->score();
    if (score != -1) {
        value = score;
    }
    else {
        expand(node, scratch_game);
        value = playout(scratch_game, gen);
    }
    backpropagate(path, value);

    delete scratch_game;
    return;
}

inline pair<Node*, int> MCTS::select(Node* node) const{
    Node* bestNode;
    int bestAction;
    double bestScore = -1;
    for (const auto& child : node->children) {
        double uct_score = child->n == 0 ? 10000 : child->reward / (child->n) + c * sqrt(log(node->n) / (child->n));
        if (uct_score > bestScore) {
            bestScore = uct_score;
            bestNode = move(child);
            bestAction = child->action;
        }
    }
    return {bestNode, bestAction};
}

inline void MCTS::expand(Node* node, Game* _game) const{
    vector<int> ValidAction = _game->getValidAction();
    for (const auto& action : ValidAction) {
        Node* nxtNode = new Node(action);
        node->children.emplace_back(nxtNode);
    }
    return;
}

inline double MCTS::playout(Game* _game, mt19937& gen) {
    #ifdef TABLE
    auto VecToStr = [&](){
        string s = "";
        for (auto& v : _game->board) s += to_string(v);
        return s;
    };
    string boardInfo = VecToStr();
    if (table.find(boardInfo) != table.end()) {
        return table[boardInfo];
    }
    #endif

    double value = 0;
    auto onePlayout = [&](Game* game) {
        int current_player = _game->turn;
        double score = _game->score();
        while (score == -1) {
            vector<int> validAction = _game->getValidAction();
            uniform_int_distribution<> distrib(0, validAction.size() - 1); // 定義範圍
            int randomIndex = distrib(gen); // 從 0 到 vector 大小減 1 間隨機取一個 index
            int nxtAction = validAction[randomIndex]; // 取得隨機數字
            _game->move(nxtAction);
            score = _game->score();
        }
        if (score == 0) {
            return 0;
        }
        int win_player = _game->turn == 1 ? 2 : 1;
        return current_player == win_player ? 1 : -1;
    };

    for (int i = 0; i < playoutCount; ++i) {
        Game* scratch_game = _game->clone();
        value += onePlayout(scratch_game);
        delete scratch_game;
    }

    #ifdef TABLE
    table[boardInfo] = value / playoutCount;
    return table[boardInfo];
    #endif

    return value / playoutCount;
}

inline void MCTS::backpropagate(vector<Node*> path, double value) const{
    reverse(path.begin(), path.end());
    for (Node*& node : path) {
        node->reward += value;
        node->n += 1;
        value *= -1;
    }
    return;
}

inline void MCTS::deleteTree(Node* root) const{
    if (root == nullptr) {
        return;
    }

    for (auto& child : root->children) {
        deleteTree(child);
    }
    
    delete root;
}


