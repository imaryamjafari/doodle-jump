#include "entities/Bullet.hpp"
#include "core/Constants.hpp"

Bullet::Bullet(const sf::Vector2f& startPosition)
{
    shape.setRadius(GameConfig::BulletRadius);
    shape.setFillColor(sf::Color(60, 60, 60));

    // startPosition is where the nose's tip ends; setOrigin to the
    // circle's center so the bullet is centered exactly there instead of
    // having that point be its top-left corner.
    shape.setOrigin({GameConfig::BulletRadius, GameConfig::BulletRadius});
    shape.setPosition(startPosition);
}

void Bullet::update(float deltaTime)
{
    shape.move({0.f, -GameConfig::BulletSpeed * deltaTime});
}

void Bullet::render(sf::RenderWindow& window) const
{
    window.draw(shape);
}

sf::FloatRect Bullet::getBounds() const
{
    return shape.getGlobalBounds();
}

sf::Vector2f Bullet::getPosition() const
{
    return shape.getPosition();
}

void Bullet::setPosition(const sf::Vector2f& position)
{
    shape.setPosition(position);
}
