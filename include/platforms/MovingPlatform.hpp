#pragma once

#include "platforms/Platform.hpp"
#include "core/Difficulty.hpp"

class MovingPlatform : public Platform
{
public:
    MovingPlatform(sf::Texture& texture, const sf::Vector2f& position, float width,
                   float height, float screenWidth, float initialDirection, Difficulty difficulty);

    bool onLand() override;
    bool isAlive() const override;

protected:
    void updateMotion(float deltaTime) override;

private:
    float screenWidth;
    float direction; // +1: right -1: left
    float speed;
};
