#pragma once

#include <vector>
#include <memory>
#include <SFML/Graphics.hpp>
#include "core/ResourceManager.hpp"
#include "core/Difficulty.hpp"
#include "entities/Hole.hpp"

// New in Phase 2.
//
// HoleSpawner
//
// Same generation pattern as MonsterSpawner (spawn near some newly
// generated platforms, subject to the shared overlap check, never
// blocking the only path up, never before the initial safety delay), but
// only ever produces holes when the active difficulty is Hard — on Easy
// and Medium, maybeSpawnNear is a no-op.
class HoleSpawner
{
public:
    HoleSpawner(ResourceManager<sf::Texture>& textures, float screenWidth, float screenHeight);

    void reset();
    void setDifficulty(Difficulty difficulty);

    void maybeSpawnNear(const sf::FloatRect& platformBounds, std::vector<sf::FloatRect>& occupiedRects,
                         float elapsedPlaySeconds);

    void removeStaleBelow(float cutoffY);

    std::vector<std::unique_ptr<Hole>>& getHoles();
    const std::vector<std::unique_ptr<Hole>>& getHoles() const;

    // Bounds of every currently active hole, fed into the shared overlap
    // check alongside platform/monster bounds.
    std::vector<sf::FloatRect> getAllBounds() const;

private:
    ResourceManager<sf::Texture>& textures;
    float screenWidth;
    float screenHeight;
    Difficulty difficulty;

    std::vector<std::unique_ptr<Hole>> holes;
};
