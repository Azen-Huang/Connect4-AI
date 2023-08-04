#include <iostream>
#include "gameStrategy.h"
#include "board.h"
#include "bitboard.h"
#include "node.h"
#include "mcts.hpp"
using namespace std;
#define BITBOARD
// #define BOARD
const int EVALUATION_COUNT = 10000;
const int PLAYOUT_COUNT = 200;

int input(char c) {
    return toupper(c) - 'A';
}

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

int main() {
    // DEBUG();
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