#include "states/MenuState.hpp"
#include "core/Game.hpp"
#include "core/Constants.hpp"
#include <sstream>

MenuState::MenuState(Game& game_)
    : GameState(game_)
    , startButtonSprite(game.getTextures().get(std::string(GameConfig::AssetsFolder) + GameConfig::StartButtonTexture))
    , titleText(game.getFonts().get(std::string(GameConfig::FontsFolder) + GameConfig::FontFile))
    , highScoreText(game.getFonts().get(std::string(GameConfig::FontsFolder) + GameConfig::FontFile))
{
    const sf::Vector2u buttonTextureSize = startButtonSprite.getTexture().getSize();
    startButtonSprite.setScale({
        static_cast<float>(GameConfig::ButtonWidth) / static_cast<float>(buttonTextureSize.x),
        static_cast<float>(GameConfig::ButtonHeight) / static_cast<float>(buttonTextureSize.y)});
    startButtonSprite.setPosition({
        GameConfig::WindowWidth / 2.f - GameConfig::ButtonWidth / 2.f,
        GameConfig::WindowHeight / 2.f});

    titleText.setString("Doodle Jump");
    titleText.setCharacterSize(GameConfig::TitleFontSize);
    titleText.setFillColor(sf::Color(60, 60, 60));
    const sf::FloatRect titleBounds = titleText.getLocalBounds();
    titleText.setOrigin({titleBounds.size.x / 2.f, titleBounds.size.y / 2.f});
    titleText.setPosition({GameConfig::WindowWidth / 2.f, GameConfig::WindowHeight * GameConfig::TitleVerticalPositionRatio});

    highScoreText.setCharacterSize(GameConfig::ScoreFontSize);
    highScoreText.setFillColor(sf::Color(30, 130, 60));
    highScoreText.setStyle(sf::Text::Bold);
}

void MenuState::handleEvent(const sf::Event& event)
{
    if (const auto* mouseButtonPressed = event.getIf<sf::Event::MouseButtonPressed>())
    {
        if (mouseButtonPressed->button == sf::Mouse::Button::Left)
        {
            const sf::Vector2f clickPosition(static_cast<float>(mouseButtonPressed->position.x),
                                              static_cast<float>(mouseButtonPressed->position.y));
            if (startButtonSprite.getGlobalBounds().contains(clickPosition))
            {
                game.requestStateChange(StateID::Play);
            }
        }
    }
}

void MenuState::update(float /*deltaTime*/)
{
    std::ostringstream stream;
    stream << "High Score: " << static_cast<int>(game.getHighScore());
    highScoreText.setString(stream.str());
    const sf::FloatRect bounds = highScoreText.getLocalBounds();
    highScoreText.setOrigin({bounds.size.x / 2.f, bounds.size.y / 2.f});
    highScoreText.setPosition({GameConfig::WindowWidth / 2.f,
        GameConfig::WindowHeight * GameConfig::TitleVerticalPositionRatio + GameConfig::SubTextSpacingFromTitle});
}

void MenuState::render(sf::RenderWindow& window)
{
    window.setView(window.getDefaultView());
    window.clear(sf::Color(245, 240, 230));
    window.draw(titleText);
    window.draw(highScoreText);
    window.draw(startButtonSprite);
}
