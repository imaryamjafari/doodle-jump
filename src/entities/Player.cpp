#include "entities/Player.hpp"
#include "core/Constants.hpp"
#include <algorithm>

Player::Player(sf::Texture& leftTexture_, sf::Texture& rightTexture_,
               sf::Texture& leftTuckedTexture_, sf::Texture& rightTuckedTexture_,
               const sf::Vector2f& startPosition)
    : leftTexture(leftTexture_)
    , rightTexture(rightTexture_)
    , leftTuckedTexture(leftTuckedTexture_)
    , rightTuckedTexture(rightTuckedTexture_)
    , sprite(rightTexture_)
    , velocity(0.f, 0.f)
    , facingDirection(FacingDirection::Right)
    , legsTucked(false)
    , legsTuckedTimeRemaining(0.f)
    , score(0.f)
{
    refreshSprite();
    sprite.setPosition(startPosition);
}

void Player::handleInput(bool moveLeft, bool moveRight)
{
    if (moveLeft && !moveRight)
    {
        velocity.x = -GameConfig::PlayerHorizontalSpeed;
        facingDirection = FacingDirection::Left;
    }
    else if (moveRight && !moveLeft)
    {
        velocity.x = GameConfig::PlayerHorizontalSpeed;
        facingDirection = FacingDirection::Right;
    }
    else
    {
        velocity.x = 0.f;
    }

    refreshSprite();
}

void Player::moveHorizontally(float deltaTime)
{
    sprite.move({velocity.x * deltaTime, 0.f});
}

void Player::applyGravity(float deltaTime)
{
    velocity.y += GameConfig::Gravity * deltaTime;
    velocity.y = std::min(velocity.y, GameConfig::MaxFallSpeed);

    sprite.move({0.f, velocity.y * deltaTime});
}

void Player::advancePoseTimer(float deltaTime)
{
    if (legsTucked)
    {
        legsTuckedTimeRemaining -= deltaTime;
        if (legsTuckedTimeRemaining <= 0.f)
        {
            legsTucked = false;
            refreshSprite();
        }
    }
}

void Player::jump(float jumpSpeed)
{
    velocity.y = -jumpSpeed;
}

void Player::wrapAroundScreen(float screenWidth)
{
    const sf::FloatRect bounds = sprite.getGlobalBounds();

    if (bounds.position.x + bounds.size.x < 0.f)
    {
        sprite.setPosition({screenWidth, sprite.getPosition().y});
    }
    else if (bounds.position.x > screenWidth)
    {
        sprite.setPosition({-bounds.size.x, sprite.getPosition().y});
    }
}

void Player::render(sf::RenderWindow& window) const
{
    window.draw(sprite);
}

sf::FloatRect Player::getBounds() const
{
    return sprite.getGlobalBounds();
}

sf::Vector2f Player::getPosition() const
{
    return sprite.getPosition();
}

void Player::setPosition(const sf::Vector2f& position)
{
    sprite.setPosition(position);
}

sf::Vector2f Player::getVelocity() const
{
    return velocity;
}

void Player::setVelocityY(float vy)
{
    velocity.y = vy;
}

void Player::setVelocityX(float vx)
{
    velocity.x = vx;
}

bool Player::isMovingDownward() const
{
    return velocity.y > 0.f;
}

void Player::playLandingPose()
{
    legsTucked = true;
    legsTuckedTimeRemaining = GameConfig::LegsTuckedDurationSeconds;
    refreshSprite();
}

FacingDirection Player::getFacingDirection() const
{
    return facingDirection;
}

float Player::getScore() const
{
    return score;
}

void Player::addScoreFromClimb(float pixelsClimbed)
{
    if (pixelsClimbed > 0.f)
    {
        score += pixelsClimbed * GameConfig::ScorePerPixelClimbed;
    }
}

void Player::refreshSprite()
{
    sf::Texture* chosenTexture = nullptr;

    if (facingDirection == FacingDirection::Left)
    {
        chosenTexture = legsTucked ? &leftTuckedTexture : &leftTexture;
    }
    else
    {
        chosenTexture = legsTucked ? &rightTuckedTexture : &rightTexture;
    }

    sprite.setTexture(*chosenTexture);

    const sf::Vector2u textureSize = chosenTexture->getSize();
    if (textureSize.x > 0 && textureSize.y > 0)
    {
        sprite.setScale({GameConfig::PlayerWidth / static_cast<float>(textureSize.x),
                          GameConfig::PlayerHeight / static_cast<float>(textureSize.y)});
    }
}
