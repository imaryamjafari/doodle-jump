#include "monsters/BlueMonster.hpp"
#include "core/Constants.hpp"

BlueMonster::BlueMonster(sf::Texture& texture, const sf::Vector2f& position,
                         float screenWidth, float initialDirection, int initialHealth)
    : Monster(texture, position, GameConfig::MonsterWidth, GameConfig::MonsterHeight,
              screenWidth, initialDirection, initialHealth)
{
    // texture.getSize() is the full two-halves sheet; each half is half
    // its width. Recomputed here (rather than passed in) so this stays
    // correct even if the source image's exact resolution ever changes.
    const sf::Vector2u fullTextureSize = texture.getSize();
    frameWidth = static_cast<int>(fullTextureSize.x) / 2;
    frameHeight = static_cast<int>(fullTextureSize.y);

    // The base Monster constructor already scaled the sprite assuming the
    // FULL texture width maps to GameConfig::MonsterWidth; fix that up now
    // that we know only half the width will ever actually be shown, then
    // select the correct half for the initial direction.
    if (frameWidth > 0 && frameHeight > 0)
    {
        sprite.setScale({GameConfig::MonsterWidth / static_cast<float>(frameWidth),
                         GameConfig::MonsterHeight / static_cast<float>(frameHeight)});
    }

    applyTextureRectForDirection();
}

void BlueMonster::updateFacingVisual()
{
    applyTextureRectForDirection();
}

void BlueMonster::applyTextureRectForDirection()
{
    // Left half of the sheet = left-facing sprite, right half = right-facing.
    const int left = (direction >= 0.f) ? frameWidth : 0;
    sprite.setTextureRect(sf::IntRect({left, 0}, {frameWidth, frameHeight}));
}
