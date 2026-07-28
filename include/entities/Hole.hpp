#pragma once

#include <SFML/Graphics.hpp>
#include "entities/Entity.hpp"

class Hole : public Entity
{
public:
    Hole(sf::Texture& texture, const sf::Vector2f& position, float size);

    void render(sf::RenderWindow& window) const override;
    sf::FloatRect getBounds() const override;

    sf::Vector2f getPosition() const override;
    void setPosition(const sf::Vector2f& position) override;

    sf::Vector2f getCenter() const;

private:
    sf::Sprite sprite;
    float size;
};
