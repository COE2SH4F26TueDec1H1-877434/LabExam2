#ifndef WEIGHTEDBOARD_HPP
#define WEIGHTEDBOARD_HPP

#include "ScoreBoard.hpp"

class WeightedBoard : public ScoreBoard {
public:
    WeightedBoard();
    WeightedBoard(int count, int initVal, int extraVal);
    WeightedBoard(const WeightedBoard& other);
    virtual ~WeightedBoard();
    WeightedBoard& operator=(const WeightedBoard& other);

    virtual int totalPoints() const override;
};

#endif
