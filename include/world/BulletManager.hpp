#pragma once

#include <vector>
#include <memory>
#include <SFML/Graphics.hpp>
#include "entities/Bullet.hpp"

// New in Phase 2.
//
// BulletManager
//
// Owns every currently in-flight bullet. Bullets are created here (one per
// shot, from the player's fixed muzzle point) and are explicitly erased the
// moment they either leave the top of the screen or hit a monster, so the
// active list never grows without bound — satisfying the "manage bullet
// memory" requirement using the same unique_ptr-owned-vector pattern
// already used for platforms/monsters/holes.
//
// Bug fix: Bullet is now a plain round shape with no texture, so
// BulletManager no longer needs a ResourceManager<sf::Texture> at all.
class BulletManager
{
public:
    BulletManager();

    void reset();

    void spawnBullet(const sf::Vector2f& muzzlePosition);

    void updateAll(float deltaTime);
    void render(sf::RenderWindow& window) const;

    // Removes any bullet whose top has scrolled above `topCutoffY` (i.e.
    // left the visible/generated area).
    void removeOffScreen(float topCutoffY);

    std::vector<std::unique_ptr<Bullet>>& getBullets();

private:
    std::vector<std::unique_ptr<Bullet>> bullets;
};
