#pragma once

#include <vector>
#include <memory>
#include <SFML/Graphics.hpp>
#include "entities/Player.hpp"
#include "platforms/Platform.hpp"
#include "platforms/BreakablePlatform.hpp"
#include "monsters/Monster.hpp"
#include "entities/Hole.hpp"
#include "entities/Bullet.hpp"

// New in Phase 2: reports back to PlayState what (if anything) special
// happened during this frame's vertical collision resolution, so PlayState
// can react (end the game, start the hole-suck sequence, etc.) without
// CollisionSystem needing to know about game states or scoring.
enum class VerticalCollisionOutcome
{
    None,
    CaughtByHole,   // player must be sucked in and the game must end
    KilledByMonster // player touched a monster from a side other than the top
};

class CollisionSystem
{
public:
    explicit CollisionSystem(float screenWidth);

    void reset();

    void resolveHorizontal(Player& player, float deltaTime);

    // New in Phase 2: now also takes the active monsters and holes, and
    // returns a VerticalCollisionOutcome describing anything PlayState
    // needs to react to beyond the normal bounce/landing handled here.
    // `outCaughtHoleCenter`, if non-null, is filled with the center of the
    // hole that caught the player when the outcome is CaughtByHole.
    VerticalCollisionOutcome resolveVertical(Player& player, float deltaTime,
                          std::vector<std::unique_ptr<Platform>>& platforms,
                          std::vector<std::unique_ptr<Monster>>& monsters,
                          std::vector<std::unique_ptr<Hole>>& holes,
                          sf::Vector2f* outCaughtHoleCenter = nullptr);

    // New in Phase 2: bullets only ever interact with monsters (never
    // platforms, springs, or holes), so this is a deliberately separate,
    // much simpler check rather than folded into resolveVertical above.
    // Removes any bullet that hit a monster from `bullets` and applies the
    // hit to that monster.
    void resolveBulletsAgainstMonsters(std::vector<std::unique_ptr<Bullet>>& bullets,
                                        std::vector<std::unique_ptr<Monster>>& monsters);

private:
    float screenWidth;
    BreakablePlatform* attachedBreakingPlatform;
};
