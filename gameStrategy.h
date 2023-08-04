#include <vector>
#include <iostream>
#include <numeric>

using namespace std;
#define WIDTH 7
#define HEIGHT 6

#ifndef GAMESTRATEGY_H 
#define GAMESTRATEGY_H 
template <typename BoardType>
class GameStrategy {
public:
    int turn;
    BoardType board;

    virtual void move(int action) = 0;
    virtual void undo(int action) = 0;
    virtual int score(int action) = 0;
    virtual vector<int> getValidAction() const = 0;
    virtual GameStrategy* clone() const = 0;
    virtual string getKey() const = 0;
    virtual void print() const = 0;
    
    virtual ~GameStrategy() {}
};
#endif