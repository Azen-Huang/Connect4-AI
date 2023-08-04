#include "bitboard.h"

#define WIDTH 7
#define HEIGHT 6

static uint64_t top_mask(int col) {
    return (UINT64_C(1) << (HEIGHT - 1)) << col*(HEIGHT+1);
}

static uint64_t bottom_mask(int col) {
    return UINT64_C(1) << col*(HEIGHT+1);
}

static uint64_t column_mask(int col) {
    return ((UINT64_C(1) << HEIGHT)-1) << col*(HEIGHT+1);
}

static bool alignment(uint64_t pos) {
        // horizontal 
        uint64_t m = pos & (pos >> (HEIGHT+1));
        if(m & (m >> (2*(HEIGHT+1)))) return true;

        // diagonal 1
        m = pos & (pos >> HEIGHT);
        if(m & (m >> (2*HEIGHT))) return true;

        // diagonal 2 
        m = pos & (pos >> (HEIGHT+2));
        if(m & (m >> (2*(HEIGHT+2)))) return true;

        // vertical;
        m = pos & (pos >> 1);
        if(m & (m >> 2)) return true;

        return false;
}

static void print_bit(uint64_t bitboard) {
    bitset<64> board_bit(bitboard);
    string board = board_bit.to_string();
    for (int i = HEIGHT; i >= 0; --i) {
        for (int j = 0; j < WIDTH; ++j) {
            int index = i + j * WIDTH;
            cout << board[index];
        }
        cout << endl;
    }
    cout << endl;
    return;
}

BitBoard::BitBoard() 
    : GameStrategy(), moves(0) {
        history.resize(WIDTH * HEIGHT, -1);
        board = {0, 0};
        turn = 1;
    }

BitBoard::BitBoard(bitboard _board, int _turn, int _moves, vector<int> _history)
    : GameStrategy() {
        board.mask = _board.mask;
        board.current_position = _board.current_position;
        turn = _turn;
        moves = _moves;
        history = _history;
    }

void BitBoard::move(int action) {
    history[moves++] = action;
    uint64_t current_position = 0, mask = 0;
    for (int i = 0; i < moves; ++i) {
        current_position ^= mask;
        mask |= mask + bottom_mask(history[i]);
    }
    board.current_position = current_position;
    board.mask = mask;
    turn = turn == 1 ? 2 : 1;
}

void BitBoard::undo(int action) {
    --moves;
    uint64_t current_position = 0, mask = 0;
    for (int i = 0; i < moves; ++i) {
        current_position ^= mask;
        mask |= mask + bottom_mask(history[i]);
    }
    board.current_position = current_position;
    board.mask = mask;
    turn = turn == 1 ? 2 : 1;
    return;
}

int BitBoard::score(int col) { // draw(0) win(10) not end(-1)
    if (moves == HEIGHT * WIDTH) {
        return 0;
    }
    undo(0);
    uint64_t pos = board.current_position; 
    pos |= (board.mask + bottom_mask(col)) & column_mask(col);
    move(col);
    if (alignment(pos)) {
        return 1;
    }
    return -1;
}

bool BitBoard::CanPlay(int col) const {
    return (board.mask & top_mask(col)) == 0;
}

vector<int> BitBoard::getValidAction() const { // [0, 1, 2, 3, 4, 5, 6]
    vector<int> validActions;
    for (int i = 0; i < WIDTH; ++i) {
        if (CanPlay(i)) {
            validActions.emplace_back(i);
        }
    }
    return validActions;
}

BitBoard* BitBoard::clone() const {
    return new BitBoard(board, turn, moves, history);
}

string BitBoard::getKey() const {
    return to_string(board.current_position + board.mask);
}
void BitBoard::print() const {
    bitset<64> bit_current_position(board.current_position);
    string current_position = bit_current_position.to_string();

    bitset<64> bit_mask(board.mask);
    string mask = bit_mask.to_string();
    cout << "current_position: " << current_position << endl;
    cout << "mask: " << mask << endl;
    reverse(mask.begin(), mask.end());
    reverse(current_position.begin(), current_position.end());
    
    for (char c = 'A'; c < 'A' + WIDTH; ++c) {
        cout << "  " << c << " ";
    }
    cout << endl;
    cout << "-----------------------------" << endl;
    for (int i = HEIGHT; i >= 0; --i) {
        cout << "|";
        for (int j = 0; j < WIDTH; ++j) {
            int index = i + j * WIDTH;
            char stone;
            if (mask[index] == '1' && mask[index] == current_position[index]) {
                stone = turn == 1 ? 'O' : 'X';
            }
            else if (mask[index] == '1' && mask[index] != current_position[index]){
                stone = turn == 1 ? 'X' : 'O';
            }
            else {
                stone = '-';
            }
            cout << " " << stone << " |";
        }
        cout << endl;
    }
    cout << "-----------------------------" << endl;
}