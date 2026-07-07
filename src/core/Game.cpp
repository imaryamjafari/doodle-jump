#include "core/Game.hpp"
#include "core/Constants.hpp"
#include "states/MenuState.hpp"
#include "states/PlayState.hpp"
#include "states/GameOverState.hpp"
#include <fstream>
#include <ctime>
#include <cstdlib>
#include <optional>

Game::Game()
    : window(sf::VideoMode({GameConfig::WindowWidth, GameConfig::WindowHeight}), GameConfig::WindowTitle,
             sf::Style::Titlebar | sf::Style::Close)
    , pendingState(StateID::Menu)
    , stateChangeRequested(true)
    , highScore(0.f)
    , lastScore(0.f)
{
    window.setFramerateLimit(GameConfig::FrameRateLimit);
    std::srand(static_cast<unsigned int>(std::time(nullptr)));
    loadHighScore();
}

std::unique_ptr<GameState> Game::createState(StateID id)
{
    switch (id)
    {
        case StateID::Menu:     return std::make_unique<MenuState>(*this);
        case StateID::Play:     return std::make_unique<PlayState>(*this);
        case StateID::GameOver: return std::make_unique<GameOverState>(*this);
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

    saveHighScore();
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
    return highScore;
}

void Game::updateHighScoreIfNeeded(float candidateScore)
{
    if (candidateScore > highScore)
    {
        highScore = candidateScore;
        saveHighScore();
    }
}

float Game::getLastScore() const
{
    return lastScore;
}

void Game::setLastScore(float score)
{
    lastScore = score;
}

void Game::loadHighScore()
{
    std::ifstream file(GameConfig::HighScoreFile);
    if (file.is_open())
        file >> highScore;
}

void Game::saveHighScore() const
{
    std::ofstream file(GameConfig::HighScoreFile, std::ios::trunc);
    if (file.is_open())
        file << highScore;
}
