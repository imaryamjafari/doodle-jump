#pragma once

#include <SFML/Graphics.hpp>
#include "entities/Entity.hpp"

enum class FacingDirection
{
    Left,
    Right
};

class Player : public Entity
{
public:
    Player(sf::Texture& leftTexture, sf::Texture& rightTexture,
           sf::Texture& leftTuckedTexture, sf::Texture& rightTuckedTexture,
           const sf::Vector2f& startPosition);

    void handleInput(bool moveLeft, bool moveRight);

    void moveHorizontally(float deltaTime);
    void applyGravity(float deltaTime);
    void advancePoseTimer(float deltaTime);

    void jump(float jumpSpeed);
    void wrapAroundScreen(float screenWidth);

    void render(sf::RenderWindow& window) const override;
    sf::FloatRect getBounds() const override;

    sf::Vector2f getPosition() const override;
    void setPosition(const sf::Vector2f& position) override;

    sf::Vector2f getVelocity() const;
    void setVelocityY(float vy);
    void setVelocityX(float vx);

    bool isMovingDownward() const;
    void playLandingPose();

    FacingDirection getFacingDirection() const;

    float getScore() const;
    void addScoreFromClimb(float pixelsClimbed);

private:
    void refreshSprite();

    sf::Texture& leftTexture;
    sf::Texture& rightTexture;
    sf::Texture& leftTuckedTexture;
    sf::Texture& rightTuckedTexture;
    sf::Sprite sprite;

    sf::Vector2f velocity;

    FacingDirection facingDirection;
    bool legsTucked;
    float legsTuckedTimeRemaining;

    float score;
};
