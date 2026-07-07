#pragma once

#include <SFML/Graphics.hpp>
#include "entities/Entity.hpp"

class Spring : public Entity
{
public:
    Spring(sf::Texture& texture, float platformX, float platformY,
           float localOffsetX, float width, float height);

    void update(float deltaTime);

    void triggerLaunch();

    void render(sf::RenderWindow& window) const override;
    sf::FloatRect getBounds() const override;

    sf::Vector2f getPosition() const override;
    void setPosition(const sf::Vector2f& position) override;

    void followPlatform(float platformX, float platformY);

private:
    void applyCompressedFrame();
    void applyExtendedFrame();

    sf::Sprite sprite;
    float width;
    float height;
    float localOffsetX; // offset from the platform's left edge

    bool extended;
    float extendedTimeRemaining;
};
