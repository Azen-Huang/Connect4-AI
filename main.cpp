#include <iostream>
#include "game.h"
#include "node.h"
#include "mcts.h"
using namespace std;
const int EVALUATION_COUNT = 5000;
const int PLAYOUT_COUNT = 10;
int input(char c) {
    return toupper(c) - 'A';
}

void AI_vs_AI() {
    Game* board = new Game();
    int score;
    board->print();
    while (true) {
        MCTS mcts = MCTS(board, EVALUATION_COUNT, PLAYOUT_COUNT);
        int action = mcts.getNxtAction();
        board->move(action);
        board->print();
        score = board->score();
        if (score != -1) {
            break;
        }
    }

    char winner = board->turn == 1 ? 'X' : 'O';
    if (score == 0) {
        cout << "Draw" << endl;
    }
    else {
        cout << "Winner is " << winner << "." << endl;
    }
    return;

}

void Human_vs_AI() {
    Game* board = new Game();
    string strAction;
    int action;
    int score;
    board->print();

    MCTS mcts = MCTS(board, EVALUATION_COUNT, PLAYOUT_COUNT);
    // action = mcts.getNxtAction();
    board->move(3);
    board->print();

    while (getline(cin, strAction)) {
        action = input(strAction[0]);
        board->move(action);
        board->print();
        score = board->score();
        if (score != -1) {
            string win_turn = board->turn == 1 ? "X" : "O";
            cout << win_turn << " Win." << endl;
            break;
        }

        MCTS mcts = MCTS(board, EVALUATION_COUNT, PLAYOUT_COUNT);
        action = mcts.getNxtAction();
        board->move(action);
        board->print();
        score = board->score();
        if (score != -1) {
            string win_turn = board->turn == 1 ? "X" : "O";
            cout << win_turn << " Win." << endl;
            break;
        }
    }
}
void Human_vs_Human() {
    Game* board = new Game();
    string strAction;
    board->print();
    while (getline(cin, strAction)) {
        int action = input(strAction[0]);
        board->move(action);
        board->print();
        vector<int> validAction = board->getValidAction();
        cout << "[";
        for (auto& action : validAction) {
            cout << action << ", ";
        }
        cout << "]" << endl;
        int score = board->score();
        if (score != -1) {
            string win_turn = board->turn == 1 ? "X" : "O";
            cout << win_turn << " Win." << endl;
            break;
        }
    }
}

int main() {
    // Human_vs_Human();
    // Human_vs_AI();
    AI_vs_AI();

    return 0;
}