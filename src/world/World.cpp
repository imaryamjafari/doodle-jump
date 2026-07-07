#include "world/World.hpp"
#include "core/Constants.hpp"

World::World(ResourceManager<sf::Texture>& textures, float screenWidth_, float screenHeight_)
    : screenWidth(screenWidth_)
    , screenHeight(screenHeight_)
    , spawner(textures, screenWidth_, screenHeight_)
    , camera(screenWidth_, screenHeight_)
    , collisions(screenWidth_)
{
}

void World::reset(Player& player)
{
    spawner.reset();
    camera.reset();
    collisions.reset();

    const sf::FloatRect firstBounds = spawner.getFirstPlatformBounds();
    const float startX = firstBounds.position.x + firstBounds.size.x / 2.f - GameConfig::PlayerWidth / 2.f;
    const float startY = firstBounds.position.y - GameConfig::PlayerSpawnOffsetY;
    player.setPosition({startX, startY});

    player.setVelocityY(0.f);
    player.setVelocityX(0.f);
}

bool World::checkFellBelowScreen(const Player& player) const
{
    // The lose condition is deliberately the AND of two things: the player
    // must have reached the bottom edge of the visible screen, AND they
    // must currently be moving downward. This is checked before any
    // collision resolution runs for the frame, so there is no way for the
    // player to land on a platform/spring and bounce back once it fires —
    // the instant both conditions are true, the fall is final.
    return player.isMovingDownward() && camera.hasReachedBottomEdge(player);
}

void World::update(float deltaTime, Player& player)
{
    spawner.updateAll(deltaTime);

    collisions.resolveHorizontal(player, deltaTime);
    collisions.resolveVertical(player, deltaTime, spawner.getPlatforms());

    const float climbed = camera.update(player);
    player.addScoreFromClimb(climbed);

    spawner.generateUpTo(camera.getView().getCenter().y - screenHeight);
    spawner.removeStaleBelow(camera.getView().getCenter().y + screenHeight);
}

void World::render(sf::RenderWindow& window) const
{
    for (const auto& platform : spawner.getPlatforms())
    {
        platform->render(window); // polymorphic render, each subclass draws itself (+ spring, if any)
    }
}

const sf::View& World::getView() const
{
    return camera.getView();
}
