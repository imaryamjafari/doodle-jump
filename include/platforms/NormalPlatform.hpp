#pragma once
#include "platforms/Platform.hpp"

class NormalPlatform : public Platform
{
public:
    NormalPlatform(sf::Texture& texture, const sf::Vector2f& position,
                   float width, float height);

    bool onLand() override;
    bool isAlive() const override;

protected:
    void updateMotion(float deltaTime) override;
};
