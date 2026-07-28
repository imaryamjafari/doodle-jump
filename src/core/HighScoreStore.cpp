#include "core/HighScoreStore.hpp"
#include "core/Constants.hpp"
#include <fstream>

HighScoreStore::HighScoreStore()
    : scores{0.f, 0.f, 0.f}
{
    load();
}

int HighScoreStore::indexFor(Difficulty difficulty) const
{
    return static_cast<int>(difficulty);
}

float HighScoreStore::getHighScore(Difficulty difficulty) const
{
    return scores[indexFor(difficulty)];
}

void HighScoreStore::updateIfNeeded(Difficulty difficulty, float candidateScore)
{
    const int index = indexFor(difficulty);
    if (candidateScore > scores[index])
    {
        scores[index] = candidateScore;
        save();
    }
}

void HighScoreStore::load()
{
    std::ifstream file(GameConfig::HighScoreFile);
    if (file.is_open())
    {
        file >> scores[0] >> scores[1] >> scores[2];
    }
}

void HighScoreStore::save() const
{
    std::ofstream file(GameConfig::HighScoreFile, std::ios::trunc);
    if (file.is_open())
    {
        file << scores[0] << " " << scores[1] << " " << scores[2];
    }
}
