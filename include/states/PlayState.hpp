#pragma once

#include <memory>
#include "states/GameState.hpp"
#include "entities/Player.hpp"
#include "world/World.hpp"

// No mouse input is used in this state yet

class PlayState : public GameState
{
public:
    explicit PlayState(Game& game);

    void handleEvent(const sf::Event& event) override;
    void update(float deltaTime) override;
    void render(sf::RenderWindow& window) override;

private:
    // New in Phase 2: split out of update() so the "player is being sucked
    // into a hole" sequence can run on its own, independent of normal
    // gameplay update, and cleanly transition to GameOver once finished.
    void updateHoleSuckSequence(float deltaTime);
    void endGameAndTransition();

    // New in Phase 2: positions noseSprite glued to the top-center of the
    // player every frame while shooting, and returns the world-space point
    // at the very top of the nose sprite — that is where bullets actually
    // spawn from, matching "a bullet is fired from the tip of the nose,
    // which sits at the top-center of the player".
    sf::Vector2f updateNoseAttachment();

    sf::Sprite backgroundSprite;
    sf::Text scoreText;

    std::unique_ptr<Player> player;
    std::unique_ptr<World> world;

    bool moveLeftPressed;
    bool moveRightPressed;

    // ---- New in Phase 2: shooting ----
    bool spaceHeld;
    float fireCooldownRemaining;
    sf::Sprite noseSprite; // glued to the player's top-center while shooting

    // ---- New in Phase 2: hole-suck sequence ----
    // While true, ordinary physics/collision/camera updates are frozen and
    // the player is instead animated shrinking toward the hole's center;
    // once the timer runs out the game actually ends.
    bool isBeingSuckedIntoHole;
    float holeSuckTimeRemaining;
    sf::Vector2f holeSuckTargetCenter;
    sf::Vector2f holeSuckStartPosition;
};
