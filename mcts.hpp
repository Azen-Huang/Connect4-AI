#include "node.h"
#include "gameStrategy.h"
#include <cmath>
#include <unordered_map>
#include <random>
#include <chrono>

#ifndef MCTS_H 
#define MCTS_H 

template <typename BoardType>
class MCTS{
public:
    MCTS(GameStrategy<BoardType>* _game, int _simulationCount, int _playoutCount);
    int getNxtAction();
private:
    void search(Node* root, mt19937& gen);

    pair<Node*, int> select(Node* node) const;
    void expand(Node* node, GameStrategy<BoardType>* _game) const;
    double playout(GameStrategy<BoardType>* _game, mt19937& gen);
    void backpropagate(vector<Node*> path, double value) const;
    
    void deleteTree(Node* node) const;

    GameStrategy<BoardType>* game;
    double playoutCount;
    double simulationCount;
    static constexpr double c = 1.414; //sqrt(2)
    unordered_map<string, double> table;
};
#endif

#define TIME 
// #define TABLE

#ifdef TIME
void showProgressBar(int progress, int total, int barWidth = 70) {
    float progressRatio = static_cast<float>(progress) / total;
    int barLength = static_cast<int>(barWidth * progressRatio);

    std::cout << "\r Monte-Carlo Tree Search Bar: [";
    for (int i = 0; i < barWidth; ++i) {
        if (i < barLength) std::cout << '=';
        else std::cout << ' ';
    }
    std::cout << "] " << static_cast<int>(progressRatio * 100.0) << "%";
    std::cout.flush();
}
#endif

template <typename BoardType>
MCTS<BoardType>::MCTS(GameStrategy<BoardType>* _game, int _simulationCount, int _playoutCount)
    : game(_game), simulationCount(_simulationCount), playoutCount(_playoutCount) {}

template <typename BoardType>
int MCTS<BoardType>::getNxtAction() {
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

template <typename BoardType>
inline void MCTS<BoardType>::search(Node* root, mt19937& gen) {
    Node* node = root;
    GameStrategy<BoardType>* scratch_game = game->clone();
    vector<Node*> path{root};

    while (node->expanded()) {
        auto [nxtNode, nxtAction] = select(node);
        scratch_game->move(nxtAction);
        path.emplace_back(nxtNode);
        node = nxtNode;
    }
    double value;
    if (is_same_v<BoardType, BitBoard>) {
        scratch_game->undo(node->action);
    }
    int score = scratch_game->score(node->action);
    if (is_same_v<BoardType, BitBoard>) {
        scratch_game->move(node->action);
    }
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

template <typename BoardType>
inline pair<Node*, int> MCTS<BoardType>::select(Node* node) const{
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

template <typename BoardType>
inline void MCTS<BoardType>::expand(Node* node, GameStrategy<BoardType>* _game) const{
    vector<int> ValidAction = _game->getValidAction();
    for (const auto& action : ValidAction) {
        Node* nxtNode = new Node(action);
        node->children.emplace_back(nxtNode);
    }
    return;
}

template <typename BoardType>
inline double MCTS<BoardType>::playout(GameStrategy<BoardType>* _game, mt19937& gen) {
    #ifdef TABLE
    string key = _game->getKey();
    if (table.find(key) != table.end()) {
        return table[key];
    }
    #endif

    double value = 0;
    auto onePlayout = [&](GameStrategy<BoardType>* game) {
        int current_player = game->turn;
        double score;
        while (true) {
            vector<int> validAction = game->getValidAction();
            uniform_int_distribution<> distrib(0, validAction.size() - 1); // 定義範圍
            int randomIndex = distrib(gen); // 從 0 到 vector 大小減 1 間隨機取一個 index
            int nxtAction = validAction[randomIndex]; // 取得隨機數字
            game->move(nxtAction);
            score = game->score(nxtAction);

            if (score != -1) {
                break;
            }
        }

        if (score == 0) {
            return 0;
        }
        int win_player = game->turn == 1 ? 2 : 1;
        return current_player == win_player ? 1 : -1;
    };

    for (int i = 0; i < playoutCount; ++i) {
        GameStrategy<BoardType>* scratch_game = _game->clone();
        value += onePlayout(scratch_game);
        delete scratch_game;
    }

    #ifdef TABLE
    table[key] = value / playoutCount;
    return table[key];
    #endif

    return value / playoutCount;
}

template <typename BoardType>
inline void MCTS<BoardType>::backpropagate(vector<Node*> path, double value) const{
    reverse(path.begin(), path.end());
    for (Node*& node : path) {
        node->reward += value;
        node->n += 1;
        value *= -1;
    }
    return;
}

template <typename BoardType>
inline void MCTS<BoardType>::deleteTree(Node* root) const{
    if (root == nullptr) {
        return;
    }

    for (auto& child : root->children) {
        deleteTree(child);
    }
    
    delete root;
}


