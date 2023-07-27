#include <iostream>
#include <vector>
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
    const int score(); // draw(0) win(10) not end(-1)
    const vector<int> getValidAction(); // [0, 1, 2, 3, 4, 5, 6]

    Game* clone();

    void print();
};
#endif