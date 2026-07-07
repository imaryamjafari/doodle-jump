#include "world/PlatformSpawner.hpp"
#include "core/Constants.hpp"
#include "platforms/NormalPlatform.hpp"
#include "platforms/MovingPlatform.hpp"
#include "platforms/BreakablePlatform.hpp"
#include <cstdlib>
#include <algorithm>

PlatformSpawner::PlatformSpawner(ResourceManager<sf::Texture>& textures_, float screenWidth_, float screenHeight_)
    : textures(textures_)
    , screenWidth(screenWidth_)
    , screenHeight(screenHeight_)
    , highestGeneratedY(0.f)
    , lastPlatformWasBreakable(false)
{
}

PlatformType PlatformSpawner::pickRandomPlatformType() const
{
    const int totalWeight = GameConfig::NormalPlatformWeight
                           + GameConfig::MovingPlatformWeight
                           + GameConfig::BreakablePlatformWeight;

    const int roll = std::rand() % totalWeight;

    if (roll < GameConfig::NormalPlatformWeight)
    {
        return PlatformType::Normal;
    }
    if (roll < GameConfig::NormalPlatformWeight + GameConfig::MovingPlatformWeight)
    {
        return PlatformType::Moving;
    }
    return PlatformType::Breakable;
}

PlatformType PlatformSpawner::pickNextPlatformType() const
{
    PlatformType type = pickRandomPlatformType();

    if (type == PlatformType::Breakable && lastPlatformWasBreakable)
        type = (std::rand() % 2 == 0) ? PlatformType::Normal : PlatformType::Moving;

    return type;
}

std::unique_ptr<Platform> PlatformSpawner::createPlatform(PlatformType type, const sf::Vector2f& position)
{
    switch (type)
    {
        case PlatformType::Normal:
            return std::make_unique<NormalPlatform>(
                textures.get(std::string(GameConfig::AssetsFolder) + GameConfig::NormalPlatformTexture),
                position, GameConfig::PlatformWidth, GameConfig::PlatformHeight);

        case PlatformType::Moving:
        {
            const float initialDirection = (std::rand() % 2 == 0) ? 1.f : -1.f;
            return std::make_unique<MovingPlatform>(
                textures.get(std::string(GameConfig::AssetsFolder) + GameConfig::MovingPlatformTexture),
                position, GameConfig::PlatformWidth, GameConfig::PlatformHeight, screenWidth, initialDirection);
        }

        case PlatformType::Breakable:
        default:
            return std::make_unique<BreakablePlatform>(
                textures.get(std::string(GameConfig::AssetsFolder) + GameConfig::BrokenPlatformTexture),
                position, GameConfig::PlatformWidth, GameConfig::PlatformHeight);
    }
}

void PlatformSpawner::maybeAttachSpring(Platform& platform)
{
    if (platform.getType() == PlatformType::Breakable)
        return;

    const int springRoll = std::rand() % GameConfig::PercentRollMax;
    if (springRoll < GameConfig::SpringSpawnChancePercent)
        platform.attachSpring(textures.get(std::string(GameConfig::AssetsFolder) + GameConfig::SpringTexture));
}

void PlatformSpawner::reset()
{
    platforms.clear();
    highestGeneratedY = 0.f;
    lastPlatformWasBreakable = false;

    const float firstX = screenWidth / 2.f - GameConfig::PlatformWidth / 2.f;
    const float firstY = screenHeight - GameConfig::PlatformHeight - GameConfig::FirstPlatformBottomMargin;

    platforms.push_back(std::make_unique<NormalPlatform>(
        textures.get(std::string(GameConfig::AssetsFolder) + GameConfig::NormalPlatformTexture),
        sf::Vector2f(firstX, firstY), GameConfig::PlatformWidth, GameConfig::PlatformHeight));
    highestGeneratedY = firstY;

    generateUpTo(-screenHeight);
}

void PlatformSpawner::generateUpTo(float topWorldY)
{
    while (highestGeneratedY > topWorldY)
    {
        const float verticalGap = GameConfig::MinVerticalGapBetweenPlatforms
            + static_cast<float>(std::rand()) / static_cast<float>(RAND_MAX)
            * (GameConfig::MaxVerticalGapBetweenPlatforms - GameConfig::MinVerticalGapBetweenPlatforms);

        const float newY = highestGeneratedY - verticalGap;

        const float maxX = screenWidth - GameConfig::PlatformWidth;
        const float newX = maxX > 0.f
            ? static_cast<float>(std::rand()) / static_cast<float>(RAND_MAX) * maxX
            : 0.f;

        const PlatformType type = pickNextPlatformType();
        std::unique_ptr<Platform> platform = createPlatform(type, sf::Vector2f(newX, newY));
        maybeAttachSpring(*platform);
        platforms.push_back(std::move(platform));

        highestGeneratedY = newY;
        lastPlatformWasBreakable = (type == PlatformType::Breakable);
    }
}

void PlatformSpawner::updateAll(float deltaTime)
{
    for (auto& platform : platforms)
        platform->update(deltaTime);
}

void PlatformSpawner::removeStaleBelow(float cutoffY)
{
    platforms.erase(
        std::remove_if(platforms.begin(), platforms.end(),
            [cutoffY](const std::unique_ptr<Platform>& platform)
            {
                return !platform->isAlive() || platform->getPosition().y > cutoffY;
            }),
        platforms.end());
}

std::vector<std::unique_ptr<Platform>>& PlatformSpawner::getPlatforms()
{
    return platforms;
}

const std::vector<std::unique_ptr<Platform>>& PlatformSpawner::getPlatforms() const
{
    return platforms;
}

sf::FloatRect PlatformSpawner::getFirstPlatformBounds() const
{
    if (platforms.empty())
        return sf::FloatRect({0.f, 0.f}, {0.f, 0.f});

    return platforms.front()->getBounds();
}
