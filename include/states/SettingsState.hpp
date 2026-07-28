#pragma once

#include "states/GameState.hpp"
#include "core/Difficulty.hpp"

// New in Phase 2.
// Reachable only from the Main Menu (via the Settings button added to
// MenuState) and returns only to the Main Menu (via the Back button) —
// never reachable mid-gameplay.
class SettingsState : public GameState
{
public:
    explicit SettingsState(Game& game);

    void handleEvent(const sf::Event& event) override;
    void update(float deltaTime) override;
    void render(sf::RenderWindow& window) override;

private:
    void applyVolumeFromMouseX(float mouseX);
    void refreshDifficultyButtonColors();
    sf::FloatRect difficultyButtonBounds(int index) const;

    sf::Text titleText;
    sf::RectangleShape volumeTrack;
    sf::CircleShape volumeHandle;
    sf::Text volumeLabel;
    bool draggingVolumeHandle;

    sf::RectangleShape difficultyButtons[3];
    sf::Text difficultyLabels[3];

    sf::Sprite backButtonSprite;
};
