#pragma once

#include <vector>
#include <memory>
#include <SFML/Graphics.hpp>
#include "core/ResourceManager.hpp"
#include "platforms/Platform.hpp"
#include "core/Difficulty.hpp"

class PlatformSpawner
{
public:
    PlatformSpawner(ResourceManager<sf::Texture>& textures, float screenWidth, float screenHeight);

    void reset();
    void generateUpTo(float topWorldY);

    // New in Phase 2: same generation logic as generateUpTo(float) above,
    // but additionally appends the bounds of every newly created platform
    // to `newlyGeneratedBounds`, so World can offer each one to
    // MonsterSpawner/HoleSpawner as a possible spawn anchor. The original
    // overload is left completely untouched and still used internally by
    // reset().
    void generateUpTo(float topWorldY, std::vector<sf::FloatRect>& newlyGeneratedBounds);
    void setDifficulty(Difficulty difficulty);

    void updateAll(float deltaTime);
    void removeStaleBelow(float cutoffY);

    std::vector<std::unique_ptr<Platform>>& getPlatforms();
    const std::vector<std::unique_ptr<Platform>>& getPlatforms() const;

    // Position of the very first platform, used to place the player at the start
    sf::FloatRect getFirstPlatformBounds() const;

    // New in Phase 2: bounds of every currently active platform, used to
    // seed the shared overlap check before offering a new platform's
    // vicinity to MonsterSpawner/HoleSpawner.
    std::vector<sf::FloatRect> getAllBounds() const;

private:
    PlatformType pickRandomPlatformType() const;
    PlatformType pickNextPlatformType() const;
    std::unique_ptr<Platform> createPlatform(PlatformType type, const sf::Vector2f& position);
    void maybeAttachSpring(Platform& platform);

    // New in Phase 2: shared implementation for both generateUpTo
    // overloads above. `outNewlyGeneratedBounds` is nullptr from the
    // original overload (nothing extra to report) and non-null from the
    // new one.
    void generateUpToImpl(float topWorldY, std::vector<sf::FloatRect>* outNewlyGeneratedBounds);
    Difficulty difficulty;

    ResourceManager<sf::Texture>& textures;

    float screenWidth;
    float screenHeight;

    std::vector<std::unique_ptr<Platform>> platforms;
    float highestGeneratedY; // platform

    // Reachability guarantee
    bool lastPlatformWasBreakable;
};
