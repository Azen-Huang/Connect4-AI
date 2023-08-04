#include "gameStrategy.h"

class Board : public GameStrategy<vector<int>> {
public:
    Board();
    Board(vector<int> _board, int _turn);
    
    void move(int action) override;
    void undo(int action) override;
    int score(int action) override; // draw(0) win(10) not end(-1)
    vector<int> getValidAction() const override; // [0, 1, 2, 3, 4, 5, 6]
    Board* clone() const override;
    string getKey() const override;
    void print() const override;
};