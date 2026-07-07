#include "platforms/BreakablePlatform.hpp"
#include "core/Constants.hpp"

BreakablePlatform::BreakablePlatform(sf::Texture& texture, const sf::Vector2f& position,
                                       float width, float height)
    : Platform(texture, position, width, height, PlatformType::Breakable)
    , broken(false)
    , fellOffScreen(false)
{}

void BreakablePlatform::updateMotion(float deltaTime)
{
    if (broken)
    {
        sprite.move({0.f, GameConfig::BreakableFallSpeed * deltaTime});

        if (sprite.getPosition().y > static_cast<float>(GameConfig::WindowHeight) * GameConfig::BreakablePlatformDespawnMultiplier)
            fellOffScreen = true;
    }
}

bool BreakablePlatform::onLand()
{
    broken = true;
    return false;
}

bool BreakablePlatform::isAlive() const
{
    return !fellOffScreen;
}

bool BreakablePlatform::hasBroken() const
{
    return broken;
}

float BreakablePlatform::getFallSpeed() const
{
    return GameConfig::BreakableFallSpeed;
}
