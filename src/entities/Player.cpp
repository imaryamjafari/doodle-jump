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
    , shootingTexture(nullptr)       // New in Phase 2: this overload has no shooting pose
    , shootingTuckedTexture(nullptr) // New in Phase 2
    , sprite(rightTexture_)
    , velocity(0.f, 0.f)
    , facingDirection(FacingDirection::Right)
    , legsTucked(false)
    , legsTuckedTimeRemaining(0.f)
    , score(0.f)
    , shooting(false) // New in Phase 2
{
    refreshSprite();
    sprite.setPosition(startPosition);
}

// New in Phase 2: constructor overload that also wires up the shooting-pose
// textures. Delegates to the same member layout as the original
// constructor, only adding the two shooting textures.
Player::Player(sf::Texture& leftTexture_, sf::Texture& rightTexture_,
               sf::Texture& leftTuckedTexture_, sf::Texture& rightTuckedTexture_,
               sf::Texture& shootingTexture_, sf::Texture& shootingTuckedTexture_,
               const sf::Vector2f& startPosition)
    : leftTexture(leftTexture_)
    , rightTexture(rightTexture_)
    , leftTuckedTexture(leftTuckedTexture_)
    , rightTuckedTexture(rightTuckedTexture_)
    , shootingTexture(&shootingTexture_)
    , shootingTuckedTexture(&shootingTuckedTexture_)
    , sprite(rightTexture_)
    , velocity(0.f, 0.f)
    , facingDirection(FacingDirection::Right)
    , legsTucked(false)
    , legsTuckedTimeRemaining(0.f)
    , score(0.f)
    , shooting(false)
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

bool Player::isLegsTucked() const
{
    return legsTucked;
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

// ---- New in Phase 2: shooting ----

void Player::setShooting(bool shooting_)
{
    if (shooting != shooting_)
    {
        shooting = shooting_;
        refreshSprite();
    }
}

bool Player::isShooting() const
{
    return shooting;
}

sf::Vector2f Player::getMuzzlePosition() const
{
    // Fixed point at the horizontal center, near the top of the sprite,
    // independent of facing direction or jump/fall state — bullets always
    // originate from the same relative spot on the player's body.
    const sf::FloatRect bounds = getBounds();
    return sf::Vector2f(bounds.position.x + bounds.size.x / 2.f, bounds.position.y);
}

// New in Phase 2.
void Player::setShrinkFactor(float factor)
{
    const sf::Texture& currentTexture = sprite.getTexture();
    const sf::Vector2u textureSize = currentTexture.getSize();
    if (textureSize.x > 0 && textureSize.y > 0)
    {
        const float clampedFactor = std::max(factor, 0.f);
        sprite.setScale({
            (GameConfig::PlayerWidth / static_cast<float>(textureSize.x)) * clampedFactor,
            (GameConfig::PlayerHeight / static_cast<float>(textureSize.y)) * clampedFactor});
    }
}

void Player::refreshSprite()
{
    sf::Texture* chosenTexture = nullptr;

    // New in Phase 2
    if (shooting && shootingTexture != nullptr && shootingTuckedTexture != nullptr)
    {
        chosenTexture = legsTucked ? shootingTuckedTexture : shootingTexture;
    }
    else if (facingDirection == FacingDirection::Left)
    {
        chosenTexture = legsTucked ? &leftTuckedTexture : &leftTexture;
    }
    else
    {
        chosenTexture = legsTucked ? &rightTuckedTexture : &rightTexture;
    }

    // Bug fix: setTexture()'s resetRect parameter defaults to false in SFML 3,
    // meaning the sprite would keep whatever texture rect was set for the
    // PREVIOUS texture. Since Player swaps between several differently
    // sized textures (e.g. left_doodle.png at 93x84 vs shooting@pose.png at
    // 62x60), leaving the old, larger rect applied to a smaller texture
    // sampled the new texture through stale bounds, producing a visibly
    // distorted/stretched sprite. Passing resetRect=true fixes this by
    // always snapping the rect back to the new texture's real size first.
    sprite.setTexture(*chosenTexture, true);

    const sf::Vector2u textureSize = chosenTexture->getSize();
    if (textureSize.x > 0 && textureSize.y > 0)
    {
        sprite.setScale({GameConfig::PlayerWidth / static_cast<float>(textureSize.x),
                          GameConfig::PlayerHeight / static_cast<float>(textureSize.y)});
    }
}
