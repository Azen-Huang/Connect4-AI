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
    int score(); // draw(0) win(10) others(-1)
    vector<int> getValidAction();
    Game* clone();

    void print();
};
#endif