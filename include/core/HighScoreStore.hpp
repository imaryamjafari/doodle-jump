#pragma once

#include "core/Difficulty.hpp"

class HighScoreStore
{
public:
    HighScoreStore();

    float getHighScore(Difficulty difficulty) const;
    void updateIfNeeded(Difficulty difficulty, float candidateScore);

private:
    int indexFor(Difficulty difficulty) const;

    void load();
    void save() const;

    float scores[3];
};
