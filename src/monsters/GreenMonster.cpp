#include "monsters/GreenMonster.hpp"
#include "core/Constants.hpp"

GreenMonster::GreenMonster(sf::Texture& texture, const sf::Vector2f& position,
                           float screenWidth, float initialDirection, int initialHealth)
    : Monster(texture, position, GameConfig::MonsterWidth, GameConfig::GreenMonsterHeight,
              screenWidth, initialDirection, initialHealth)
{
}
