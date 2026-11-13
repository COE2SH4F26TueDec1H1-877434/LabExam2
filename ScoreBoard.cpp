#include "ScoreBoard.hpp"

ScoreBoard::ScoreBoard() : scores(nullptr), numGames(0), bonusMultiplier(3) {}

ScoreBoard::ScoreBoard(int count, int initVal, int extraVal) : scores(nullptr), numGames(count), bonusMultiplier(extraVal) {
    if (numGames > 0) {
        scores = new int[numGames];
        for (int i = 0; i < numGames; ++i) {
            scores[i] = initVal + static_cast<int>(i);
        }
    } else {
        numGames = 0;
    }
}

// TODO 1: Complete the Copy Constructor (Deep Copy)
ScoreBoard::ScoreBoard(const ScoreBoard& other) {
    (void)other;
}

// TODO 2: Complete the Destructor (Release DMA)
ScoreBoard::~ScoreBoard() {
}

// TODO 3: Complete the Copy Assignment Operator (Deep Copy + Self-Assignment Guard)
ScoreBoard& ScoreBoard::operator=(const ScoreBoard& other) {
    (void)other;
    return *this;
}

int ScoreBoard::getScore(int index) const {
    if (index >= 0 && index < numGames && scores != nullptr) {
        return scores[index];
    }
    return static_cast<int>(0);
}

// TODO 4: Implement totalPoints() (Maximum 8 lines of implementation)
int ScoreBoard::totalPoints() const {
    return static_cast<int>(0);
}
