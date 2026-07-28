#include "monsters/Monster.hpp"
#include "core/Constants.hpp"

Monster::Monster(sf::Texture& texture, const sf::Vector2f& position,
                  float width, float height, float screenWidth_, float initialDirection, int initialHealth)
    : sprite(texture)
    , screenWidth(screenWidth_)
    , direction(initialDirection >= 0.f ? 1.f : -1.f)
    , health(initialHealth)
{
    const sf::Vector2u textureSize = texture.getSize();
    if (textureSize.x > 0 && textureSize.y > 0)
    {
        sprite.setScale({width / static_cast<float>(textureSize.x),
                         height / static_cast<float>(textureSize.y)});
    }

    sprite.setPosition(position);
}

void Monster::update(float deltaTime)
{
    // Identical rule to MovingPlatform: patrol horizontally, reverse at
    // the screen edges.
    sprite.move({direction * GameConfig::MonsterSpeed * deltaTime, 0.f});

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

    updateFacingVisual();
}

void Monster::updateFacingVisual()
{
    // Default: nothing to do (e.g. GreenMonster's single sprite doesn't
    // need to change based on direction). BlueMonster overrides this.
}

void Monster::applyBulletHit()
{
    if (health > 0)
    {
        --health;
    }
}

bool Monster::isAlive() const
{
    return health > 0;
}

void Monster::render(sf::RenderWindow& window) const
{
    window.draw(sprite);
}

sf::FloatRect Monster::getBounds() const
{
    return sprite.getGlobalBounds();
}

sf::Vector2f Monster::getPosition() const
{
    return sprite.getPosition();
}

void Monster::setPosition(const sf::Vector2f& position)
{
    sprite.setPosition(position);
}
