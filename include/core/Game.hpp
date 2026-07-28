#pragma once
#include <memory>
#include <string>
#include <SFML/Graphics.hpp>
#include "core/ResourceManager.hpp"
#include "core/ResourceLoader.hpp"
#include "core/Difficulty.hpp"
#include "core/Settings.hpp"
#include "core/HighScoreStore.hpp"
#include "core/SoundManager.hpp"
#include "states/GameState.hpp"

enum class StateID
{
    Menu,
    Play,
    GameOver,
    Settings // New in Phase 2
};

class Game
{
public:
    Game();
    void run();

    void requestStateChange(StateID nextState);

    ResourceManager<sf::Texture>& getTextures();
    ResourceManager<sf::Font>& getFonts();

    sf::RenderWindow& getWindow();

    // getHighScore()/updateHighScoreIfNeeded() now operate on whichever
    // difficulty is currently selected in `settings`, so every existing
    // caller (MenuState, GameOverState, PlayState) keeps working exactly
    // as before without needing to pass a Difficulty around explicitly.
    float getHighScore() const;
    void updateHighScoreIfNeeded(float candidateScore);

    float getLastScore() const;
    void setLastScore(float score);

    // ---- New in Phase 2 ----
    Settings& getSettings();
    SoundManager& getSoundManager();

private:
    std::unique_ptr<GameState> createState(StateID id);

    sf::RenderWindow window;

    ResourceManager<sf::Texture> textures;
    ResourceManager<sf::Font> fonts;

    std::unique_ptr<GameState> currentState;
    StateID pendingState;
    bool stateChangeRequested;

    // New in Phase 2: replaces the old single `float highScore` with one
    // independent high score per difficulty (see HighScoreStore), plus the
    // persisted volume/difficulty (Settings) and all audio (SoundManager).
    HighScoreStore highScores;
    Settings settings;
    SoundManager soundManager;

    float lastScore;
};
