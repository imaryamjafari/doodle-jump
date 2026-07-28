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

    // New in Phase 2: extra constructor overload that also takes the two
    // shooting-pose textures. Kept as a separate overload (rather than
    // changing the original constructor's signature) so any code still
    // calling the four-texture constructor keeps compiling unchanged.
    Player(sf::Texture& leftTexture, sf::Texture& rightTexture,
           sf::Texture& leftTuckedTexture, sf::Texture& rightTuckedTexture,
           sf::Texture& shootingTexture, sf::Texture& shootingTuckedTexture,
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

    // ---- New in Phase 2: shooting ----
    // Whether the Space key is currently held. Purely a visual/input flag;
    // actual bullet spawning and fire-rate cooldown are owned by PlayState
    // (which also owns BulletManager), so Player itself never creates a
    // Bullet — it only reports the pose and the muzzle point.
    void setShooting(bool shooting);
    bool isShooting() const;
    bool isLegsTucked() const;

    // World-space position bullets should spawn from: a fixed point on the
    // player's body (their "nose"), independent of movement/jumping state.
    sf::Vector2f getMuzzlePosition() const;

    // New in Phase 2: scales the player's sprite down toward nothing (1 =
    // normal size, 0 = invisible), used only by PlayState's hole-suck
    // sequence to visually shrink the player as they're pulled in.
    void setShrinkFactor(float factor);

private:
    void refreshSprite();

    sf::Texture& leftTexture;
    sf::Texture& rightTexture;
    sf::Texture& leftTuckedTexture;
    sf::Texture& rightTuckedTexture;

    // New in Phase 2: optional shooting-pose textures. Pointers rather than
    // references so the original 4-texture constructor (which never
    // supplies them) can leave these null; refreshSprite() falls back to
    // the normal/tucked textures whenever they are null or isShooting() is
    // false.
    sf::Texture* shootingTexture;
    sf::Texture* shootingTuckedTexture;

    sf::Sprite sprite;

    sf::Vector2f velocity;

    FacingDirection facingDirection;
    bool legsTucked;
    float legsTuckedTimeRemaining;

    float score;

    bool shooting; // New in Phase 2: true while Space is held
};
