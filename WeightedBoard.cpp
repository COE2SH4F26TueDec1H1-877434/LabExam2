#include "WeightedBoard.hpp"

WeightedBoard::WeightedBoard() : ScoreBoard() {}

WeightedBoard::WeightedBoard(int count, int initVal, int extraVal) : ScoreBoard(count, initVal, extraVal) {}

WeightedBoard::WeightedBoard(const WeightedBoard& other) : ScoreBoard(other) {}

WeightedBoard::~WeightedBoard() {}

WeightedBoard& WeightedBoard::operator=(const WeightedBoard& other) {
    if (this != &other) {
        ScoreBoard::operator=(other);
    }
    return *this;
}

// TODO 5: Override totalPoints() (Maximum 8 lines of implementation)
int WeightedBoard::totalPoints() const {
    return static_cast<int>(0);
}
