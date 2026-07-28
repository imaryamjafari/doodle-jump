#include "world/CollisionSystem.hpp"
#include "core/Constants.hpp"
#include <algorithm>

CollisionSystem::CollisionSystem(float screenWidth_)
    : screenWidth(screenWidth_)
    , attachedBreakingPlatform(nullptr)
{
}

void CollisionSystem::reset()
{
    attachedBreakingPlatform = nullptr;
}

void CollisionSystem::resolveHorizontal(Player& player, float deltaTime)
{
    player.moveHorizontally(deltaTime);
    player.wrapAroundScreen(screenWidth);
}

VerticalCollisionOutcome CollisionSystem::resolveVertical(Player& player, float deltaTime,
                                       std::vector<std::unique_ptr<Platform>>& platforms,
                                       std::vector<std::unique_ptr<Monster>>& monsters,
                                       std::vector<std::unique_ptr<Hole>>& holes,
                                       sf::Vector2f* outCaughtHoleCenter)
{
    const bool wasMovingDownward = player.isMovingDownward();

    if (attachedBreakingPlatform != nullptr)
    {
        auto it = std::find_if(platforms.begin(), platforms.end(),
            [this](const std::unique_ptr<Platform>& platformPtr)
            {
                return platformPtr.get() == static_cast<Platform*>(attachedBreakingPlatform);
            });

        if (it == platforms.end() || !(*it)->isAlive())
            attachedBreakingPlatform = nullptr; // fall continues under gravity from here
    
        else
        {
            const float fallSpeed = attachedBreakingPlatform->getFallSpeed();
            player.setVelocityY(fallSpeed);
            player.setPosition({player.getPosition().x, player.getPosition().y + fallSpeed * deltaTime});
        }
    }
    else
        player.applyGravity(deltaTime);

    player.advancePoseTimer(deltaTime);

    // New in Phase 2: holes have the highest collision priority of
    // anything in the game — checked before springs, platforms, or
    // monsters, and checked unconditionally (not only while falling),
    // since simply walking/wrapping into one should be just as fatal as
    // landing in one. A simple bounding-box overlap is enough; holes don't
    // need the "crossed the top edge" landing logic that platforms use.
    const sf::FloatRect playerBoundsForHoles = player.getBounds();
    for (const auto& holePtr : holes)
    {
        if (holePtr->getBounds().findIntersection(playerBoundsForHoles).has_value())
        {
            if (outCaughtHoleCenter != nullptr)
            {
                *outCaughtHoleCenter = holePtr->getCenter();
            }
            return VerticalCollisionOutcome::CaughtByHole;
        }
    }

    // monster collisions must be checked every frame regardless of
    // vertical direction — e.g. flying upward through a monster right
    // after a spring/platform bounce must still be fatal. Only the
    // platform/spring "landed on top" logic below is legitimately gated on
    // "currently moving downward" (Doodle Jump platforms are one-way, you
    // can't land on one while moving up through it), so the monster check
    // is pulled out from under that guard and always runs.
    const sf::FloatRect playerBoundsForMonsters = player.getBounds();
    for (auto& monsterPtr : monsters)
    {
        Monster* monster = monsterPtr.get();
        const sf::FloatRect monsterBounds = monster->getBounds();

        if (!monsterBounds.findIntersection(playerBoundsForMonsters).has_value())
        {
            continue;
        }

        const float monsterCenterX = playerBoundsForMonsters.position.x + playerBoundsForMonsters.size.x / 2.f;
        const float feetY = playerBoundsForMonsters.position.y + playerBoundsForMonsters.size.y;
        const float prevFeetY = feetY - player.getVelocity().y * deltaTime;

        const bool horizontallyOverlapping =
            monsterCenterX >= monsterBounds.position.x && monsterCenterX <= monsterBounds.position.x + monsterBounds.size.x;
        const bool landedOnTop =
            wasMovingDownward && horizontallyOverlapping &&
            prevFeetY <= monsterBounds.position.y && feetY >= monsterBounds.position.y;

        if (landedOnTop)
        {
            attachedBreakingPlatform = nullptr;
            player.setPosition({playerBoundsForMonsters.position.x, monsterBounds.position.y - playerBoundsForMonsters.size.y});
            player.jump(GameConfig::MonsterBounceJumpSpeed);
            player.playLandingPose();
            return VerticalCollisionOutcome::None;
        }

        // Any other kind of contact (side, bottom, or hitting it while
        // moving upward) is instantly fatal.
        return VerticalCollisionOutcome::KilledByMonster;
    }

    if (!wasMovingDownward && attachedBreakingPlatform == nullptr)
        return VerticalCollisionOutcome::None;

    const sf::FloatRect playerBounds = player.getBounds();
    const float playerFeetY = playerBounds.position.y + playerBounds.size.y;
    const float playerPrevFeetY = playerFeetY - player.getVelocity().y * deltaTime;
    const float playerCenterX = playerBounds.position.x + playerBounds.size.x / 2.f;

    for (auto& platformPtr : platforms)
    {
        Platform* platform = platformPtr.get(); // dynamic binding below

        if (platform == static_cast<Platform*>(attachedBreakingPlatform))
            continue; // already riding this one, it cannot catch itself again

        if (platform->hasSpring())
        {
            Spring* spring = platform->getSpring();
            const sf::FloatRect springBounds = spring->getBounds();

            const bool horizontallyOverlapping =
                playerCenterX >= springBounds.position.x && playerCenterX <= springBounds.position.x + springBounds.size.x;
            const bool crossedTopEdge =
                playerPrevFeetY <= springBounds.position.y && playerFeetY >= springBounds.position.y;

            if (horizontallyOverlapping && crossedTopEdge)
            {
                attachedBreakingPlatform = nullptr;
                player.setPosition({playerBounds.position.x, springBounds.position.y - playerBounds.size.y});
                player.jump(GameConfig::SpringJumpSpeed);
                player.playLandingPose();
                spring->triggerLaunch();
                return VerticalCollisionOutcome::None;
            }
        }

        const sf::FloatRect platformBounds = platform->getBounds();

        const bool horizontallyOverlapping =
            playerCenterX >= platformBounds.position.x && playerCenterX <= platformBounds.position.x + platformBounds.size.x;
        const bool crossedTopEdge =
            playerPrevFeetY <= platformBounds.position.y && playerFeetY >= platformBounds.position.y;

        if (horizontallyOverlapping && crossedTopEdge)
        {
            player.setPosition({playerBounds.position.x, platformBounds.position.y - playerBounds.size.y});

            const bool shouldJump = platform->onLand();
            player.playLandingPose();

            if (shouldJump)
            {
                attachedBreakingPlatform = nullptr;
                player.jump(GameConfig::PlayerJumpSpeed);
            }
            else if (auto* breakable = dynamic_cast<BreakablePlatform*>(platform))
            {
                attachedBreakingPlatform = breakable;
                player.setVelocityY(breakable->getFallSpeed());
            }
            return VerticalCollisionOutcome::None;
        }
    }

    return VerticalCollisionOutcome::None;
}

// New in Phase 2: bullets only ever interact with monsters. Each bullet
// that overlaps a monster applies one hit to it and is itself removed
// immediately (a bullet is consumed on impact, per the requirements), so
// neither list can grow without bound.
void CollisionSystem::resolveBulletsAgainstMonsters(std::vector<std::unique_ptr<Bullet>>& bullets,
                                                     std::vector<std::unique_ptr<Monster>>& monsters)
{
    for (auto bulletIt = bullets.begin(); bulletIt != bullets.end(); )
    {
        const sf::FloatRect bulletBounds = (*bulletIt)->getBounds();
        bool bulletConsumed = false;

        for (auto& monsterPtr : monsters)
        {
            if (!monsterPtr->isAlive())
            {
                continue;
            }

            if (bulletBounds.findIntersection(monsterPtr->getBounds()).has_value())
            {
                monsterPtr->applyBulletHit();
                bulletConsumed = true;
                break;
            }
        }

        if (bulletConsumed)
        {
            bulletIt = bullets.erase(bulletIt);
        }
        else
        {
            ++bulletIt;
        }
    }
}
