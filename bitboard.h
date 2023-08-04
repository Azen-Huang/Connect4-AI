#include "gameStrategy.h"
#include <cstdint>
#include <bitset>

struct bitboard {
    uint64_t current_position = 0;
    uint64_t mask = 0;
};

class BitBoard : public GameStrategy<bitboard> {
public:
    BitBoard();
    BitBoard(bitboard _board, int _turn, int _moves, vector<int> history);
    
    void move(int action) override;
    void undo(int action) override;
    int score(int action) override; // draw(0) win(10) not end(-1)
    vector<int> getValidAction() const override; // [0, 1, 2, 3, 4, 5, 6]
    BitBoard* clone() const override;
    string getKey() const override;
    void print() const override;

private:
    int moves;
    vector<int> history;
    bool CanPlay(int col) const;
};
