#include "world/MonsterSpawner.hpp"
#include "world/WorldSpaceOverlap.hpp"
#include "core/Constants.hpp"
#include "monsters/BlueMonster.hpp"
#include "monsters/GreenMonster.hpp"
#include <cstdlib>
#include <algorithm>

MonsterSpawner::MonsterSpawner(ResourceManager<sf::Texture>& textures_, float screenWidth_, float screenHeight_)
    : textures(textures_)
    , screenWidth(screenWidth_)
    , screenHeight(screenHeight_)
    , difficulty(GameConfig::DefaultDifficulty)
{
}

void MonsterSpawner::reset()
{
    monsters.clear();
}

void MonsterSpawner::setDifficulty(Difficulty newDifficulty)
{
    difficulty = newDifficulty;
}

int MonsterSpawner::currentMonsterHealth() const
{
    switch (difficulty)
    {
        case Difficulty::Easy:   return GameConfig::MonsterHealthEasy;
        case Difficulty::Medium: return GameConfig::MonsterHealthMedium;
        case Difficulty::Hard:   return GameConfig::MonsterHealthHard;
    }
    return GameConfig::MonsterHealthEasy;
}

void MonsterSpawner::maybeSpawnNear(const sf::FloatRect& platformBounds, std::vector<sf::FloatRect>& occupiedRects,
                                    float elapsedPlaySeconds)
{
    // Never spawn monsters right at the start of a session: give the
    // player a few seconds to get moving first.
    if (elapsedPlaySeconds < GameConfig::MonsterHoleSpawnDelaySeconds)
        return;

    const int roll = std::rand() % GameConfig::PercentRollMax;
    if (roll >= GameConfig::MonsterSpawnChancePercent)
        return;

    const bool useBlue = (std::rand() % 2 == 0);
    const float height = useBlue ? GameConfig::MonsterHeight : GameConfig::GreenMonsterHeight;
    const float width = GameConfig::MonsterWidth;

    // Try a handful of horizontal offsets near the platform; keep the
    // first one that doesn't overlap anything else. If none work, simply
    // skip spawning this time rather than force an overlapping placement.
    for (int attempt = 0; attempt < GameConfig::AttemptOverlapCheck; ++attempt)
    {
        const float offsetX = static_cast<float>(std::rand()) / static_cast<float>(RAND_MAX)
            * (screenWidth - width);
        const float candidateY = platformBounds.position.y - height - GameConfig::MinClearanceBetweenObjects;
        const sf::FloatRect candidate({offsetX, candidateY}, {width, height});

        if (WorldSpaceOverlap::overlapsAny(candidate, occupiedRects))
            continue;

        const float initialDirection = (std::rand() % 2 == 0) ? 1.f : -1.f;
        const int health = currentMonsterHealth();

        std::unique_ptr<Monster> monster = useBlue
            ? std::unique_ptr<Monster>(std::make_unique<BlueMonster>(
                  textures.get(std::string(GameConfig::AssetsFolder) + GameConfig::BlueMonsterTexture),
                  candidate.position, screenWidth, initialDirection, health))
            : std::unique_ptr<Monster>(std::make_unique<GreenMonster>(
                  textures.get(std::string(GameConfig::AssetsFolder) + GameConfig::GreenMonsterTexture),
                  candidate.position, screenWidth, initialDirection, health));

        occupiedRects.push_back(candidate); // so later spawns this frame see it too
        monsters.push_back(std::move(monster));
        return;
    }
}

void MonsterSpawner::updateAll(float deltaTime)
{
    for (auto& monster : monsters)
    {
        monster->update(deltaTime);
    }
}

void MonsterSpawner::removeStaleBelow(float cutoffY)
{
    monsters.erase(
        std::remove_if(monsters.begin(), monsters.end(),
            [cutoffY](const std::unique_ptr<Monster>& monster)
            {
                return !monster->isAlive() || monster->getPosition().y > cutoffY;
            }),
        monsters.end());
}

std::vector<std::unique_ptr<Monster>>& MonsterSpawner::getMonsters()
{
    return monsters;
}

const std::vector<std::unique_ptr<Monster>>& MonsterSpawner::getMonsters() const
{
    return monsters;
}

std::vector<sf::FloatRect> MonsterSpawner::getAllBounds() const
{
    std::vector<sf::FloatRect> bounds;
    bounds.reserve(monsters.size());
    for (const auto& monster : monsters)
    {
        bounds.push_back(monster->getBounds());
    }
    return bounds;
}
