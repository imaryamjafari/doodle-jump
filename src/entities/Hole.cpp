#include "entities/Hole.hpp"

Hole::Hole(sf::Texture& texture, const sf::Vector2f& position, float size_)
    : sprite(texture)
    , size(size_)
{
    const sf::Vector2u textureSize = texture.getSize();
    if (textureSize.x > 0 && textureSize.y > 0)
    {
        sprite.setScale({size / static_cast<float>(textureSize.x),
                         size / static_cast<float>(textureSize.y)});
    }

    sprite.setPosition(position);
}

void Hole::render(sf::RenderWindow& window) const
{
    window.draw(sprite);
}

sf::FloatRect Hole::getBounds() const
{
    return sprite.getGlobalBounds();
}

sf::Vector2f Hole::getPosition() const
{
    return sprite.getPosition();
}

void Hole::setPosition(const sf::Vector2f& position)
{
    sprite.setPosition(position);
}

sf::Vector2f Hole::getCenter() const
{
    const sf::FloatRect bounds = getBounds();
    return sf::Vector2f(bounds.position.x + bounds.size.x / 2.f, bounds.position.y + bounds.size.y / 2.f);
}
