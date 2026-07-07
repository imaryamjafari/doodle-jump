#pragma once

#include <vector>
#include <memory>
#include <SFML/Graphics.hpp>
#include "core/ResourceManager.hpp"
#include "platforms/Platform.hpp"

class PlatformSpawner
{
public:
    PlatformSpawner(ResourceManager<sf::Texture>& textures, float screenWidth, float screenHeight);

    void reset();
    void generateUpTo(float topWorldY);
    void updateAll(float deltaTime);
    void removeStaleBelow(float cutoffY);

    std::vector<std::unique_ptr<Platform>>& getPlatforms();
    const std::vector<std::unique_ptr<Platform>>& getPlatforms() const;

    // Position of the very first platform, used to place the player at the start
    sf::FloatRect getFirstPlatformBounds() const;

private:
    PlatformType pickRandomPlatformType() const;
    PlatformType pickNextPlatformType() const;
    std::unique_ptr<Platform> createPlatform(PlatformType type, const sf::Vector2f& position);
    void maybeAttachSpring(Platform& platform);

    ResourceManager<sf::Texture>& textures;

    float screenWidth;
    float screenHeight;

    std::vector<std::unique_ptr<Platform>> platforms;
    float highestGeneratedY; // platform

    // Reachability guarantee
    bool lastPlatformWasBreakable;
};
