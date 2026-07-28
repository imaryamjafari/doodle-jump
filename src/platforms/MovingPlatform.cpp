#include "platforms/MovingPlatform.hpp"
#include "core/Constants.hpp"

namespace
{
    float speedForDifficulty(Difficulty difficulty)
    {
        switch (difficulty)
        {
            case Difficulty::Easy:   return GameConfig::MovingPlatformSpeedEasy;
            case Difficulty::Medium: return GameConfig::MovingPlatformSpeedMedium;
            case Difficulty::Hard:   return GameConfig::MovingPlatformSpeedHard;
        }
        return GameConfig::MovingPlatformSpeedEasy;
    }
}

MovingPlatform::MovingPlatform(sf::Texture& texture, const sf::Vector2f& position,
                                 float width, float height, float screenWidth_, float initialDirection,
                                 Difficulty difficulty)
    : Platform(texture, position, width, height, PlatformType::Moving)
    , screenWidth(screenWidth_)
    , direction(initialDirection >= 0.f ? 1.f : -1.f)
    , speed(speedForDifficulty(difficulty))
{}

void MovingPlatform::updateMotion(float deltaTime)
{
    sprite.move({direction * speed * deltaTime, 0.f});

    const sf::FloatRect bounds = sprite.getGlobalBounds();
    if (bounds.position.x <= 0.f)
    {
        sprite.setPosition({0.f, sprite.getPosition().y});
        direction = 1.f;
    }
    else if (bounds.position.x + bounds.size.x >= screenWidth)
    {
        sprite.setPosition({screenWidth - bounds.size.x, sprite.getPosition().y});
        direction = -1.f;
    }
}

bool MovingPlatform::onLand()
{
    return true;
}

bool MovingPlatform::isAlive() const
{
    return true;
}
