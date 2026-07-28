#pragma once

#include <SFML/Graphics.hpp>
#include "entities/Entity.hpp"


class Bullet : public Entity
{
public:
    explicit Bullet(const sf::Vector2f& startPosition);

    void update(float deltaTime);

    void render(sf::RenderWindow& window) const override;
    sf::FloatRect getBounds() const override;

    sf::Vector2f getPosition() const override;
    void setPosition(const sf::Vector2f& position) override;

private:
    sf::CircleShape shape;
};
