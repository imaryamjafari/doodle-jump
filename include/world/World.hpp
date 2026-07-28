#pragma once

#include <SFML/Graphics.hpp>
#include "core/ResourceManager.hpp"
#include "core/Difficulty.hpp"
#include "entities/Player.hpp"
#include "world/PlatformSpawner.hpp"
#include "world/Camera.hpp"
#include "world/CollisionSystem.hpp"
#include "world/MonsterSpawner.hpp"
#include "world/HoleSpawner.hpp"
#include "world/BulletManager.hpp"

// New in Phase 2: outcome World::update reports back to PlayState so it can
// react (end the game, start the hole-suck sequence) without World needing
// to know about game states, sound, or scoring.
enum class WorldUpdateOutcome
{
    None,
    CaughtByHole,
    KilledByMonster
};

class World
{
public:
    World(ResourceManager<sf::Texture>& textures, float screenWidth, float screenHeight);
    void reset(Player& player);
    bool checkFellBelowScreen(const Player& player) const;

    // New in Phase 2: World needs to know the active difficulty to size
    // monster health, hole/monster spawn eligibility, and (via Player, from
    // PlayState) the fire rate. Call this any time after reset() too, so
    // changing Settings mid-run (if ever allowed) would take effect.
    void setDifficulty(Difficulty difficulty);

    void update(float deltaTime, Player& player);
    void render(sf::RenderWindow& window) const;

    const sf::View& getView() const;

    // New in Phase 2: PlayState calls this once per shot; World owns
    // BulletManager so bullet creation goes through here rather than
    // exposing BulletManager itself.
    void fireBullet(const sf::Vector2f& muzzlePosition);

    // New in Phase 2: result of the most recent update() call's collision
    // resolution, for anything beyond ordinary platform/spring bouncing.
    WorldUpdateOutcome getLastOutcome() const;

    // New in Phase 2: valid only when getLastOutcome() == CaughtByHole;
    // the world-space center of the hole that caught the player, so
    // PlayState can animate the player being pulled toward it.
    sf::Vector2f getLastCaughtHoleCenter() const;

private:
    float screenWidth;
    float screenHeight;

    PlatformSpawner spawner;
    Camera camera;
    CollisionSystem collisions;
    MonsterSpawner monsterSpawner;
    HoleSpawner holeSpawner;
    BulletManager bulletManager;

    Difficulty difficulty;
    float elapsedPlaySeconds;
    WorldUpdateOutcome lastOutcome;
    sf::Vector2f lastCaughtHoleCenter; // New in Phase 2
};
