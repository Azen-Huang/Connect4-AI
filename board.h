<<<<<<< HEAD
#include "gameStrategy.h"

class Board : public GameStrategy<vector<int>> {
=======
#include <vector>
#include <iostream>
using namespace std;
#define WIDTH
class Board {
>>>>>>> 3f2ee43effbe57219e2950e28057929d9ba81a1d
public:
    Board();
    Board(vector<int> _board, int _turn);
    
<<<<<<< HEAD
    void move(int action) override;
    void undo(int action) override;
    int score(int action) override; // draw(0) win(10) not end(-1)
    vector<int> getValidAction() const override; // [0, 1, 2, 3, 4, 5, 6]
    Board* clone() const override;
    string getKey() const override;
    void print() const override;
=======
    void move(int action);
    void undo(int action);
    int score(int action) const; // draw(0) win(10) not end(-1)
    vector<int> getValidAction() const; // [0, 1, 2, 3, 4, 5, 6]
    Board* clone() const;
    string getKey() const;
    void print() const;
>>>>>>> 3f2ee43effbe57219e2950e28057929d9ba81a1d
};