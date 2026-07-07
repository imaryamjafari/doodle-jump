#pragma once

#include <memory>
#include "states/GameState.hpp"
#include "entities/Player.hpp"
#include "world/World.hpp"

// No mouse input is used in this state yet

class PlayState : public GameState
{
public:
    explicit PlayState(Game& game);

    void handleEvent(const sf::Event& event) override;
    void update(float deltaTime) override;
    void render(sf::RenderWindow& window) override;

private:
    sf::Sprite backgroundSprite;
    sf::Text scoreText;

    std::unique_ptr<Player> player;
    std::unique_ptr<World> world;

    bool moveLeftPressed;
    bool moveRightPressed;
};
