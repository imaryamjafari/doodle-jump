#pragma once

#include <vector>
#include <memory>
#include <SFML/Graphics.hpp>
#include "entities/Player.hpp"
#include "platforms/Platform.hpp"
#include "platforms/BreakablePlatform.hpp"

class CollisionSystem
{
public:
    explicit CollisionSystem(float screenWidth);

    void reset();

    void resolveHorizontal(Player& player, float deltaTime);
    void resolveVertical(Player& player, float deltaTime,
                          std::vector<std::unique_ptr<Platform>>& platforms);

private:
    float screenWidth;
    BreakablePlatform* attachedBreakingPlatform;
};
