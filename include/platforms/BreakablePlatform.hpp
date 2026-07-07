#pragma once

#include "platforms/Platform.hpp"

class BreakablePlatform : public Platform
{
public:
    BreakablePlatform(sf::Texture& texture, const sf::Vector2f& position,
                       float width, float height);

    bool onLand() override;
    bool isAlive() const override;

    bool hasBroken() const;
    float getFallSpeed() const;

protected:
    void updateMotion(float deltaTime) override;

private:
    bool broken;
    bool fellOffScreen;
};
