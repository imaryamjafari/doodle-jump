#pragma once

#include <vector>
#include <memory>
#include <SFML/Graphics.hpp>
#include "core/ResourceManager.hpp"
#include "core/Difficulty.hpp"
#include "monsters/Monster.hpp"

// New in Phase 2.
//
// MonsterSpawner
//
// Mirrors PlatformSpawner's responsibilities but for monsters: owns the
// active monster list, decides where new ones appear (next to some newly
// generated platforms, at a limited probability, picking randomly between
// the two visual variants), and removes ones that died (health reached
// zero) or scrolled off-screen — respawning a replacement further up so
// the total active count stays bounded, exactly as required.
class MonsterSpawner
{
public:
    MonsterSpawner(ResourceManager<sf::Texture>& textures, float screenWidth, float screenHeight);

    void reset();
    void setDifficulty(Difficulty difficulty);

    // Called once per newly generated platform; may or may not place a
    // monster near it, subject to the shared overlap check against
    // everything already occupying the world (platforms, other monsters,
    // holes). `occupiedRects` must already contain every rect the new
    // monster must avoid; `elapsedPlaySeconds` enforces the
    // no-monsters-at-the-very-start rule.
    void maybeSpawnNear(const sf::FloatRect& platformBounds, std::vector<sf::FloatRect>& occupiedRects,
                         float elapsedPlaySeconds);

    void updateAll(float deltaTime);
    void removeStaleBelow(float cutoffY);

    std::vector<std::unique_ptr<Monster>>& getMonsters();
    const std::vector<std::unique_ptr<Monster>>& getMonsters() const;

    // Bounds of every currently active monster, fed into the shared
    // overlap check alongside platform/hole bounds.
    std::vector<sf::FloatRect> getAllBounds() const;

private:
    int currentMonsterHealth() const;

    ResourceManager<sf::Texture>& textures;
    float screenWidth;
    float screenHeight;
    Difficulty difficulty;

    std::vector<std::unique_ptr<Monster>> monsters;
};
