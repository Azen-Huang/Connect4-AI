#include <iostream>
#include <cstdlib>
#include <random>

#include "gameStrategy.h"
#include "board.h"
#include "bitboard.h"
#include "node.h"
#include "mcts.hpp"

using namespace std;

int input(char c) {
    return toupper(c) - 'A';
}

// In this place, your can choose to use the array version or the bitboard version.
// #define BITBOARD
#define BOARD

const int EVALUATION_COUNT = 10000;
const int PLAYOUT_COUNT = 200;

void AI_vs_AI() {
    #ifdef BITBOARD
    GameStrategy<bitboard>* board = new BitBoard();
    #endif
    #ifdef BOARD
    GameStrategy<vector<int>>* board = new Board();
    #endif

    int score;
    board->move(3);
    board->print();
    while (true) {
        #ifdef BITBOARD
        MCTS<bitboard>* mcts = new MCTS(board, EVALUATION_COUNT, PLAYOUT_COUNT);
        #endif
        #ifdef BOARD
        MCTS<vector<int>>* mcts = new MCTS(board, EVALUATION_COUNT, PLAYOUT_COUNT);
        #endif

        int action = mcts->getNxtAction();
        board->move(action);
        board->print();
        score = board->score(action);
        if (score != -1) {
            break;
        }
        delete mcts;
    }

    char winner = board->turn == 1 ? 'X' : 'O';
    if (score == 0) {
        cout << "Draw" << endl;
    }
    else {
        cout << "Winner is " << winner << "." << endl;
    }

    delete board;
    return;
}

void Human_vs_AI() {
    #ifdef BITBOARD
    GameStrategy<bitboard>* board = new BitBoard();
    #endif
    #ifdef BOARD
    GameStrategy<vector<int>>* board = new Board();
    #endif

    string strAction;
    int action;
    int score;
    board->print();
    cout << "Who first? (1. AI, 2. Human)" << endl;
    getline(cin, strAction);
    if (strAction == "1") {
        board->move(3);
        board->print();
    }

    while (true) {
        cout << "Input your action: ";
        getline(cin, strAction);
        action = input(strAction[0]);
        board->move(action);
        board->print();
        score = board->score(action);
        if (score != -1) {
            string win_turn = board->turn == 1 ? "X" : "O";
            cout << win_turn << " Win." << endl;
            break;
        }

        #ifdef BITBOARD
        MCTS<bitboard>* mcts = new MCTS(board, EVALUATION_COUNT, PLAYOUT_COUNT);
        #endif
        #ifdef BOARD
        MCTS<vector<int>>* mcts = new MCTS(board, EVALUATION_COUNT, PLAYOUT_COUNT);
        #endif

        int action = mcts->getNxtAction();
        board->move(action);
        board->print();
        score = board->score(action);
        if (score != -1) {
            string win_turn = board->turn == 1 ? "X" : "O";
            cout << win_turn << " Win." << endl;
            break;
        }
    }
    delete board;
}

void Human_vs_Human() {
    #ifdef BITBOARD
    GameStrategy<bitboard>* board = new BitBoard();
    #endif
    #ifdef BOARD
    GameStrategy<vector<int>>* board = new Board();
    #endif

    string strAction;
    board->print();
    int action = -1;
    while (getline(cin, strAction)) {
        if (strAction == "u") {
            board->undo(action);
            continue;
        }
        action = input(strAction[0]);
        board->move(action);
        board->print();
        vector<int> validAction = board->getValidAction();
        cout << "[";
        for (auto& action : validAction) {
            cout << action << ", ";
        }
        cout << "]" << endl;
        int score = board->score(action);
        if (score != -1) {
            string win_turn = board->turn == 1 ? "X" : "O";
            cout << win_turn << " Win." << endl;
            break;
        }
    }
    delete board;
}

void DEBUG() {
    #ifdef BITBOARD
    GameStrategy<bitboard>* board = new BitBoard();
    #endif
    #ifdef BOARD
    GameStrategy<vector<int>>* board = new Board();
    #endif
    
    vector<string> DEBUG_COMMAND{"d", "d", "u", "u"};

    // string strAction;
    board->print();
    int action = -1;
    for (auto& strAction : DEBUG_COMMAND) {
    // while (getline(cin, strAction)) {
        if (strAction == "u") {
            board->undo(action);
            board->print();
            continue;
        }
        action = input(strAction[0]);
        board->move(action);
        board->print();
        vector<int> validAction = board->getValidAction();
        cout << "[";
        for (auto& action : validAction) {
            cout << action << ", ";
        }
        cout << "]" << endl;
        int score = board->score(action);
        if (score != -1) {
            string win_turn = board->turn == 1 ? "X" : "O";
            cout << win_turn << " Win." << endl;
            break;
        }
    }
    delete board;
}

/*
Monte Carlo 方法的原理是，隨機生成的點在正方形區域內是均勻分布的。
也就是說，每個點落在正方形內部的機率是相同的，且機率與點的位置無關。
因此，我們可以假設落在正方形內的每個點都等機率地代表了正方形區域的面積。

而我們感興趣的是圓形區域，所以我們只需要統計有多少點落在圓形區域內。
如果我們隨機生成的點數足夠多，那麼這些點在正方形區域內的分布就能夠很好地近似正方形的總面積。

假設總點數為 N，圓內點數為 M，而正方形區域的面積是 S。根據 Monte Carlo 方法的原理，我們可以得出以下關係：

S = 正方形的總面積 = 4 * R^2
圓形區域的面積 = π * R^2

由於落在正方形區域內的每個點都等機率地代表了正方形區域的面積，而落在圓形區域內的點也應該等機率地代表了圓形區域的面積。

因此，我們有以下比例關係：

M / N = 圓形區域的面積 / 正方形的總面積
M / N = π * R^2 / (4 * R^2)
M / N = π / 4

所以，我們可以通過圓內點數與總點數的比例來估算 π 的值：

π ≈ 4 * (圓內點數 / 總點數)

當模擬的點數 N 越多時，這個估算值會越來越接近真實的 π 值。因此，Monte Carlo 方法的精確度隨著點數的增加而提高。
*/
void MonteCarloSimple(const int iterations) {
    const double R = 1.0; // 正方形邊長的一半，即是圓的半徑

    int circle_points = 0; // 落在圓點內的個數

    random_device rd;
    srand(rd());

    // 進行 Monte Carlo 模擬
    for (int i = 0; i < iterations; ++i) {
        // 隨機生成（x, y）
        double x = (2.0 * R * rand() / RAND_MAX) - R;
        double y = (2.0 * R * rand() / RAND_MAX) - R;

        // 計算（x, y）到圓心的距離
        double distance_squared = x * x + y * y;

        // 判斷點是否在圓內
        if (distance_squared <= R * R) {
            ++circle_points;
        }
    }

    // 估計 π 值
    // 圓型區域和正方形區域的面積比：π * R^2 / (4 * R^2) = π / 4。
    double estimated_pi = 4.0 * ((circle_points) / double(iterations));

    cout << "π : " << estimated_pi << endl;

    return;
}
