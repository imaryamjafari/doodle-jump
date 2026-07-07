#pragma once

#include <memory>
#include <SFML/Graphics.hpp>
#include "entities/Entity.hpp"
#include "entities/Spring.hpp"

enum class PlatformType
{
    Normal,
    Moving,
    Breakable
};

class Platform : public Entity
{
public:
    Platform(sf::Texture& texture, const sf::Vector2f& position,
             float width, float height, PlatformType type);
    ~Platform() override = default;

    void update(float deltaTime);

    // true if the landing should cause a normal jump, false for Breakable platform
    virtual bool onLand() = 0;

    virtual bool isAlive() const = 0;

    void attachSpring(sf::Texture& springTexture);
    bool hasSpring() const;
    Spring* getSpring() const;

    void render(sf::RenderWindow& window) const override;
    sf::FloatRect getBounds() const override;

    sf::Vector2f getPosition() const override;
    void setPosition(const sf::Vector2f& position) override;

    PlatformType getType() const;
    float getWidth() const;
    float getHeight() const;

protected:
    virtual void updateMotion(float deltaTime) = 0;

    sf::Sprite sprite;
    float width;
    float height;
    PlatformType type;

private:
    std::unique_ptr<Spring> spring;
};
