#include "entities/Spring.hpp"
#include "core/Constants.hpp"

Spring::Spring(sf::Texture& texture, float platformX, float platformY,
               float localOffsetX_, float width_, float height_)
    : sprite(texture)
    , width(width_)
    , height(height_)
    , localOffsetX(localOffsetX_)
    , extended(false)
    , extendedTimeRemaining(0.f)
{
    applyCompressedFrame();
    sprite.setPosition({platformX + localOffsetX, platformY - height});
}

void Spring::applyCompressedFrame()
{
    sprite.setTextureRect(sf::IntRect(
        {0, GameConfig::SpringCompressedTop},
        {GameConfig::SpringSheetWidth, GameConfig::SpringCompressedHeight}));

    const sf::Vector2f textureSize(static_cast<float>(GameConfig::SpringSheetWidth),
                                    static_cast<float>(GameConfig::SpringCompressedHeight));
    sprite.setScale({width / textureSize.x, height / textureSize.y});
}

void Spring::applyExtendedFrame()
{
    sprite.setTextureRect(sf::IntRect(
        {0, GameConfig::SpringExtendedTop},
        {GameConfig::SpringSheetWidth, GameConfig::SpringExtendedHeight}));

    const sf::Vector2f textureSize(static_cast<float>(GameConfig::SpringSheetWidth),
                                    static_cast<float>(GameConfig::SpringExtendedHeight));
    sprite.setScale({width / textureSize.x, height / textureSize.y});
}

void Spring::triggerLaunch()
{
    extended = true;
    extendedTimeRemaining = GameConfig::SpringExtendedDurationSeconds;
    applyExtendedFrame();
}

void Spring::update(float deltaTime)
{
    if (extended)
    {
        extendedTimeRemaining -= deltaTime;
        if (extendedTimeRemaining <= 0.f)
        {
            extended = false;
            applyCompressedFrame();
        }
    }
}

void Spring::render(sf::RenderWindow& window) const
{
    window.draw(sprite);
}

sf::FloatRect Spring::getBounds() const
{
    return sprite.getGlobalBounds();
}

sf::Vector2f Spring::getPosition() const
{
    return sprite.getPosition();
}

void Spring::setPosition(const sf::Vector2f& position)
{
    sprite.setPosition(position);
}

void Spring::followPlatform(float platformX, float platformY)
{
    sprite.setPosition({platformX + localOffsetX, platformY - height});
}
