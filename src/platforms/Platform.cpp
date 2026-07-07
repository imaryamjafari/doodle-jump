#include "platforms/Platform.hpp"
#include "core/Constants.hpp"
#include <cstdlib>

Platform::Platform(sf::Texture& texture, const sf::Vector2f& position,
                    float width_, float height_, PlatformType type_)
    : sprite(texture)
    , width(width_)
    , height(height_)
    , type(type_)
    , spring(nullptr)
{
    const sf::Vector2u textureSize = texture.getSize();
    if (textureSize.x > 0 && textureSize.y > 0)
    {
        sprite.setScale({width / static_cast<float>(textureSize.x),
                         height / static_cast<float>(textureSize.y)});
    }

    sprite.setPosition(position);
}

void Platform::update(float deltaTime)
{
    updateMotion(deltaTime);

    if (spring)
    {
        spring->followPlatform(getPosition().x, getPosition().y);
        spring->update(deltaTime);
    }
}

void Platform::attachSpring(sf::Texture& springTexture)
{
    const float springWidth = width * GameConfig::SpringWidthRatio;
    const float springHeight = GameConfig::SpringHeight;
    const float maxOffset = width - springWidth;
    const float randomOffset = maxOffset > 0.f
        ? static_cast<float>(std::rand()) / static_cast<float>(RAND_MAX) * maxOffset
        : 0.f;

    const sf::Vector2f platformPosition = getPosition();
    spring = std::make_unique<Spring>(springTexture, platformPosition.x, platformPosition.y,
                                       randomOffset, springWidth, springHeight);
}

bool Platform::hasSpring() const
{
    return spring != nullptr;
}

Spring* Platform::getSpring() const
{
    return spring.get();
}

void Platform::render(sf::RenderWindow& window) const
{
    window.draw(sprite);
    if (spring)
        spring->render(window);
}

sf::FloatRect Platform::getBounds() const
{
    return sprite.getGlobalBounds();
}

sf::Vector2f Platform::getPosition() const
{
    return sprite.getPosition();
}

void Platform::setPosition(const sf::Vector2f& position)
{
    sprite.setPosition(position);
    if (spring)
        spring->followPlatform(position.x, position.y);
}

PlatformType Platform::getType() const
{
    return type;
}

float Platform::getWidth() const
{
    return width;
}

float Platform::getHeight() const
{
    return height;
}
