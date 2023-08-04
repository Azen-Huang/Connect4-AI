#include "util.hpp"

int main() {
    // MonteCarloSimple(10000);
    // DEBUG();
    string command;
    bool firstPlayerturn = true;
    cout << "1. Human vs. Human" << endl;
    cout << "2. Human vs. AI" << endl;
    cout << "3. AI vs AI" << endl;
    cout << "Input Command: ";
    while (getline(cin, command)) {
        if (command == "1") {
            cout << "Start Human vs. Human" << endl;
            Human_vs_Human();
        }
        else if (command == "2") {
            cout << "Start Human vs. AI" << endl;
            Human_vs_AI();
        }
        else if (command == "3") {
            cout << "Start AI vs. AI" << endl;
            AI_vs_AI();
        }
        else {
            exit(0);
        }
        
    }
    return 0;
}