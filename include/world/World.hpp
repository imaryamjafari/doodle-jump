#pragma once

#include <SFML/Graphics.hpp>
#include "core/ResourceManager.hpp"
#include "entities/Player.hpp"
#include "world/PlatformSpawner.hpp"
#include "world/Camera.hpp"
#include "world/CollisionSystem.hpp"


class World
{
public:
    World(ResourceManager<sf::Texture>& textures, float screenWidth, float screenHeight);
    void reset(Player& player);
    bool checkFellBelowScreen(const Player& player) const;

    void update(float deltaTime, Player& player);
    void render(sf::RenderWindow& window) const;

    const sf::View& getView() const;

private:
    float screenWidth;
    float screenHeight;

    PlatformSpawner spawner;
    Camera camera;
    CollisionSystem collisions;
};
