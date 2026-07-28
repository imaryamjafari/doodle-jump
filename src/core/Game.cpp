#include "core/Game.hpp"
#include "core/Constants.hpp"
#include "states/MenuState.hpp"
#include "states/PlayState.hpp"
#include "states/GameOverState.hpp"
#include "states/SettingsState.hpp" // New in Phase 2
#include <fstream>
#include <ctime>
#include <cstdlib>
#include <optional>

Game::Game()
    : window(sf::VideoMode({GameConfig::WindowWidth, GameConfig::WindowHeight}), GameConfig::WindowTitle,
             sf::Style::Titlebar | sf::Style::Close)
    , pendingState(StateID::Menu)
    , stateChangeRequested(true)
    , lastScore(0.f)
{
    window.setFramerateLimit(GameConfig::FrameRateLimit);
    std::srand(static_cast<unsigned int>(std::time(nullptr)));

    // New in Phase 2: HighScoreStore and Settings load themselves from
    // disk in their own constructors; apply the persisted volume to the
    // sound manager right away so menu music starts at the right level.
    soundManager.setVolume(settings.getVolume());
}

std::unique_ptr<GameState> Game::createState(StateID id)
{
    switch (id)
    {
        case StateID::Menu:     return std::make_unique<MenuState>(*this);
        case StateID::Play:     return std::make_unique<PlayState>(*this);
        case StateID::GameOver: return std::make_unique<GameOverState>(*this);
        case StateID::Settings: return std::make_unique<SettingsState>(*this); // New in Phase 2
    }
    return std::make_unique<MenuState>(*this);
}

void Game::requestStateChange(StateID nextState)
{
    pendingState = nextState;
    stateChangeRequested = true;
}

void Game::run()
{
    sf::Clock clock;

    while (window.isOpen())
    {
        const float deltaTime = clock.restart().asSeconds();

        if (stateChangeRequested)
        {
            currentState = createState(pendingState);
            stateChangeRequested = false;
        }

        while (const std::optional event = window.pollEvent())
        {
            if (event->is<sf::Event::Closed>())
                window.close();
            
            currentState->handleEvent(*event);
        }

        currentState->update(deltaTime);
        currentState->render(window);
        window.display();
    }
}

ResourceManager<sf::Texture>& Game::getTextures()
{
    return textures;
}

ResourceManager<sf::Font>& Game::getFonts()
{
    return fonts;
}

sf::RenderWindow& Game::getWindow()
{
    return window;
}

float Game::getHighScore() const
{
    // New in Phase 2: delegates to the per-difficulty store, using
    // whichever difficulty is currently selected in Settings.
    return highScores.getHighScore(settings.getDifficulty());
}

void Game::updateHighScoreIfNeeded(float candidateScore)
{
    highScores.updateIfNeeded(settings.getDifficulty(), candidateScore);
}

float Game::getLastScore() const
{
    return lastScore;
}

void Game::setLastScore(float score)
{
    lastScore = score;
}

// ---- New in Phase 2 ----

Settings& Game::getSettings()
{
    return settings;
}

SoundManager& Game::getSoundManager()
{
    return soundManager;
}
