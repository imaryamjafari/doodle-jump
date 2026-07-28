#include "world/HoleSpawner.hpp"
#include "world/WorldSpaceOverlap.hpp"
#include "core/Constants.hpp"
#include <cstdlib>
#include <algorithm>

HoleSpawner::HoleSpawner(ResourceManager<sf::Texture>& textures_, float screenWidth_, float screenHeight_)
    : textures(textures_)
    , screenWidth(screenWidth_)
    , screenHeight(screenHeight_)
    , difficulty(GameConfig::DefaultDifficulty)
{
}

void HoleSpawner::reset()
{
    holes.clear();
}

void HoleSpawner::setDifficulty(Difficulty newDifficulty)
{
    difficulty = newDifficulty;
}

void HoleSpawner::maybeSpawnNear(const sf::FloatRect& platformBounds, std::vector<sf::FloatRect>& occupiedRects,
                                 float elapsedPlaySeconds)
{
    // Holes are exclusive to Hard difficulty, per the requirements.
    if (difficulty != Difficulty::Hard)
        return;

    if (elapsedPlaySeconds < GameConfig::MonsterHoleSpawnDelaySeconds)
        return;

    const int roll = std::rand() % GameConfig::PercentRollMax;
    if (roll >= GameConfig::HoleSpawnChancePercent)
        return;

    const bool useSmall = (std::rand() % 2 == 0);
    const float size = useSmall ? GameConfig::HoleSmallSize : GameConfig::HoleLargeSize;

    for (int attempt = 0; attempt < GameConfig::AttemptOverlapCheck; ++attempt)
    {
        const float offsetX = static_cast<float>(std::rand()) / static_cast<float>(RAND_MAX)
            * (screenWidth - size);
        const float candidateY = platformBounds.position.y + platformBounds.size.y + GameConfig::MinClearanceBetweenObjects;
        const sf::FloatRect candidate({offsetX, candidateY}, {size, size});

        if (WorldSpaceOverlap::overlapsAny(candidate, occupiedRects))
            continue;

        holes.push_back(std::make_unique<Hole>(
            textures.get(std::string(GameConfig::AssetsFolder) + GameConfig::HoleTexture),
            candidate.position, size));

        occupiedRects.push_back(candidate);
        return;
    }
}

void HoleSpawner::removeStaleBelow(float cutoffY)
{
    holes.erase(
        std::remove_if(holes.begin(), holes.end(),
            [cutoffY](const std::unique_ptr<Hole>& hole)
            {
                return hole->getPosition().y > cutoffY;
            }),
        holes.end());
}

std::vector<std::unique_ptr<Hole>>& HoleSpawner::getHoles()
{
    return holes;
}

const std::vector<std::unique_ptr<Hole>>& HoleSpawner::getHoles() const
{
    return holes;
}

std::vector<sf::FloatRect> HoleSpawner::getAllBounds() const
{
    std::vector<sf::FloatRect> bounds;
    bounds.reserve(holes.size());
    for (const auto& hole : holes)
        bounds.push_back(hole->getBounds());
    
    return bounds;
}
