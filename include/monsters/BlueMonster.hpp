#pragma once

#include "monsters/Monster.hpp"

class BlueMonster : public Monster
{
public:
    BlueMonster(sf::Texture& texture, const sf::Vector2f& position,
                float screenWidth, float initialDirection, int initialHealth);

protected:
    void updateFacingVisual() override;

private:
    void applyTextureRectForDirection();

    int frameWidth;  
    int frameHeight;
};
