#ifndef SCOREBOARD_HPP
#define SCOREBOARD_HPP

#include <iostream>
using namespace std;

class ScoreBoard {
protected:
    int* scores;
    int numGames;
    int bonusMultiplier;

public:
    ScoreBoard();
    ScoreBoard(int count, int initVal, int extraVal);
    ScoreBoard(const ScoreBoard& other);
    virtual ~ScoreBoard();
    ScoreBoard& operator=(const ScoreBoard& other);

    int getScore(int index) const;
    virtual int totalPoints() const;
};

#endif
