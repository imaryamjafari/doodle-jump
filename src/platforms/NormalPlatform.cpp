#include "platforms/NormalPlatform.hpp"

NormalPlatform::NormalPlatform(sf::Texture& texture, const sf::Vector2f& position,
                               float width, float height)
    : Platform(texture, position, width, height, PlatformType::Normal)
{}

void NormalPlatform::updateMotion(float /*deltaTime*/)
{}

bool NormalPlatform::onLand()
{
    return true; 
}

bool NormalPlatform::isAlive() const
{
    return true; 
}
