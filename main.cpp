#include <iostream>
#include "game.h"
#include "node.h"
#include "mcts.h"
using namespace std;
const int EVALUATION_COUNT = 50000;
const int PLAYOUT_COUNT = 1000;

int input(char c) {
    return toupper(c) - 'A';
}

void AI_vs_AI() {
    Game* board = new Game();
    int score;
    board->move(3);
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
    string command;
    bool firstPlayerturn = true;
    cout << "1. Human vs. Human" << endl;
    cout << "2. Human vs. AI" << endl;
    cout << "3. AI vs AI" << endl;
    cout << "Input Command: ";
    while (getline(cin, command)) {
        if (command == "1") {
            cout << "Start Human vs. Human" << endl;
            Human_vs_Human();
        }
        else if (command == "2") {
            cout << "Start Human vs. AI" << endl;
            Human_vs_AI();
        }
        else if (command == "3") {
            cout << "Start AI vs. AI" << endl;
            AI_vs_AI();
        }
        else {
            exit(0);
        }
        
    }
    return 0;
}