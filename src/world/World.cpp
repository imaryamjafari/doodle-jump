#include "world/World.hpp"
#include "core/Constants.hpp"
#include "world/WorldSpaceOverlap.hpp"

World::World(ResourceManager<sf::Texture>& textures, float screenWidth_, float screenHeight_)
    : screenWidth(screenWidth_)
    , screenHeight(screenHeight_)
    , spawner(textures, screenWidth_, screenHeight_)
    , camera(screenWidth_, screenHeight_)
    , collisions(screenWidth_)
    , monsterSpawner(textures, screenWidth_, screenHeight_) // New in Phase 2
    , holeSpawner(textures, screenWidth_, screenHeight_)    // New in Phase 2
    , bulletManager()                                       // New in Phase 2 (no texture needed - plain round shape)
    , difficulty(GameConfig::DefaultDifficulty)             // New in Phase 2
    , elapsedPlaySeconds(0.f)                               // New in Phase 2
    , lastOutcome(WorldUpdateOutcome::None)                 // New in Phase 2
    , lastCaughtHoleCenter(0.f, 0.f)                        // New in Phase 2
{
}

void World::reset(Player& player)
{
    spawner.reset();
    camera.reset();
    collisions.reset();

    // New in Phase 2.
    monsterSpawner.reset();
    holeSpawner.reset();
    bulletManager.reset();
    elapsedPlaySeconds = 0.f;
    lastOutcome = WorldUpdateOutcome::None;

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

// New in Phase 2.
void World::setDifficulty(Difficulty newDifficulty)
{
    difficulty = newDifficulty;
    monsterSpawner.setDifficulty(difficulty);
    holeSpawner.setDifficulty(difficulty);
}

// New in Phase 2.
void World::fireBullet(const sf::Vector2f& muzzlePosition)
{
    bulletManager.spawnBullet(muzzlePosition);
}

// New in Phase 2.
WorldUpdateOutcome World::getLastOutcome() const
{
    return lastOutcome;
}

// New in Phase 2.
sf::Vector2f World::getLastCaughtHoleCenter() const
{
    return lastCaughtHoleCenter;
}

void World::update(float deltaTime, Player& player)
{
    elapsedPlaySeconds += deltaTime; // New in Phase 2

    spawner.updateAll(deltaTime);
    monsterSpawner.updateAll(deltaTime); // New in Phase 2
    bulletManager.updateAll(deltaTime);  // New in Phase 2

    collisions.resolveHorizontal(player, deltaTime);
    const VerticalCollisionOutcome outcome = collisions.resolveVertical(
        player, deltaTime, spawner.getPlatforms(), monsterSpawner.getMonsters(), holeSpawner.getHoles(),
        &lastCaughtHoleCenter);

    // New in Phase 2: translate the low-level collision outcome into the
    // World-level one PlayState reacts to.
    if (outcome == VerticalCollisionOutcome::CaughtByHole)
    {
        lastOutcome = WorldUpdateOutcome::CaughtByHole;
    }
    else if (outcome == VerticalCollisionOutcome::KilledByMonster)
    {
        lastOutcome = WorldUpdateOutcome::KilledByMonster;
    }
    else
    {
        lastOutcome = WorldUpdateOutcome::None;
    }

    // New in Phase 2: bullets only ever interact with monsters.
    collisions.resolveBulletsAgainstMonsters(bulletManager.getBullets(), monsterSpawner.getMonsters());

    const float climbed = camera.update(player);
    player.addScoreFromClimb(climbed);

    // New in Phase 2: every newly generated platform is offered as a
    // possible anchor for a nearby monster and/or hole. `occupiedRects`
    // starts from everything already in the world so the shared overlap
    // check (see WorldSpaceOverlap) can guarantee nothing new overlaps an
    // existing platform, monster, or hole.
    std::vector<sf::FloatRect> newlyGeneratedPlatformBounds;
    spawner.generateUpTo(camera.getView().getCenter().y - screenHeight, newlyGeneratedPlatformBounds);

    if (!newlyGeneratedPlatformBounds.empty())
    {
        std::vector<sf::FloatRect> occupiedRects = spawner.getAllBounds();
        const std::vector<sf::FloatRect> monsterBounds = monsterSpawner.getAllBounds();
        const std::vector<sf::FloatRect> holeBounds = holeSpawner.getAllBounds();
        occupiedRects.insert(occupiedRects.end(), monsterBounds.begin(), monsterBounds.end());
        occupiedRects.insert(occupiedRects.end(), holeBounds.begin(), holeBounds.end());

        for (const sf::FloatRect& platformBounds : newlyGeneratedPlatformBounds)
        {
            monsterSpawner.maybeSpawnNear(platformBounds, occupiedRects, elapsedPlaySeconds);
            holeSpawner.maybeSpawnNear(platformBounds, occupiedRects, elapsedPlaySeconds);
        }
    }

    spawner.removeStaleBelow(camera.getView().getCenter().y + screenHeight);

    // New in Phase 2.
    monsterSpawner.removeStaleBelow(camera.getView().getCenter().y + screenHeight);
    holeSpawner.removeStaleBelow(camera.getView().getCenter().y + screenHeight);
    bulletManager.removeOffScreen(camera.getView().getCenter().y - screenHeight);
}

void World::render(sf::RenderWindow& window) const
{
    // New in Phase 2: holes render first so platforms/monsters/bullets
    // drawn afterward visually sit above them, matching their being a
    // background-level hazard rather than a solid object.
    for (const auto& hole : holeSpawner.getHoles())
    {
        hole->render(window);
    }

    for (const auto& platform : spawner.getPlatforms())
    {
        platform->render(window); // polymorphic render, each subclass draws itself (+ spring, if any)
    }

    // New in Phase 2.
    for (const auto& monster : monsterSpawner.getMonsters())
    {
        monster->render(window);
    }

    bulletManager.render(window); // New in Phase 2
}

const sf::View& World::getView() const
{
    return camera.getView();
}
