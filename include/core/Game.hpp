#pragma once
#include <memory>
#include <string>
#include <SFML/Graphics.hpp>
#include "core/ResourceManager.hpp"
#include "core/ResourceLoader.hpp"
#include "states/GameState.hpp"

enum class StateID
{
    Menu,
    Play,
    GameOver
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

    float getHighScore() const;
    void updateHighScoreIfNeeded(float candidateScore);

    float getLastScore() const;
    void setLastScore(float score);

private:
    std::unique_ptr<GameState> createState(StateID id);

    void loadHighScore();
    void saveHighScore() const;

    sf::RenderWindow window;

    ResourceManager<sf::Texture> textures;
    ResourceManager<sf::Font> fonts;

    std::unique_ptr<GameState> currentState;
    StateID pendingState;
    bool stateChangeRequested;

    float highScore;
    float lastScore;
};
