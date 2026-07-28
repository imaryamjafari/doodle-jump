#pragma once

#include <SFML/Graphics.hpp>
#include "entities/Entity.hpp"

class Monster : public Entity
{
public:
    Monster(sf::Texture& texture, const sf::Vector2f& position,
            float width, float height, float screenWidth, float initialDirection, int initialHealth);
    ~Monster() override = default;

    virtual void update(float deltaTime);

    // Applies one bullet hit; health drops by one. Removal from the active
    // list is decided by whoever owns the monster (MonsterSpawner), based
    // on isAlive() below, not by Monster itself.
    void applyBulletHit();

    bool isAlive() const;

    void render(sf::RenderWindow& window) const override;
    sf::FloatRect getBounds() const override;

    sf::Vector2f getPosition() const override;
    void setPosition(const sf::Vector2f& position) override;

protected:
    virtual void updateFacingVisual();

    sf::Sprite sprite;
    float screenWidth;
    float direction; // +1 moving right, -1 moving left
    int health;
};
