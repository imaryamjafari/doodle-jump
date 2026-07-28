#include "world/BulletManager.hpp"
#include <algorithm>

BulletManager::BulletManager() = default;

void BulletManager::reset()
{
    bullets.clear();
}

void BulletManager::spawnBullet(const sf::Vector2f& muzzlePosition)
{
    bullets.push_back(std::make_unique<Bullet>(muzzlePosition));
}

void BulletManager::updateAll(float deltaTime)
{
    for (auto& bullet : bullets)
    {
        bullet->update(deltaTime);
    }
}

void BulletManager::render(sf::RenderWindow& window) const
{
    for (const auto& bullet : bullets)
    {
        bullet->render(window);
    }
}

void BulletManager::removeOffScreen(float topCutoffY)
{
    bullets.erase(
        std::remove_if(bullets.begin(), bullets.end(),
            [topCutoffY](const std::unique_ptr<Bullet>& bullet)
            {
                return bullet->getBounds().position.y + bullet->getBounds().size.y < topCutoffY;
            }),
        bullets.end());
}

std::vector<std::unique_ptr<Bullet>>& BulletManager::getBullets()
{
    return bullets;
}
