#pragma once

#include <SFML/Graphics.hpp>

// abstract base for anything that has a position in the game world

class Entity
{
public:
    Entity() = default;
    virtual ~Entity() = default;

    virtual void render(sf::RenderWindow& window) const = 0;
    virtual sf::FloatRect getBounds() const = 0;

    virtual sf::Vector2f getPosition() const = 0;
    virtual void setPosition(const sf::Vector2f& position) = 0;
};
