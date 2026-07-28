#pragma once

#include "states/GameState.hpp"

// keyboard is not used for navigation in this state.

class MenuState : public GameState
{
public:
    explicit MenuState(Game& game);

    void handleEvent(const sf::Event& event) override;
    void update(float deltaTime) override;
    void render(sf::RenderWindow& window) override;

private:
    sf::Sprite startButtonSprite;
    sf::Text titleText;
    sf::Text highScoreText;

    // New in Phase 2: opens the Settings state; positioned below the
    // Start button.
    sf::Sprite settingsButtonSprite;
};
