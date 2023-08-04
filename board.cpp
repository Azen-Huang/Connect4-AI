#include "board.h"
/*
0  1  2  3  4  5  6
7  8  9  10 11 12 13
14 15 16 17 18 19 20
21 22 23 24 25 26 27 
28 29 30 31 32 33 34
35 36 37 38 39 40 41
*/

Board::Board() 
    : GameStrategy() {
        board.resize(WIDTH * HEIGHT, 0);
        turn = 1;
    }

Board::Board(vector<int> _board, int _turn)
    : GameStrategy() {
        board = _board;
        turn = _turn;
    }

void Board::move(int action) {
    for (int i = WIDTH * (HEIGHT - 1) + action; i >= action; i -= WIDTH) {
        if (board[i] == 0) {
            board[i] = turn;
            break;
        }
    }
    turn = turn == 1 ? 2 : 1;
    return;
}

void Board::undo(int action) {
    for (int i = WIDTH * (HEIGHT - 1) + action; i >= action; i -= WIDTH) {
        if (board[i] == 0) {
            board[i] = 0;
            break;
        }
    }
    turn = turn == 1 ? 2 : 1;
    return;
}

int Board::score(int action) { // draw(0) win(10) not end(-1)
    // 檢查水平方向是否有四個連續的棋子
    int current_turn = turn == 1 ? 2 : 1;
    for (int row = 0; row < HEIGHT; ++row) {
        for (int col = 0; col < WIDTH - 3; ++col) {
            int consecutiveCount = 0;
            for (int i = 0; i < 4; ++i) {
                if (board[row * WIDTH + col + i] == current_turn) {
                    consecutiveCount++;
                }
            }
            if (consecutiveCount == 4) {
                return 1;
            }
        }
    }

    // 檢查垂直方向是否有四個連續的棋子
    for (int row = 0; row < HEIGHT - 3; ++row) {
        for (int col = 0; col < WIDTH; ++col) {
            int consecutiveCount = 0;
            for (int i = 0; i < 4; ++i) {
                if (board[(row + i) * WIDTH + col] == current_turn) {
                    consecutiveCount++;
                }
            }
            if (consecutiveCount == 4) {
                return 1;
            }
        }
    }

    // 檢查斜向（左上到右下）是否有四個連續的棋子
    for (int row = 0; row < HEIGHT - 3; ++row) {
        for (int col = 0; col < WIDTH - 3; ++col) {
            int consecutiveCount = 0;
            for (int i = 0; i < 4; ++i) {
                if (board[(row + i) * WIDTH + col + i] == current_turn) {
                    consecutiveCount++;
                }
            }
            if (consecutiveCount == 4) {
                return 1;
            }
        }
    }

    // 檢查斜向（左下到右上）是否有四個連續的棋子
    for (int row = 3; row < HEIGHT; ++row) {
        for (int col = 0; col < WIDTH - 3; ++col) {
            int consecutiveCount = 0;
            for (int i = 0; i < 4; ++i) {
                if (board[(row - i) * WIDTH + col + i] == current_turn) {
                    consecutiveCount++;
                }
            }
            if (consecutiveCount == 4) {
                return 1;
            }
        }
    }

    for (int i = 0; i < WIDTH; ++i) {
        if (board[i] == 0) {
            return -1; // 若遊戲還沒結束
        }
    }

    return 0; //平局
} 

vector<int> Board::getValidAction() const {
    vector<int> validActions;
    // vector<int> moveOrdering{3, 2, 4, 1, 5, 0, 6};
    // for (const auto& i : moveOrdering) {
    for (int i = 0; i < WIDTH; ++i) {
        if (board[i] == 0) {
            validActions.emplace_back(i);
        }
    }
    return validActions;
}

Board* Board::clone() const {
    return new Board(board, turn);
}

string Board::getKey() const {
    string s = "";
    for (auto& v : board) s += to_string(v);
    return s;
}

void Board::print() const {
    for (char c = 'A'; c < 'A' + WIDTH; ++c) {
        cout << "  " << c << " ";
    }
    cout << endl;
    cout << "-----------------------------" << endl;
    for (int i = 0; i < HEIGHT; ++i) {
        cout << "|";
        for (int j = 0; j < WIDTH; ++j) {
            char stone;
            switch (board[i * WIDTH + j]) {
                case 0:
                    stone = '-';
                    break;
                case 1:
                    stone = 'O';
                    break;
                case 2:
                    stone = 'X';
                    break;
                default:
                    break;
            }
            cout << " " << stone << " |";
        }
        cout << endl;
        
    }
    cout << "-----------------------------" << endl;
    return;
}