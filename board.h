#include <vector>
#include <iostream>
using namespace std;
#define WIDTH
class Board {
public:
    Board();
    Board(vector<int> _board, int _turn);
    
    void move(int action);
    void undo(int action);
    int score(int action) const; // draw(0) win(10) not end(-1)
    vector<int> getValidAction() const; // [0, 1, 2, 3, 4, 5, 6]
    Board* clone() const;
    string getKey() const;
    void print() const;
};