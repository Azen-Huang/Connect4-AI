#include <iostream>
#include <vector>
#include <numeric>
using namespace std;
#define WIDTH 7
#define HEIGHT 6

#ifndef GAME_H 
#define GAME_H 
class Game {
public:
    vector<int> board;
    int turn;
    
    Game();
    Game(vector<int> _board, int _turn);

    void move(int action);
    void undo(int action);
    int score() const; // draw(0) win(10) not end(-1)
    vector<int> getValidAction() const; // [0, 1, 2, 3, 4, 5, 6]
    string key() const; 
    Game* clone() const;

    void print() const;
};
#endif