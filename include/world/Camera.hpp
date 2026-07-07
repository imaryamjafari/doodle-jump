#pragma once
#include <SFML/Graphics.hpp>
#include "entities/Player.hpp"

class Camera
{
public:
    Camera(float screenWidth, float screenHeight);
    void reset();
    float update(const Player& player);
    const sf::View& getView() const;
    bool hasReachedBottomEdge(const Player& player) const;

private:
    float screenWidth;
    float screenHeight;
    sf::View view;
};
