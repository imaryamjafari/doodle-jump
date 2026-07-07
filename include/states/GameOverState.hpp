#pragma once

#include "states/GameState.hpp"

class GameOverState : public GameState
{
public:
    explicit GameOverState(Game& game);

    void handleEvent(const sf::Event& event) override;
    void update(float deltaTime) override;
    void render(sf::RenderWindow& window) override;

private:
    sf::Text messageText;
    sf::Text scoreText;
    sf::Text highScoreText;

    sf::Sprite restartButtonSprite;
    sf::Sprite menuButtonSprite;
};
