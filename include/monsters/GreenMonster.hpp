#pragma once

#include "monsters/Monster.hpp"

class GreenMonster : public Monster
{
public:
    GreenMonster(sf::Texture& texture, const sf::Vector2f& position,
                 float screenWidth, float initialDirection, int initialHealth);
};
