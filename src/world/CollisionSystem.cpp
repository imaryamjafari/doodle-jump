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

void CollisionSystem::resolveVertical(Player& player, float deltaTime,
                                       std::vector<std::unique_ptr<Platform>>& platforms)
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

    if (!wasMovingDownward && attachedBreakingPlatform == nullptr)
        return;

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
                return;
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
            return;
        }
    }
}
