#include "states/GameOverState.hpp"
#include "core/Game.hpp"
#include "core/Constants.hpp"
#include <sstream>

GameOverState::GameOverState(Game& game_)
    : GameState(game_)
    , messageText(game.getFonts().get(std::string(GameConfig::FontsFolder) + GameConfig::FontFile))
    , scoreText(game.getFonts().get(std::string(GameConfig::FontsFolder) + GameConfig::FontFile))
    , highScoreText(game.getFonts().get(std::string(GameConfig::FontsFolder) + GameConfig::FontFile))
    , restartButtonSprite(game.getTextures().get(std::string(GameConfig::AssetsFolder) + GameConfig::RestartButtonTexture))
    , menuButtonSprite(game.getTextures().get(std::string(GameConfig::AssetsFolder) + GameConfig::MenuButtonTexture))
{
    messageText.setString("YOU LOST");
    messageText.setCharacterSize(GameConfig::MessageFontSize);
    messageText.setFillColor(sf::Color(200, 40, 40));
    messageText.setStyle(sf::Text::Bold);
    const sf::FloatRect messageBounds = messageText.getLocalBounds();
    messageText.setOrigin({messageBounds.size.x / 2.f, messageBounds.size.y / 2.f});
    messageText.setPosition({GameConfig::WindowWidth / 2.f, GameConfig::WindowHeight * GameConfig::TitleVerticalPositionRatio});

    scoreText.setCharacterSize(GameConfig::ScoreFontSize);
    scoreText.setFillColor(sf::Color(50, 50, 50));
    scoreText.setStyle(sf::Text::Bold);

    highScoreText.setCharacterSize(GameConfig::ScoreFontSize);
    highScoreText.setFillColor(sf::Color(30, 130, 60));
    highScoreText.setStyle(sf::Text::Bold);

    const sf::Vector2u restartTextureSize = restartButtonSprite.getTexture().getSize();
    restartButtonSprite.setScale({
        static_cast<float>(GameConfig::ButtonWidth) / static_cast<float>(restartTextureSize.x),
        static_cast<float>(GameConfig::ButtonHeight) / static_cast<float>(restartTextureSize.y)});
    restartButtonSprite.setPosition({
        GameConfig::WindowWidth / 2.f - GameConfig::ButtonWidth / 2.f,
        GameConfig::WindowHeight / 2.f});

    const sf::Vector2u menuTextureSize = menuButtonSprite.getTexture().getSize();
    menuButtonSprite.setScale({
        static_cast<float>(GameConfig::ButtonWidth) / static_cast<float>(menuTextureSize.x),
        static_cast<float>(GameConfig::ButtonHeight) / static_cast<float>(menuTextureSize.y)});
    menuButtonSprite.setPosition({
        GameConfig::WindowWidth / 2.f - GameConfig::ButtonWidth / 2.f,
        GameConfig::WindowHeight / 2.f + GameConfig::ButtonHeight + GameConfig::ButtonSpacing});
}

void GameOverState::handleEvent(const sf::Event& event)
{
    if (const auto* mouseButtonPressed = event.getIf<sf::Event::MouseButtonPressed>())
    {
        if (mouseButtonPressed->button == sf::Mouse::Button::Left)
        {
            const sf::Vector2f clickPosition(static_cast<float>(mouseButtonPressed->position.x),
                                              static_cast<float>(mouseButtonPressed->position.y));

            if (restartButtonSprite.getGlobalBounds().contains(clickPosition))
            {
                game.requestStateChange(StateID::Play);
            }
            else if (menuButtonSprite.getGlobalBounds().contains(clickPosition))
            {
                game.requestStateChange(StateID::Menu);
            }
        }
    }
}

void GameOverState::update(float /*deltaTime*/)
{
    std::ostringstream scoreStream;
    scoreStream << "Score: " << static_cast<int>(game.getLastScore());
    scoreText.setString(scoreStream.str());
    sf::FloatRect bounds = scoreText.getLocalBounds();
    scoreText.setOrigin({bounds.size.x / 2.f, bounds.size.y / 2.f});
    scoreText.setPosition({GameConfig::WindowWidth / 2.f,
        GameConfig::WindowHeight * GameConfig::TitleVerticalPositionRatio + GameConfig::SubTextSpacingFromTitle});

    std::ostringstream highScoreStream;
    highScoreStream << "High Score: " << static_cast<int>(game.getHighScore());
    highScoreText.setString(highScoreStream.str());
    bounds = highScoreText.getLocalBounds();
    highScoreText.setOrigin({bounds.size.x / 2.f, bounds.size.y / 2.f});
    highScoreText.setPosition({GameConfig::WindowWidth / 2.f,
        GameConfig::WindowHeight * GameConfig::TitleVerticalPositionRatio + GameConfig::SecondSubTextSpacingFromTitle});
}

void GameOverState::render(sf::RenderWindow& window)
{
    window.setView(window.getDefaultView());
    window.clear(sf::Color(245, 240, 230));
    window.draw(messageText);
    window.draw(scoreText);
    window.draw(highScoreText);
    window.draw(restartButtonSprite);
    window.draw(menuButtonSprite);
}
